#include <Wire.h>
#include <math.h>
#include "driver/pcnt.h" 
#include "esp_timer.h" 


#define PIN_L_IN1    25
#define PIN_L_IN2    26
#define PIN_L_PWM    18
#define PIN_R_IN1    32
#define PIN_R_IN2    33
#define PIN_R_PWM    19
#define PIN_STBY     4   

#define PIN_L_ENC_A  27
#define PIN_L_ENC_B  23
#define PIN_R_ENC_A  14
#define PIN_R_ENC_B  13

#define I2C_SDA       21
#define I2C_SCL       22

#define LEDC_FREQ      20000
#define LEDC_RES       10

#define WHEEL_DIAMETER      0.085f      
#define WHEEL_BASE          0.180f      
#define GEAR_RATIO          10.0f       
#define ENCODER_PPR         11.0f       
#define ENCODER_TICKS_PER_REV (ENCODER_PPR * 4.0f) 
#define METERS_PER_TICK     (M_PI * WHEEL_DIAMETER / (ENCODER_TICKS_PER_REV * GEAR_RATIO))

// 编码器方向系数 — 实车验证方法：
//   1. 先设两侧均为 1，手动推车前进 1 米
//   2. serial_bridge 打印 /odom position.x，若为正值则前进方向正确
//   3. 若 position.x 为负值，将两轮系数互换符号（如 1→-1, -1→1）
//   4. 再验证转向：原地旋转 90°，/odom orientation.z 应正确累积
//   5. 最后验证左右轮差速方向是否一致，若两轮转速相反则取反一侧
#define ENC_LEFT_DIR       1
#define ENC_RIGHT_DIR      -1

#define MAX_RAMP_RPM_PER_SEC  100.0f  
#define RPM_FILTER_ALPHA      0.3f    
#define RPM_STOP_THRESHOLD    1.0f    
#define PID_INTEGRAL_LIMIT    2000.0f  
#define MAX_CMD_RPM           200.0f  
#define MIN_START_PWM         150.0f  
#define PID_OUTPUT_LIMIT      1023.0f 

#define LOOP_INTERVAL_US  10000
#define WATCHDOG_TIMEOUT  30000

#define STALL_DETECT_PWM_THRESH  1023.0f
#define STALL_DETECT_RPM_THRESH  1.0f
#define STALL_DETECT_TIME_MS     99999

struct PIDState { 
  float kp, ki, kd;
  float integral;
  float prev_measurement;
  bool  initialized; 
};
PIDState pid_left;
PIDState pid_right;

float target_rpm_left  = 0;
float target_rpm_right = 0;
float target_rpm_left_filtered  = 0;  
float target_rpm_right_filtered = 0;
float actual_rpm_left  = 0;
float actual_rpm_right = 0;
float last_pwm_left = 0;
float last_pwm_right = 0;

float odom_x = 0;
float odom_y = 0;
float odom_theta = 0;

int64_t last_loop_us = 0;
int64_t prev_exec_us = 0; 
unsigned long loop_count = 0; 
unsigned long last_cmd_time = 0;

unsigned long stall_timer_start_l = 0;
unsigned long stall_timer_start_r = 0;
bool stall_fault_l = false;
bool stall_fault_r = false;

#define MPU6050_ADDR 0x68
#define IMU_LP_ALPHA  0.30f
#define CALIB_SAMPLES 200

float gbias_x = 0, gbias_y = 0, gbias_z = 0;
float abias_x = 0, abias_y = 0, abias_z = 0;
bool imu_calibrated = false;
bool imu_data_valid = false; 

float imu_ax = 0, imu_ay = 0, imu_az = 0;
float imu_gx = 0, imu_gy = 0, imu_gz = 0;
float accel_scale = 1.0f;
bool imu_healthy = true;

#define COMP_GAIN      0.85f
#define GYRO_DEADBAND  0.002f
float cf_roll = 0, cf_pitch = 0;
float cf_yaw = 0;

float normalize_angle(float a) {
  while (a > M_PI) a -= 2 * M_PI;
  while (a < -M_PI) a += 2 * M_PI;
  return a;
}

void setup_pcnt(pcnt_unit_t unit, int pulse_pin, int ctrl_pin) {
  pcnt_config_t pcnt_config = {};
  pcnt_config.pulse_gpio_num = pulse_pin;
  pcnt_config.ctrl_gpio_num = ctrl_pin;
  pcnt_config.lctrl_mode = PCNT_MODE_REVERSE;
  pcnt_config.hctrl_mode = PCNT_MODE_KEEP;
  pcnt_config.pos_mode = PCNT_COUNT_INC;
  pcnt_config.neg_mode = PCNT_COUNT_DEC; 
  pcnt_config.counter_h_lim = 32767;
  pcnt_config.counter_l_lim = -32768;
  pcnt_config.unit = unit;
  pcnt_config.channel = PCNT_CHANNEL_0;
  pcnt_unit_config(&pcnt_config);

  pcnt_config_t pcnt_config_ch1 = pcnt_config;
  pcnt_config_ch1.pulse_gpio_num = ctrl_pin; 
  pcnt_config_ch1.ctrl_gpio_num = pulse_pin;
  pcnt_config_ch1.lctrl_mode = PCNT_MODE_KEEP;
  pcnt_config_ch1.hctrl_mode = PCNT_MODE_REVERSE;
  pcnt_config_ch1.channel = PCNT_CHANNEL_1;
  pcnt_unit_config(&pcnt_config_ch1);

  pcnt_set_filter_value(unit, 250); 
  pcnt_filter_enable(unit);
  pcnt_counter_clear(unit);
}

long read_pcnt_left() {
  static int16_t last_val = 0;
  int16_t count = 0;
  if (pcnt_get_counter_value(PCNT_UNIT_0, &count) != ESP_OK) return 0;
  int32_t diff = (int32_t)count - (int32_t)last_val;
  if (diff > 32767) diff -= 65536;
  else if (diff < -32768) diff += 65536;
  last_val = count;
  return diff;
}

long read_pcnt_right() {
  static int16_t last_val = 0;
  int16_t count = 0;
  if (pcnt_get_counter_value(PCNT_UNIT_1, &count) != ESP_OK) return 0;
  int32_t diff = (int32_t)count - (int32_t)last_val;
  if (diff > 32767) diff -= 65536;
  else if (diff < -32768) diff += 65536;
  last_val = count;
  return diff;
}

void setup_motors() {
  pinMode(PIN_L_IN1, OUTPUT); pinMode(PIN_L_IN2, OUTPUT);
  pinMode(PIN_R_IN1, OUTPUT); pinMode(PIN_R_IN2, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  digitalWrite(PIN_STBY, HIGH); 
  ledcAttach(PIN_L_PWM, LEDC_FREQ, LEDC_RES);
  ledcAttach(PIN_R_PWM, LEDC_FREQ, LEDC_RES);
}

float apply_deadzone_ff(float pid_output) {
    if (fabs(pid_output) < 0.01f) return 0.0f;
    float sign = (pid_output > 0) ? 1.0f : -1.0f;
    float abs_out = fabs(pid_output);
    if (abs_out < MIN_START_PWM) {
        abs_out = MIN_START_PWM * sqrtf(abs_out / MIN_START_PWM);
    }
    return sign * abs_out;
}

void set_motor_raw(float left_f, float right_f) {
  if (stall_fault_l) left_f = 0;
  if (stall_fault_r) right_f = 0;
  int16_t left = (int16_t)left_f;
  int16_t right = (int16_t)right_f;
  if (left > 0) { digitalWrite(PIN_L_IN1, LOW); digitalWrite(PIN_L_IN2, HIGH); ledcWrite(PIN_L_PWM, left); } 
  else if (left < 0) { digitalWrite(PIN_L_IN1, HIGH); digitalWrite(PIN_L_IN2, LOW); ledcWrite(PIN_L_PWM, -left); } 
  else { digitalWrite(PIN_L_IN1, LOW); digitalWrite(PIN_L_IN2, LOW); ledcWrite(PIN_L_PWM, 0); }
  if (right > 0) { digitalWrite(PIN_R_IN1, LOW); digitalWrite(PIN_R_IN2, HIGH); ledcWrite(PIN_R_PWM, right); } 
  else if (right < 0) { digitalWrite(PIN_R_IN1, HIGH); digitalWrite(PIN_R_IN2, LOW); ledcWrite(PIN_R_PWM, -right); } 
  else { digitalWrite(PIN_R_IN1, LOW); digitalWrite(PIN_R_IN2, LOW); ledcWrite(PIN_R_PWM, 0); }
}

float pid_compute(PIDState &s, float setpoint, float measurement, float dt) {
  float error = setpoint - measurement;
  if (!s.initialized) { s.prev_measurement = measurement; s.initialized = true; }
  float p_term = s.kp * error;
  float derivative = -(measurement - s.prev_measurement) / dt;
  s.prev_measurement = measurement;
  float d_term = s.kd * derivative;
  float potential_integral = s.integral + error * dt;
  if (potential_integral > PID_INTEGRAL_LIMIT) potential_integral = PID_INTEGRAL_LIMIT;
  if (potential_integral < -PID_INTEGRAL_LIMIT) potential_integral = -PID_INTEGRAL_LIMIT;
  float potential_output = p_term + s.ki * potential_integral + d_term;
  if (potential_output > PID_OUTPUT_LIMIT || potential_output < -PID_OUTPUT_LIMIT) {
    if ((potential_output > PID_OUTPUT_LIMIT && error > 0) || (potential_output < -PID_OUTPUT_LIMIT && error < 0)) { } 
    else { s.integral = potential_integral; }
  } else { s.integral = potential_integral; }
  float output = p_term + s.ki * s.integral + d_term;
  if (output > PID_OUTPUT_LIMIT) return PID_OUTPUT_LIMIT;
  if (output < -PID_OUTPUT_LIMIT) return -PID_OUTPUT_LIMIT;
  return output;
}

void pid_reset(PIDState &s) { s.integral = 0; s.prev_measurement = 0; s.initialized = false; }

int i2c_fail_count = 0;

void recover_i2c() {
  set_motor_raw(0, 0);
  pid_reset(pid_left);
  pid_reset(pid_right);
  Wire.end();
  pinMode(I2C_SCL, OUTPUT_OPEN_DRAIN);
  pinMode(I2C_SDA, INPUT_PULLUP);
  for (int i = 0; i < 9; i++) {
    digitalWrite(I2C_SCL, LOW); delayMicroseconds(5);
    digitalWrite(I2C_SCL, HIGH); delayMicroseconds(5);
  }
  pinMode(I2C_SDA, OUTPUT_OPEN_DRAIN);
  digitalWrite(I2C_SDA, LOW); delayMicroseconds(5);
  digitalWrite(I2C_SCL, HIGH); delayMicroseconds(5);
  digitalWrite(I2C_SDA, HIGH); delayMicroseconds(10);
  Wire.begin(I2C_SDA, I2C_SCL, 400000);
  setup_imu();
  Serial.println("I2C bus recovered, IMU reinitialized");
}

void setup_imu() {
  Wire.setTimeOut(20);
  Wire.beginTransmission(MPU6050_ADDR);
  if (Wire.endTransmission() != 0) {
    imu_healthy = false;
    return;
  }
  imu_healthy = true;
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x6B); Wire.write(0x00); Wire.endTransmission();
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x1A); Wire.write(0x03); Wire.endTransmission();
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x1B); Wire.write(0x08); Wire.endTransmission();
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x1C); Wire.write(0x08); Wire.endTransmission();
}

void calibrate_imu() {
  if (!imu_healthy) return;
  Serial.println("Calibrating IMU, keep robot still...");
  float sx = 0, sy = 0, sz = 0;
  int valid = 0;
  for (int i = 0; i < CALIB_SAMPLES; i++) {
    Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x43); Wire.endTransmission(false);
    if (Wire.requestFrom((uint8_t)MPU6050_ADDR, (uint8_t)6) >= 6) {
      int16_t gx_raw = (Wire.read() << 8) | Wire.read();
      int16_t gy_raw = (Wire.read() << 8) | Wire.read();
      int16_t gz_raw = (Wire.read() << 8) | Wire.read();
      sx += gx_raw / 65.5f * (M_PI / 180.0f);
      sy += gy_raw / 65.5f * (M_PI / 180.0f);
      sz += gz_raw / 65.5f * (M_PI / 180.0f);
      valid++;
    }
    delay(5);
  }
  if (valid > 0) {
    gbias_x = sx / valid;
    gbias_y = sy / valid;
    gbias_z = sz / valid;
    Serial.println("Gyro bias calibration done.");

    float mag_sum = 0;
    int accel_valid = 0;
    float axs = 0, ays = 0, azs = 0;
    for (int i = 0; i < 100; i++) {
      Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x3B); Wire.endTransmission(false);
      if (Wire.requestFrom((uint8_t)MPU6050_ADDR, (uint8_t)6) >= 6) {
        int16_t ax = (Wire.read() << 8) | Wire.read();
        int16_t ay = (Wire.read() << 8) | Wire.read();
        int16_t az = (Wire.read() << 8) | Wire.read();
        axs += ax; ays += ay; azs += az;
        mag_sum += sqrt(ax*ax + ay*ay + az*az) / 8192.0f * 9.81f;
        accel_valid++;
      }
      delay(5);
    }
    if (accel_valid > 20) {
      accel_scale = 9.81f / (mag_sum / accel_valid);
      abias_x = axs / accel_valid;
      abias_y = ays / accel_valid;
      abias_z = azs / accel_valid - 8192;
      Serial.println("Accel calibration done.");
    } else {
      Serial.println("Accel calibration skipped (noisy).");
    }

    imu_calibrated = true;
    read_imu();
    if (imu_data_valid) {
      cf_roll = atan2(-imu_ay, -imu_az);
      cf_pitch = atan2(imu_ax, sqrt(imu_ay * imu_ay + imu_az * imu_az));
    }
  } else {
    Serial.println("IMU Calibration failed! IMU disabled.");
    imu_healthy = false;
  }
}

void read_imu() {
  if (!imu_healthy) { imu_data_valid = false; return; }
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x3B); Wire.endTransmission(false);
  uint8_t bytes_received = Wire.requestFrom((uint8_t)MPU6050_ADDR, (uint8_t)14);
  
  if (bytes_received < 14) {
    imu_data_valid = false;
    i2c_fail_count++;
    if (i2c_fail_count > 20) {
      i2c_fail_count = 0;
      recover_i2c();
    }
    return; 
  }
  
  i2c_fail_count = 0;

  int16_t ax_raw = (Wire.read() << 8) | Wire.read();
  int16_t ay_raw = (Wire.read() << 8) | Wire.read();
  int16_t az_raw = (Wire.read() << 8) | Wire.read();
  Wire.read(); Wire.read();
  int16_t gx_raw = (Wire.read() << 8) | Wire.read();
  int16_t gy_raw = (Wire.read() << 8) | Wire.read();
  int16_t gz_raw = (Wire.read() << 8) | Wire.read();

  float rax = (ax_raw - abias_x) / 8192.0f * 9.81f * accel_scale;
  float ray = (ay_raw - abias_y) / 8192.0f * 9.81f * accel_scale;
  float raz = (az_raw - abias_z) / 8192.0f * 9.81f * accel_scale;
  float rgx = gx_raw / 65.5f * (M_PI / 180.0f);
  float rgy = gy_raw / 65.5f * (M_PI / 180.0f);
  float rgz = gz_raw / 65.5f * (M_PI / 180.0f);

  if (!imu_calibrated) {
    imu_ax = rax; imu_ay = ray; imu_az = raz;
    imu_gx = rgx; imu_gy = rgy; imu_gz = rgz;
  } else {
    imu_ax = IMU_LP_ALPHA * rax + (1.0f - IMU_LP_ALPHA) * imu_ax;
    imu_ay = IMU_LP_ALPHA * ray + (1.0f - IMU_LP_ALPHA) * imu_ay;
    imu_az = IMU_LP_ALPHA * raz + (1.0f - IMU_LP_ALPHA) * imu_az;
    imu_gx = IMU_LP_ALPHA * (rgx - gbias_x) + (1.0f - IMU_LP_ALPHA) * imu_gx;
    imu_gy = IMU_LP_ALPHA * (rgy - gbias_y) + (1.0f - IMU_LP_ALPHA) * imu_gy;
    imu_gz = IMU_LP_ALPHA * (rgz - gbias_z) + (1.0f - IMU_LP_ALPHA) * imu_gz;
  }
  
  imu_data_valid = true; 
}

void printHelp() {
  Serial.println();
  Serial.println("===== ESP32 Board Test =====");
  Serial.println("Commands (send via Serial Monitor, add line ending):");
  Serial.println("  L<rpm>   - Left motor RPM  (e.g. L200)");
  Serial.println("  R<rpm>   - Right motor RPM (e.g. R-150)");
  Serial.println("  F<rpm>   - Forward (both motors)");
  Serial.println("  B<rpm>   - Backward (both motors)");
  Serial.println("  T<rpm>   - Turn in place (left=-rpm, right=+rpm)");
  Serial.println("  S         - Stop");
  Serial.println("  H         - Show this help");
  Serial.println("Example: F 200  → forward at 200 RPM");
  Serial.println("         T 100  → spin at 100 RPM");
  Serial.println("=============================");
  Serial.println();
}

void processSerialCommands() {
  static String buf = "";
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (buf.length() == 0) continue;
      buf.trim();
      char cmd = toupper(buf[0]);
      float val = buf.substring(1).toFloat();
      buf = "";
      switch (cmd) {
        case 'L': target_rpm_left = val; target_rpm_right = 0; break;
        case 'R': target_rpm_left = 0; target_rpm_right = val; break;
        case 'F': target_rpm_left = val; target_rpm_right = val; break;
        case 'B': target_rpm_left = -val; target_rpm_right = -val; break;
        case 'T': target_rpm_left = -val; target_rpm_right = val; break;
        case 'S': target_rpm_left = 0; target_rpm_right = 0; break;
        case 'H': printHelp(); return;
        default:
          Serial.print("Unknown: ");
          Serial.println(cmd);
          return;
      }
      target_rpm_left  = constrain(target_rpm_left,  -MAX_CMD_RPM, MAX_CMD_RPM);
      target_rpm_right = constrain(target_rpm_right, -MAX_CMD_RPM, MAX_CMD_RPM);
      last_cmd_time = millis();
      if (stall_fault_l && fabs(target_rpm_left) < 1.0f) { stall_fault_l = false; stall_timer_start_l = 0; pid_reset(pid_left); }
      if (stall_fault_r && fabs(target_rpm_right) < 1.0f) { stall_fault_r = false; stall_timer_start_r = 0; pid_reset(pid_right); }
      digitalWrite(PIN_STBY, HIGH);
      Serial.print("L:");
      Serial.print(target_rpm_left);
      Serial.print("  R:");
      Serial.println(target_rpm_right);
    } else {
      buf += c;
    }
  }
}

void print_encoder_raw() {
  int16_t cnt0 = 0, cnt1 = 0;
  pcnt_get_counter_value(PCNT_UNIT_0, &cnt0);
  pcnt_get_counter_value(PCNT_UNIT_1, &cnt1);
  Serial.print("  ENC raw L:");
  Serial.print(cnt0);
  Serial.print(" R:");
  Serial.print(cnt1);
}

void setup() {
  Serial.setRxBufferSize(1024);
  Serial.setTxBufferSize(1024);
  Serial.begin(250000);
  delay(500);
  printHelp();
  
  pinMode(PIN_L_ENC_A, INPUT_PULLUP); pinMode(PIN_L_ENC_B, INPUT_PULLUP);
  pinMode(PIN_R_ENC_A, INPUT_PULLUP); pinMode(PIN_R_ENC_B, INPUT_PULLUP);
  setup_pcnt(PCNT_UNIT_0, PIN_L_ENC_A, PIN_L_ENC_B);
  setup_pcnt(PCNT_UNIT_1, PIN_R_ENC_A, PIN_R_ENC_B);
  read_pcnt_left(); read_pcnt_right();
  
  setup_motors();
  Wire.begin(I2C_SDA, I2C_SCL, 400000);
  setup_imu();
  for (int i = 0; i < 10; i++) { read_imu(); delay(10); }
  
  pid_left  = {1.5f, 0.3f, 0.05f, 0, 0, false};
  pid_right = {1.5f, 0.3f, 0.05f, 0, 0, false}; 
  
  calibrate_imu();
  for (int i = 0; i < 50; i++) { read_imu(); delay(5); }

  // 编码器检查
  int16_t ecnt0 = 0, ecnt1 = 0;
  pcnt_get_counter_value(PCNT_UNIT_0, &ecnt0);
  pcnt_get_counter_value(PCNT_UNIT_1, &ecnt1);
  Serial.print("Encoder init check - L:");
  Serial.print(ecnt0);
  Serial.print(" R:");
  Serial.println(ecnt1);
  Serial.println("手转左轮看 L 变不变，转右轮看 R 变不变");
  
  last_loop_us = esp_timer_get_time();
  prev_exec_us = last_loop_us;
}

void loop() {
  int64_t now = esp_timer_get_time();
  if (now - last_loop_us < LOOP_INTERVAL_US) {
    delay(1); 
    return;
  }
  
  float dt = (now - prev_exec_us) / 1000000.0f;
  prev_exec_us = now;
  last_loop_us += LOOP_INTERVAL_US;
  if (now - last_loop_us > LOOP_INTERVAL_US * 2) last_loop_us = now; 
  bool dt_valid = (dt > 0.001f && dt < 0.05f); 

  read_imu();

  processSerialCommands();

  if (millis() - last_cmd_time > WATCHDOG_TIMEOUT) {
    target_rpm_left = 0;
    target_rpm_right = 0;
    target_rpm_left_filtered = 0;
    target_rpm_right_filtered = 0;
    digitalWrite(PIN_STBY, LOW);
    pid_reset(pid_left); pid_reset(pid_right);
    stall_fault_l = false; stall_timer_start_l = 0;
    stall_fault_r = false; stall_timer_start_r = 0;
  }

  float delta_l = read_pcnt_left() * ENC_LEFT_DIR;
  float delta_r = read_pcnt_right() * ENC_RIGHT_DIR;

  float dist_left = delta_l * METERS_PER_TICK;
  float dist_right = delta_r * METERS_PER_TICK;
  float dist_center = (dist_left + dist_right) / 2.0;
  float delta_theta = (dist_right - dist_left) / WHEEL_BASE;
  
  odom_x += dist_center * cos(odom_theta + delta_theta * 0.5f);
  odom_y += dist_center * sin(odom_theta + delta_theta * 0.5f);
  odom_theta += delta_theta;
  odom_theta = normalize_angle(odom_theta);

  if (dt_valid) {
    float raw_rpm_left  = (delta_l / ENCODER_TICKS_PER_REV) * (60.0f / dt) / GEAR_RATIO;
    float raw_rpm_right = (delta_r / ENCODER_TICKS_PER_REV) * (60.0f / dt) / GEAR_RATIO;
    
    actual_rpm_left  = actual_rpm_left * (1.0f - RPM_FILTER_ALPHA) + raw_rpm_left * RPM_FILTER_ALPHA;
    actual_rpm_right = actual_rpm_right * (1.0f - RPM_FILTER_ALPHA) + raw_rpm_right * RPM_FILTER_ALPHA;

    float max_ramp = MAX_RAMP_RPM_PER_SEC * dt;
    target_rpm_left_filtered  += constrain(target_rpm_left  - target_rpm_left_filtered,  -max_ramp, max_ramp);
    target_rpm_right_filtered += constrain(target_rpm_right - target_rpm_right_filtered, -max_ramp, max_ramp);

    if (fabs(target_rpm_left_filtered) > 0.01f || fabs(target_rpm_right_filtered) > 0.01f || 
        fabs(actual_rpm_left) > RPM_STOP_THRESHOLD || fabs(actual_rpm_right) > RPM_STOP_THRESHOLD) {
      
      last_pwm_left = 0; last_pwm_right = 0;
      if (!stall_fault_l) {
        last_pwm_left = pid_compute(pid_left, target_rpm_left_filtered, actual_rpm_left, dt);
        last_pwm_left = apply_deadzone_ff(last_pwm_left);
      }
      if (!stall_fault_r) {
        last_pwm_right = pid_compute(pid_right, target_rpm_right_filtered, actual_rpm_right, dt);
        last_pwm_right = apply_deadzone_ff(last_pwm_right);
      }

      if (fabs(last_pwm_left) > STALL_DETECT_PWM_THRESH && fabs(actual_rpm_left) < STALL_DETECT_RPM_THRESH
          && fabs(target_rpm_left_filtered) > 20.0f) {
        if (stall_timer_start_l == 0) stall_timer_start_l = millis();
        else if (millis() - stall_timer_start_l > STALL_DETECT_TIME_MS) {
          if (!stall_fault_l) { 
            stall_fault_l = true; 
            pid_reset(pid_left); 
            Serial.println("Stall detected on LEFT motor!"); 
          }
        }
      } else { stall_timer_start_l = 0; }
      
      if (fabs(last_pwm_right) > STALL_DETECT_PWM_THRESH && fabs(actual_rpm_right) < STALL_DETECT_RPM_THRESH
          && fabs(target_rpm_right_filtered) > 20.0f) {
        if (stall_timer_start_r == 0) stall_timer_start_r = millis();
        else if (millis() - stall_timer_start_r > STALL_DETECT_TIME_MS) {
          if (!stall_fault_r) { 
            stall_fault_r = true; 
            pid_reset(pid_right); 
            Serial.println("Stall detected on RIGHT motor!"); 
          }
        }
      } else { stall_timer_start_r = 0; }
      
      set_motor_raw(last_pwm_left, last_pwm_right);
    } else {
      pid_reset(pid_left); pid_reset(pid_right);
      last_pwm_left = 0; last_pwm_right = 0;
      set_motor_raw(0, 0);
    }
  } else {
    last_pwm_left = 0; last_pwm_right = 0;
    set_motor_raw(0, 0);
    pid_reset(pid_left);
    pid_reset(pid_right);
  }

  if (imu_data_valid && imu_calibrated) {
    float gx = fabs(imu_gx) > GYRO_DEADBAND ? imu_gx : 0;
    float gy = fabs(imu_gy) > GYRO_DEADBAND ? imu_gy : 0;
    float gz = fabs(imu_gz) > GYRO_DEADBAND ? imu_gz : 0;
    float accel_roll = atan2(-imu_ay, -imu_az);
    float accel_pitch = atan2(imu_ax, sqrt(imu_ay * imu_ay + imu_az * imu_az));
    if (dt_valid) {
      cf_roll = COMP_GAIN * (cf_roll + gx * dt) + (1.0f - COMP_GAIN) * accel_roll;
      cf_pitch = COMP_GAIN * (cf_pitch + gy * dt) + (1.0f - COMP_GAIN) * accel_pitch;
      cf_yaw += gz * dt;
      cf_yaw = normalize_angle(cf_yaw);
    }
  }

  if (loop_count % 100 == 0) {
    print_encoder_raw();
    Serial.print(" PWM L:");
    Serial.print(last_pwm_left, 0);
    Serial.print(" R:");
    Serial.print(last_pwm_right, 0);
    Serial.print("  IMU ax:");
    Serial.print(imu_ax);
    Serial.print(" ay:");
    Serial.print(imu_ay);
    Serial.print(" az:");
    Serial.print(imu_az);
    Serial.print("  RPM tgtL:");
    Serial.print(target_rpm_left_filtered, 0);
    Serial.print(" actL:");
    Serial.print(actual_rpm_left, 0);
    Serial.print(" tgtR:");
    Serial.print(target_rpm_right_filtered, 0);
    Serial.print(" actR:");
    Serial.print(actual_rpm_right, 0);
    Serial.print("  odom x:");
    Serial.print(odom_x, 3);
    Serial.print(" y:");
    Serial.print(odom_y, 3);
    Serial.print(" theta:");
    Serial.println(odom_theta, 3);
  }
  loop_count++;
}
