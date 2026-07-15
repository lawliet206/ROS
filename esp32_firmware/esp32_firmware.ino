#include <Wire.h>
#include <math.h>
#include "driver/pcnt.h" 
#include "esp_timer.h" 
#include "esp_task_wdt.h"
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>

// ============================================================
// 1. 硬件与物理参数
// ============================================================
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

#define LEDC_CH_L_PWM  0
#define LEDC_CH_R_PWM  1
#define LEDC_FREQ      20000
#define LEDC_RES       10

// 🔄 根据你的实际硬件更新参数
#define WHEEL_DIAMETER      0.085f      // 轮径 8.5cm
#define WHEEL_BASE          0.180f      // 轮距 18cm
#define GEAR_RATIO          10.0f       // 减速比 10:1
#define ENCODER_PPR         11.0f       // 编码器基础脉冲数 11PPR
#define ENCODER_TICKS_PER_REV (ENCODER_PPR * 4.0f) 
#define METERS_PER_TICK     (M_PI * WHEEL_DIAMETER / (ENCODER_TICKS_PER_REV * GEAR_RATIO))

// 🛠️ 方向修正宏 (如果前进时里程计倒退，或 PID 反转，把对应的 1 改成 -1)
#define ENC_LEFT_DIR       1
#define ENC_RIGHT_DIR      1

// --- 控制与安全参数 ---
#define MAX_RAMP_RPM_PER_SEC  800.0f  
#define RPM_FILTER_ALPHA      0.3f    
#define RPM_STOP_THRESHOLD    1.0f    
#define PID_INTEGRAL_LIMIT    500.0f  
#define MAX_CMD_RPM           800.0f  
#define MIN_START_PWM         150.0f  
#define PID_OUTPUT_LIMIT      900.0f  

#define LOOP_INTERVAL_US  10000
#define WATCHDOG_TIMEOUT  500

#define STALL_DETECT_PWM_THRESH  800.0f
#define STALL_DETECT_RPM_THRESH  5.0f
#define STALL_DETECT_TIME_MS     500

// ============================================================
// 2. ROS 节点与消息定义
// ============================================================
ros::NodeHandle nh;

geometry_msgs::Twist cmd_vel_msg;
nav_msgs::Odometry odom_msg;
sensor_msgs::Imu imu_msg;

ros::Publisher pub_odom("odom", &odom_msg);
ros::Publisher pub_imu("imu", &imu_msg);

// ============================================================
// 3. 全局变量与状态
// ============================================================
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

float imu_ax = 0, imu_ay = 0, imu_az = 0;
float imu_gx = 0, imu_gy = 0, imu_gz = 0;
bool  imu_data_valid = false; 
uint8_t imu_timeout_count = 0; 

// ============================================================
// 4. PCNT 硬件编码器初始化
// ============================================================
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

  pcnt_set_filter_value(unit, 200); 
  pcnt_filter_enable(unit);
  pcnt_counter_clear(unit);
}

// 🔴 修正：PCNT返回值判断错误（致命错误修复）
long read_pcnt_left() {
  static int16_t last_val = 0;
  int16_t count = 0;
  if (pcnt_get_counter_value(PCNT_UNIT_0, &count) != ESP_OK) return 0; // 修正：ESP_OK 替代 ESP32
  int32_t diff = (int32_t)count - (int32_t)last_val;
  if (diff > 32767) diff -= 65536;
  else if (diff < -32768) diff += 65536;
  last_val = count;
  return diff;
}

// 🔴 修正：PCNT返回值判断错误（致命错误修复）
long read_pcnt_right() {
  static int16_t last_val = 0;
  int16_t count = 0;
  if (pcnt_get_counter_value(PCNT_UNIT_1, &count) != ESP_OK) return 0; // 修正：ESP_OK 替代 ESP32
  int32_t diff = (int32_t)count - (int32_t)last_val;
  if (diff > 32767) diff -= 65536;
  else if (diff < -32768) diff += 65536;
  last_val = count;
  return diff;
}

// ============================================================
// 5. 电机控制与 PID
// ============================================================
void setup_motors() {
  pinMode(PIN_L_IN1, OUTPUT); pinMode(PIN_L_IN2, OUTPUT);
  pinMode(PIN_R_IN1, OUTPUT); pinMode(PIN_R_IN2, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  digitalWrite(PIN_STBY, HIGH); 
  ledcSetup(LEDC_CH_L_PWM, LEDC_FREQ, LEDC_RES);
  ledcSetup(LEDC_CH_R_PWM, LEDC_FREQ, LEDC_RES);
  ledcAttachPin(PIN_L_PWM, LEDC_CH_L_PWM);
  ledcAttachPin(PIN_R_PWM, LEDC_CH_R_PWM);
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
  if (left > 0) { digitalWrite(PIN_L_IN1, HIGH); digitalWrite(PIN_L_IN2, LOW); ledcWrite(LEDC_CH_L_PWM, left); } 
  else if (left < 0) { digitalWrite(PIN_L_IN1, LOW); digitalWrite(PIN_L_IN2, HIGH); ledcWrite(LEDC_CH_L_PWM, -left); } 
  else { digitalWrite(PIN_L_IN1, LOW); digitalWrite(PIN_L_IN2, LOW); ledcWrite(LEDC_CH_L_PWM, 0); }
  if (right > 0) { digitalWrite(PIN_R_IN1, HIGH); digitalWrite(PIN_R_IN2, LOW); ledcWrite(LEDC_CH_R_PWM, right); } 
  else if (right < 0) { digitalWrite(PIN_R_IN1, LOW); digitalWrite(PIN_R_IN2, HIGH); ledcWrite(LEDC_CH_R_PWM, -right); } 
  else { digitalWrite(PIN_R_IN1, LOW); digitalWrite(PIN_R_IN2, LOW); ledcWrite(LEDC_CH_R_PWM, 0); }
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

// ============================================================
// 6. MPU6050 配置与读取
// ============================================================
#define MPU6050_ADDR 0x68
void recover_i2c() {
  Wire.end(); 
  pinMode(I2C_SCL, OUTPUT_OPEN_DRAIN); pinMode(I2C_SDA, OUTPUT_OPEN_DRAIN);
  for (int i = 0; i < 9; i++) { digitalWrite(I2C_SCL, HIGH); delayMicroseconds(5); digitalWrite(I2C_SCL, LOW); delayMicroseconds(5); }
  digitalWrite(I2C_SDA, HIGH); delayMicroseconds(10);
  pinMode(I2C_SDA, INPUT_PULLUP); pinMode(I2C_SCL, INPUT_PULLUP);
}
void setup_imu() {
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x6B); Wire.write(0x00); Wire.endTransmission();
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x1A); Wire.write(0x03); Wire.endTransmission();
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x1B); Wire.write(0x08); Wire.endTransmission();
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x1C); Wire.write(0x08); Wire.endTransmission();
}
void read_imu() {
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x3B); Wire.endTransmission(false);
  Wire.setTimeOut(2000); 
  uint8_t bytes_received = Wire.requestFrom((uint8_t)MPU6050_ADDR, (uint8_t)14);
  if (bytes_received < 14) {
    imu_data_valid = false; imu_timeout_count++;
    if (imu_timeout_count >= 3) { recover_i2c(); Wire.begin(I2C_SDA, I2C_SCL, 400000); setup_imu(); imu_timeout_count = 0; }
    return; 
  }
  imu_timeout_count = 0; 
  int16_t ax_raw = (Wire.read() << 8) | Wire.read();
  int16_t ay_raw = (Wire.read() << 8) | Wire.read();
  int16_t az_raw = (Wire.read() << 8) | Wire.read();
  Wire.read(); Wire.read(); 
  int16_t gx_raw = (Wire.read() << 8) | Wire.read();
  int16_t gy_raw = (Wire.read() << 8) | Wire.read();
  int16_t gz_raw = (Wire.read() << 8) | Wire.read();
  imu_ax = ax_raw / 8192.0f * 9.81f; imu_ay = ay_raw / 8192.0f * 9.81f; imu_az = az_raw / 8192.0f * 9.81f;
  imu_gx = gx_raw / 65.5f * (M_PI / 180.0f); imu_gy = gy_raw / 65.5f * (M_PI / 180.0f); imu_gz = gz_raw / 65.5f * (M_PI / 180.0f);
  imu_data_valid = true; 
}

// ============================================================
// 7. ROS 回调函数 (订阅 cmd_vel)
// ============================================================
void cmdVelCallback(const geometry_msgs::Twist& msg) {
  float v = msg.linear.x;
  float w = msg.angular.z;
  
  float v_left = v - (w * WHEEL_BASE / 2.0);
  float v_right = v + (w * WHEEL_BASE / 2.0);
  
  target_rpm_left = (v_left / (M_PI * WHEEL_DIAMETER)) * 60.0;
  target_rpm_right = (v_right / (M_PI * WHEEL_DIAMETER)) * 60.0;
  
  target_rpm_left = constrain(target_rpm_left, -MAX_CMD_RPM, MAX_CMD_RPM);
  target_rpm_right = constrain(target_rpm_right, -MAX_CMD_RPM, MAX_CMD_RPM);
  
  last_cmd_time = millis();
  
  if (stall_fault_l) { stall_fault_l = false; stall_timer_start_l = 0; pid_reset(pid_left); }
  if (stall_fault_r) { stall_fault_r = false; stall_timer_start_r = 0; pid_reset(pid_right); }
}
ros::Subscriber<geometry_msgs::Twist> sub_cmd("cmd_vel", &cmdVelCallback);

// ============================================================
// 8. 初始化与主循环 
// ============================================================
void setup() {
  Serial.setRxBufferSize(1024);
  Serial.setTxBufferSize(1024);
  Serial.begin(460800);
  
  pinMode(PIN_L_ENC_A, INPUT_PULLUP); pinMode(PIN_L_ENC_B, INPUT_PULLUP);
  pinMode(PIN_R_ENC_A, INPUT_PULLUP); pinMode(PIN_R_ENC_B, INPUT_PULLUP);
  setup_pcnt(PCNT_UNIT_0, PIN_L_ENC_A, PIN_L_ENC_B);
  setup_pcnt(PCNT_UNIT_1, PIN_R_ENC_A, PIN_R_ENC_B);
  read_pcnt_left(); read_pcnt_right();
  
  setup_motors();
  Wire.begin(I2C_SDA, I2C_SCL, 400000);
  setup_imu();
  
  pid_left  = {1.5f, 0.3f, 0.05f, 0, 0, false};
  pid_right = {1.5f, 0.3f, 0.05f, 0, 0, false}; 
  
  // 硬件看门狗：主循环卡死 5 秒后自动重启 ESP32
  esp_task_wdt_init(5, true);
  esp_task_wdt_add(NULL);

  nh.initNode();
  nh.advertise(pub_odom);
  nh.advertise(pub_imu);
  nh.subscribe(sub_cmd);
  
  last_loop_us = esp_timer_get_time();
  prev_exec_us = last_loop_us;
}

void loop() {
  esp_task_wdt_reset();  // 喂硬件看门狗

  int64_t now = esp_timer_get_time();
  if (now - last_loop_us < LOOP_INTERVAL_US) {
    nh.spinOnce(); 
    delay(1); 
    return;
  }
  
  float dt = (now - prev_exec_us) / 1000000.0f;
  prev_exec_us = now;
  last_loop_us += LOOP_INTERVAL_US;
  if (now - last_loop_us > LOOP_INTERVAL_US * 2) last_loop_us = now; 
  bool dt_valid = (dt > 0.001f && dt < 0.05f); 

  read_imu();

  if (millis() - last_cmd_time > WATCHDOG_TIMEOUT) {
    target_rpm_left = 0;
    target_rpm_right = 0;
    target_rpm_left_filtered = 0;
    target_rpm_right_filtered = 0;
  }

  long delta_l_raw = read_pcnt_left();
  long delta_r_raw = read_pcnt_right();

  // 🛠️ 核心修复：统一应用方向修正宏
  float delta_l = delta_l_raw * ENC_LEFT_DIR;
  float delta_r = delta_r_raw * ENC_RIGHT_DIR;

  // 里程计积分计算
  float dist_left = delta_l * METERS_PER_TICK;
  float dist_right = delta_r * METERS_PER_TICK;
  float dist_center = (dist_left + dist_right) / 2.0;
  
  odom_theta += (dist_right - dist_left) / WHEEL_BASE;
  odom_x += dist_center * cos(odom_theta);
  odom_y += dist_center * sin(odom_theta);

  if (dt_valid) {
    // 转速计算
    float raw_rpm_left  = (delta_l / ENCODER_TICKS_PER_REV) * (60.0f / dt) / GEAR_RATIO;
    float raw_rpm_right = (delta_r / ENCODER_TICKS_PER_REV) * (60.0f / dt) / GEAR_RATIO;
    
    actual_rpm_left  = actual_rpm_left * (1.0f - RPM_FILTER_ALPHA) + raw_rpm_left * RPM_FILTER_ALPHA;
    actual_rpm_right = actual_rpm_right * (1.0f - RPM_FILTER_ALPHA) + raw_rpm_right * RPM_FILTER_ALPHA;

    float max_ramp = MAX_RAMP_RPM_PER_SEC * dt;
    target_rpm_left_filtered  += constrain(target_rpm_left  - target_rpm_left_filtered,  -max_ramp, max_ramp);
    target_rpm_right_filtered += constrain(target_rpm_right - target_rpm_right_filtered, -max_ramp, max_ramp);

    if (fabs(target_rpm_left_filtered) > 0.01f || fabs(target_rpm_right_filtered) > 0.01f || 
        fabs(actual_rpm_left) > RPM_STOP_THRESHOLD || fabs(actual_rpm_right) > RPM_STOP_THRESHOLD) {
      
      float pwm_left  = pid_compute(pid_left,  target_rpm_left_filtered,  actual_rpm_left,  dt);
      float pwm_right = pid_compute(pid_right, target_rpm_right_filtered, actual_rpm_right, dt);
      pwm_left = apply_deadzone_ff(pwm_left);
      pwm_right = apply_deadzone_ff(pwm_right);

      if (fabs(pwm_left) > STALL_DETECT_PWM_THRESH && fabs(actual_rpm_left) < STALL_DETECT_RPM_THRESH) {
        if (stall_timer_start_l == 0) stall_timer_start_l = millis();
        else if (millis() - stall_timer_start_l > STALL_DETECT_TIME_MS) {
          if (!stall_fault_l) { 
            stall_fault_l = true; 
            pid_reset(pid_left); 
            nh.logwarn("Stall detected on LEFT motor!"); 
          }
        }
      } else { stall_timer_start_l = 0; }
      
      if (fabs(pwm_right) > STALL_DETECT_PWM_THRESH && fabs(actual_rpm_right) < STALL_DETECT_RPM_THRESH) {
        if (stall_timer_start_r == 0) stall_timer_start_r = millis();
        else if (millis() - stall_timer_start_r > STALL_DETECT_TIME_MS) {
          if (!stall_fault_r) { 
            stall_fault_r = true; 
            pid_reset(pid_right); 
            nh.logwarn("Stall detected on RIGHT motor!"); 
          }
        }
      } else { stall_timer_start_r = 0; }
      
      set_motor_raw(pwm_left, pwm_right);
    } else {
      pid_reset(pid_left); pid_reset(pid_right);
      set_motor_raw(0, 0);
    }
  } else {
    set_motor_raw(0, 0);
  }

  float vx = 0, vth = 0;
  if (dt_valid) {
    vx = dist_center / dt;
    vth = (dist_right - dist_left) / (WHEEL_BASE * dt);
  }

  odom_msg.header.stamp = nh.now();
  odom_msg.header.frame_id = "odom";
  odom_msg.child_frame_id = "base_footprint";
  odom_msg.pose.pose.position.x = odom_x;
  odom_msg.pose.pose.position.y = odom_y;
  odom_msg.pose.pose.position.z = 0.0;
  odom_msg.pose.pose.orientation.z = sin(odom_theta / 2.0);
  odom_msg.pose.pose.orientation.w = cos(odom_theta / 2.0);
  odom_msg.twist.twist.linear.x = vx;
  odom_msg.twist.twist.angular.z = vth;
  for(int i=0; i<36; i++) odom_msg.pose.covariance[i] = 0.0;
  odom_msg.pose.covariance[0] = 0.001; 
  odom_msg.pose.covariance[7] = 0.001; 
  odom_msg.pose.covariance[35] = 0.001; 
  for(int i=0; i<36; i++) odom_msg.twist.covariance[i] = 0.0;
  odom_msg.twist.covariance[0] = 0.001;
  odom_msg.twist.covariance[35] = 0.001;

  // 🟡 优化：修正IMU协方差矩阵（仅对角线设方差，非对角线清零）
  if (imu_data_valid) {
    imu_msg.header.stamp = nh.now();
    imu_msg.header.frame_id = "imu_link";
    imu_msg.linear_acceleration.x = imu_ax;
    imu_msg.linear_acceleration.y = imu_ay;
    imu_msg.linear_acceleration.z = imu_az;
    imu_msg.angular_velocity.x = imu_gx;
    imu_msg.angular_velocity.y = imu_gy;
    imu_msg.angular_velocity.z = imu_gz;
    // 加速度协方差：仅对角线设0.01，非对角线清零
    for (int i = 0; i < 9; i++) imu_msg.linear_acceleration_covariance[i] = 0.0;
    imu_msg.linear_acceleration_covariance[0] = 0.01; // x方差
    imu_msg.linear_acceleration_covariance[4] = 0.01; // y方差
    imu_msg.linear_acceleration_covariance[8] = 0.01; // z方差
    // 角速度协方差：仅对角线设0.01，非对角线清零
    for (int i = 0; i < 9; i++) imu_msg.angular_velocity_covariance[i] = 0.0;
    imu_msg.angular_velocity_covariance[0] = 0.01; // x方差
    imu_msg.angular_velocity_covariance[4] = 0.01; // y方差
    imu_msg.angular_velocity_covariance[8] = 0.01; // z方差
    // 姿态协方差设为-1（表示不提供姿态）
    for(int i=0; i<9; i++) imu_msg.orientation_covariance[i] = -1.0; 
  }

  if (loop_count % 2 == 0) {
    pub_odom.publish(&odom_msg);
    if (imu_data_valid) pub_imu.publish(&imu_msg);
  }
  loop_count++;

  nh.spinOnce();
}