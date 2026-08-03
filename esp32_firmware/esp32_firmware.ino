#include <Wire.h>
#include <math.h>
#include "driver/pcnt.h" 
#include "esp_timer.h" 
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>


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

#define LEDC_FREQ      20000                   // [可调] PWM 频率 (Hz). TB6612 支持到 100kHz. 20kHz 在听觉上限, 降低会听到电机啸叫
#define LEDC_RES       10                      // [可调] PWM 分辨率. 10bit=0~1023. 改大会降低频率, 改小会降低控制精度

// ========== 机械参数 (换轮子/换电机/改轮距时必须改) ==========
#define WHEEL_DIAMETER      0.085f             // [实车校准] 轮径 (m). 用卷尺实测, 橡胶轮随气压和磨损变化
#define WHEEL_BASE          0.180f             // [实车校准] 轮距 (m). 左右轮着地点中心距, 不准则转弯半径和里程计航向都偏
#define GEAR_RATIO          10.0f              // [硬件参数] 减速比. JGB37-520 常见 1:10, 1:19, 1:30. 编码器在电机轴上, 转速需除以此值得到轮子转速
#define ENCODER_PPR         11.0f              // [硬件参数] 编码器线数 (脉冲/转). JGB37-520 AB 相霍尔每转 11 个脉冲
#define ENCODER_TICKS_PER_REV (ENCODER_PPR * 4.0f)  // 四倍频后每转 tick 数 (11×4=44). 不改
#define METERS_PER_TICK     (M_PI * WHEEL_DIAMETER / (ENCODER_TICKS_PER_REV * GEAR_RATIO))  // 每个编码器 tick 对应的轮子行走距离 (m). 不改

// ========== 编码器方向 (实车必须验证) ==========
// 编码器方向系数 — 实车验证方法：
//   1. 先设两侧均为 1，手动推车前进 1 米
//   2. serial_bridge 打印 /odom position.x，若为正值则前进方向正确
//   3. 若 position.x 为负值，将两轮系数互换符号（如 1→-1, -1→1）
//   4. 再验证转向：原地旋转 90°，/odom orientation.z 应正确累积
//   5. 最后验证左右轮差速方向是否一致，若两轮转速相反则取反一侧
#define ENC_LEFT_DIR       1                  // [实车校准] 左轮编码器方向. 1=正向, -1=反向. 推车前进看 /odom position.x 正负
#define ENC_RIGHT_DIR      -1                 // [实车校准] 右轮编码器方向. 注意右轮电机是镜像安装, 通常需要取反

// ========== PID 控制参数 ==========
// PID 输出 = Kp*error + Ki*∫error*dt + Kd*d(measurement)/dt
// 微分项算在测量值上 (不是误差), 避免设定值突变时产生过冲
// 调参顺序: 先只调 Kp 让车轮跟得上设定转速, 再加 Ki 消除稳态误差, 最后加少量 Kd 抑制震荡
#define MAX_RAMP_RPM_PER_SEC  100.0f          // [可调] RPM 斜坡限制 (RPM/s). 降低到 100 使起步更平缓安全
#define RPM_FILTER_ALPHA      0.3f            // [可调] 转速低通滤波 α (0~1). 越小越平滑但响应越慢. 0.3 是默认. 高速场景需要更快响应时可加大到 0.5, 但 RPM 读数会更抖
#define RPM_STOP_THRESHOLD    1.0f            // [可调] 停止判定 RPM 阈值. 低于此值认为电机已停, PID 复位. 设太大电机会一直微动, 太小则停下来后还有残余 PWM
#define PID_INTEGRAL_LIMIT    2000.0f         // [可调] PID 积分上限. 限制积分项不会无限累积 (抗积分饱和). 过大 → 积分失控, 过小 → 稳态误差清不掉
#define MAX_CMD_RPM           200.0f          // [可调] 最大允许 RPM. 200 RPM ≈ 0.89 m/s, 安全测试速度
#define MIN_START_PWM         150.0f          // [可调] 启动死区 PWM (0~1023). TB6612 在极低 PWM 时电机不转, 需要最小值克服静摩擦力. 太小起步抖, 太大起步冲. sqrt 平滑过渡避免突然窜出
#define PID_OUTPUT_LIMIT      1023.0f         // [可调] PID 输出上限. 10bit PWM 最大 1023. 小于此值可限制最大功率, 保护电机或省电

// ========== 时序与安全 ==========
#define LOOP_INTERVAL_US  10000                 // [可调] 主循环间隔 (μs). 10000=10ms=100Hz. 改小提高控制频率但增加 CPU 负载. 改大降低频率但编码器采样更稀疏, 高速下 RPM 估算不准
#define WATCHDOG_TIMEOUT  800                   // [可调] 看门狗超时 (ms). 超过此时间没收到 /cmd_vel 则自动停车拉低 STBY. 设太小正常行驶中突然无指令会急停, 太大失控后要很久才自动停

// ========== 堵转检测 ==========
// 判定条件: 命令转速 ≥ STALL_DETECT_CMD_THRESH 但实际转速 < 1 RPM, 持续超过 STALL_DETECT_TIME_MS
// 不用 PWM 阈值判断: 堵转 5s 时 PID 输出仅 ~600 (Kp*200 + Ki*200*5), 积分 10s 才饱和到 900,
// 固定 PWM 阈值 (如 800/1000) 无法在触发窗口内命中. 目标/实际转速差才是堵转的本质特征.
#define STALL_DETECT_CMD_THRESH  80.0f         // [可调] 堵转判定命令转速阈值 (RPM). 命令 ≥80 RPM 但实际 <1 → 堵转
#define STALL_DETECT_RPM_THRESH  1.0f           // [可调] 堵转判定 RPM 阈值. RPM 降到 1 以下才算堵转
#define STALL_DETECT_TIME_MS     2000           // [可调] 堵转确认时间 (ms). 持续 2 秒真堵转才停, 过坎不会误判 (实车调参)

ros::NodeHandle nh;

nav_msgs::Odometry odom_msg;
sensor_msgs::Imu imu_msg;

ros::Publisher pub_odom("odom", &odom_msg);
ros::Publisher pub_imu("imu", &imu_msg);

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

#define MPU6050_ADDR 0x68
// ========== IMU (MPU6050) 参数 ==========
// IMU 低通滤波 α (0~1). 越小越平滑但延迟越大. 0.3 是平衡值
// EKF 用 IMU 数据时此值影响融合质量: 太大=噪声多, 太小=延迟导致相位差
#define IMU_LP_ALPHA  0.30f                    // [可调] IMU 低通滤波系数
#define CALIB_SAMPLES 200                      // [可调] 陀螺仪校准样本数. 越多越准但启动越慢. 200≈1秒

float gbias_x = 0, gbias_y = 0, gbias_z = 0;
float abias_x = 0, abias_y = 0, abias_z = 0;
bool imu_calibrated = false;
bool imu_data_valid = false; 

float imu_ax = 0, imu_ay = 0, imu_az = 0;
float imu_gx = 0, imu_gy = 0, imu_gz = 0;
float accel_scale = 1.0f;
bool imu_healthy = true;

// 互补滤波的陀螺仪权重 (0~1). 0.85=85%信陀螺仪, 15%信加速度计
// 设太接近 1: roll/pitch 长期漂移; 太接近 0: 振动敏感的 roll/pitch 乱跳
#define COMP_GAIN      0.85f                   // [可调] 互补滤波陀螺仪权重
#define GYRO_DEADBAND  0.002f                  // [可调] 陀螺仪死区 (rad/s). 低于此值视为零, 避免静止时角度漂移. 太小=漂移, 太大=慢转被忽略
// 注意: cf_yaw 是纯陀螺仪积分, 没有磁力计修正, 长期会漂移 (几度/分钟)
// EKF 融合时建议只用 IMU 的角速度和加速度, 不要用 IMU 的 orientation 做偏航参考
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
  // 左轮: forward (left>0) → IN1=LOW, IN2=HIGH
  if (left > 0) { digitalWrite(PIN_L_IN1, LOW); digitalWrite(PIN_L_IN2, HIGH); ledcWrite(PIN_L_PWM, left); } 
  else if (left < 0) { digitalWrite(PIN_L_IN1, HIGH); digitalWrite(PIN_L_IN2, LOW); ledcWrite(PIN_L_PWM, -left); } 
  else { digitalWrite(PIN_L_IN1, LOW); digitalWrite(PIN_L_IN2, LOW); ledcWrite(PIN_L_PWM, 0); }
  // 右轮: forward (right>0) → IN1=LOW, IN2=HIGH (与左轮相同, commit d3aec66 已交换接线)
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

// ========== I2C 总线恢复 ==========
// 当 I2C 连续失败超过阈值时, 复位 I2C 总线并重新初始化 IMU
// 注意: 恢复期间 (约 50ms) 电机会被强制停止, 导航中触发会导致短暂刹车
int i2c_fail_count = 0;
void recover_i2c() {                         // [可调] I2C 失败阈值在 read_imu() 中: i2c_fail_count > 20 (约 200ms 连续失败). 设太小=误触发, 太大=挂了很久才恢复
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
  nh.logwarn("I2C bus recovered, IMU reinitialized");
}

void setup_imu() {
  Wire.setTimeOut(20);                       // [可调] I2C 超时 (ms). 太短 I2C 经常超时失败, 太长会卡主循环
  Wire.beginTransmission(MPU6050_ADDR);
  if (Wire.endTransmission() != 0) {
    imu_healthy = false;
    return;
  }
  imu_healthy = true;
  // MPU6050 寄存器配置 — 不改
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x6B); Wire.write(0x00); Wire.endTransmission();  // 退出睡眠模式
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x1A); Wire.write(0x03); Wire.endTransmission();  // DLPF=3 (44Hz 带宽, 平衡噪声和延迟)
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x1B); Wire.write(0x08); Wire.endTransmission();  // [可调] 陀螺仪量程. 0x08=±500°/s (灵敏度 65.5 LSB/°/s). 改 0x00=±250, 0x10=±1000, 0x18=±2000. 改量程需同步改校准代码中 65.5f 除数和 imu_gx 换算
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x1C); Wire.write(0x08); Wire.endTransmission();  // [可调] 加速度计量程. 0x08=±4g (灵敏度 8192 LSB/g). 改 0x00=±2g, 0x10=±8g, 0x18=±16g. 改量程需同步改 calibrate_imu 和 read_imu 中的 8192.0f 除数
}

void calibrate_imu() {
  if (!imu_healthy) return;
  nh.logwarn("Calibrating IMU, keep robot still...");
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
    nh.loginfo("Gyro bias calibration done.");

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
      imu_calibrated = true;   // 加速度计校准成功才标记已校准 (陀螺仪零偏已单独校准)
      nh.loginfo("Accel calibration done.");
    } else {
      nh.logwarn("Accel calibration skipped (noisy).");
    }

    read_imu();
    if (imu_data_valid) {
      cf_roll  = atan2(-imu_ay,  imu_az);
      cf_pitch = atan2(imu_ax, sqrt(imu_ay * imu_ay + imu_az * imu_az));
    }
  } else {
    nh.logerror("IMU Calibration failed! IMU disabled.");
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

void cmdVelCallback(const geometry_msgs::Twist& msg) {
  digitalWrite(PIN_STBY, HIGH);
  float v = msg.linear.x;
  float w = msg.angular.z;
  
  float v_left = v - (w * WHEEL_BASE / 2.0);
  float v_right = v + (w * WHEEL_BASE / 2.0);
  
  target_rpm_left = (v_left / (M_PI * WHEEL_DIAMETER)) * 60.0;
  target_rpm_right = (v_right / (M_PI * WHEEL_DIAMETER)) * 60.0;
  
  target_rpm_left = constrain(target_rpm_left, -MAX_CMD_RPM, MAX_CMD_RPM);
  target_rpm_right = constrain(target_rpm_right, -MAX_CMD_RPM, MAX_CMD_RPM);
  
  last_cmd_time = millis();
  
  if (stall_fault_l && fabs(target_rpm_left) < 1.0f) { stall_fault_l = false; stall_timer_start_l = 0; pid_reset(pid_left); }
  if (stall_fault_r && fabs(target_rpm_right) < 1.0f) { stall_fault_r = false; stall_timer_start_r = 0; pid_reset(pid_right); }
}
ros::Subscriber<geometry_msgs::Twist> sub_cmd("cmd_vel", &cmdVelCallback);

void setup() {
  // ========== 通信参数 ==========
  Serial.setRxBufferSize(1024);              // [可调] 串口接收缓冲区 (bytes). 460800 波特率下 1024 字节够用. 如果丢包/粘包频繁可以加大, 但 ESP32 内存有限
  Serial.setTxBufferSize(1024);              // [可调] 串口发送缓冲区 (bytes). 同上
  Serial.begin(460800);                      // [可调] 波特率. 460800 为默认. 降低可提高稳定性, 但数据吞吐量下降
  
  pinMode(PIN_L_ENC_A, INPUT_PULLUP); pinMode(PIN_L_ENC_B, INPUT_PULLUP);
  pinMode(PIN_R_ENC_A, INPUT_PULLUP); pinMode(PIN_R_ENC_B, INPUT_PULLUP);
  setup_pcnt(PCNT_UNIT_0, PIN_L_ENC_A, PIN_L_ENC_B);
  setup_pcnt(PCNT_UNIT_1, PIN_R_ENC_A, PIN_R_ENC_B);
  read_pcnt_left(); read_pcnt_right();
  
  setup_motors();
  Wire.begin(I2C_SDA, I2C_SCL, 400000);
  setup_imu();
  for (int i = 0; i < 10; i++) { read_imu(); delay(10); }
  
  // ========== PID 增益 (左右独立可调) ==========
  // 调参顺序: 先只加 Kp 让轮子跟得上设定转速, 再加 Ki 消除稳态误差 (低速/重载时重要), 最后加少量 Kd 抑制振荡
  // {Kp, Ki, Kd, integral, prev_measurement, initialized}
  pid_left  = {1.5f, 0.3f, 0.05f, 0, 0, false};   // [可调] 左轮 PID. Kp=比例, Ki=积分, Kd=微分. 左右通常一致, 但如果两侧电机/传动有差异可独立调
  pid_right = {1.5f, 0.3f, 0.05f, 0, 0, false};   // [可调] 右轮 PID. 调大 Kp: 跟得更紧但可能振荡. 调大 Ki: 消除稳态误差但可能过冲. 调大 Kd: 抑制振荡但可能放大噪声

  // 关键: 必须显式设置 rosserial 波特率!
  // ros_lib 的 ArduinoHardware 构造函数默认波特率是 57600 (ros_lib/ArduinoHardware.h 的默认参数),
  // 若不设置, 下面的 nh.initNode() 会调用 Serial.begin(57600) 覆盖上方 Serial.begin(460800),
  // 导致 J1900 以 _baud:=460800 连接失败.
  nh.getHardware()->setBaud(460800);
  nh.initNode();
  nh.advertise(pub_odom);
  nh.advertise(pub_imu);
  nh.subscribe(sub_cmd);
  
  delay(500);
  calibrate_imu();
  for (int i = 0; i < 50; i++) { read_imu(); delay(5); }
  
  last_loop_us = esp_timer_get_time();
  prev_exec_us = last_loop_us;
}

void loop() {
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
      
      float pwm_left = 0, pwm_right = 0;
      if (!stall_fault_l) {
        pwm_left = pid_compute(pid_left, target_rpm_left_filtered, actual_rpm_left, dt);
        pwm_left = apply_deadzone_ff(pwm_left);
      }
      if (!stall_fault_r) {
        pwm_right = pid_compute(pid_right, target_rpm_right_filtered, actual_rpm_right, dt);
        pwm_right = apply_deadzone_ff(pwm_right);
      }

      if (fabs(target_rpm_left_filtered) > STALL_DETECT_CMD_THRESH
          && fabs(actual_rpm_left) < STALL_DETECT_RPM_THRESH) {
        if (stall_timer_start_l == 0) stall_timer_start_l = millis();
        else if (millis() - stall_timer_start_l > STALL_DETECT_TIME_MS) {
          if (!stall_fault_l) { 
            stall_fault_l = true; 
            pid_reset(pid_left); 
            nh.logwarn("Stall detected on LEFT motor!"); 
          }
        }
      } else { stall_timer_start_l = 0; }
      
      if (fabs(target_rpm_right_filtered) > STALL_DETECT_CMD_THRESH
          && fabs(actual_rpm_right) < STALL_DETECT_RPM_THRESH) {
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
    pid_reset(pid_left);
    pid_reset(pid_right);
  }

  if (imu_data_valid && imu_calibrated) {
    float gx = fabs(imu_gx) > GYRO_DEADBAND ? imu_gx : 0;
    float gy = fabs(imu_gy) > GYRO_DEADBAND ? imu_gy : 0;
    float gz = fabs(imu_gz) > GYRO_DEADBAND ? imu_gz : 0;
    float accel_roll  = atan2(-imu_ay,  imu_az);                                  // Roll: 绕 X=右轴, YZ 平面
    float accel_pitch = atan2( imu_ax, sqrt(imu_ay * imu_ay + imu_az * imu_az));  // Pitch: 绕 Y=前进轴, XZ 平面
    if (dt_valid) {
      cf_roll = COMP_GAIN * (cf_roll + gx * dt) + (1.0f - COMP_GAIN) * accel_roll;
      cf_pitch = COMP_GAIN * (cf_pitch + gy * dt) + (1.0f - COMP_GAIN) * accel_pitch;
      cf_yaw += gz * dt;
      cf_yaw = normalize_angle(cf_yaw);
    }
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
  // ========== 里程计协方差 ==========
  // 协方差矩阵告诉 ROS 这条数据的可信度. 值越小=越可信, 值越大=越不靠谱
  // [0]=(x,x), [7]=(y,y), [35]=(yaw,yaw)  — 6x6 矩阵对角线
  for(int i=0; i<36; i++) odom_msg.pose.covariance[i] = 0.0;
  odom_msg.pose.covariance[0] = 0.001;       // [可调] 位置 x 方差 (m²). 增大表示不相信编码器的位置估计, EKF 会更依赖激光/IMU 做定位
  odom_msg.pose.covariance[7] = 0.001;       // [可调] 位置 y 方差
  odom_msg.pose.covariance[35] = 0.001;      // [可调] 偏航角方差 (rad²). 打滑严重时增大此值, 让 EKF 少信里程计航向
  for(int i=0; i<36; i++) odom_msg.twist.covariance[i] = 0.0;
  odom_msg.twist.covariance[0] = 0.001;      // [可调] 线速度 x 方差
  odom_msg.twist.covariance[35] = 0.001;     // [可调] 角速度方差

  if (imu_data_valid) {
    imu_msg.header.stamp = nh.now();
    imu_msg.header.frame_id = "base_link";

    // 旋转 IMU 数据到 base_link 帧 (R_z(-π/2): imu→base)
    // imu_link: X=右, Y=前进, Z=上  →  base_link: X=前进, Y=左, Z=上
    // 变换矩阵: x_base = +y_imu, y_base = -x_imu (与 URDF imu_joint rpy="0 0 -1.5708" 一致, 实车确认 Y 朝车头)
    imu_msg.linear_acceleration.x =  imu_ay;   // 前进 = imu Y
    imu_msg.linear_acceleration.y = -imu_ax;   // 左 = -imu X (右→左)
    imu_msg.linear_acceleration.z =  imu_az;
    imu_msg.angular_velocity.x =  imu_gy;
    imu_msg.angular_velocity.y = -imu_gx;
    imu_msg.angular_velocity.z =  imu_gz;

    // base_link 系姿态角: 互补滤波角按 imu 轴定义, 映射到 base 轴需互换 roll/pitch 且 pitch 取反:
    //   base roll  (绕车头轴 X) =  cf_pitch (绕 imu Y=车头轴, 与 ωx=+imu_gy 一致)
    //   base pitch (绕左右轴 Y) = -cf_roll  (绕 imu X=左右轴, 与 ωy=-imu_gx 一致)
    //   base yaw   (绕上轴   Z) =  cf_yaw   (绕 imu Z=上轴, 同向且零位对齐, 无需旋转)
    float half_roll  =  cf_pitch * 0.5f;
    float half_pitch = -cf_roll * 0.5f;
    float half_yaw   =  cf_yaw * 0.5f;
    float cr = cos(half_roll), sr = sin(half_roll);
    float cp = cos(half_pitch), sp = sin(half_pitch);
    float cy = cos(half_yaw), sy = sin(half_yaw);
    imu_msg.orientation.x = sr * cp * cy - cr * sp * sy;
    imu_msg.orientation.y = cr * sp * cy + sr * cp * sy;
    imu_msg.orientation.z = cr * cp * sy - sr * sp * cy;
    imu_msg.orientation.w = cr * cp * cy + sr * sp * sy;

    // ========== IMU 协方差 ==========
    // [0]=(ax,ax), [4]=(ay,ay), [8]=(az,az)  — 3x3 矩阵对角线
    for (int i = 0; i < 9; i++) imu_msg.linear_acceleration_covariance[i] = 0.0;
    imu_msg.linear_acceleration_covariance[0] = 0.01;   // [可调] 加速度 x 方差 (m²/s⁴). 振动大时增大
    imu_msg.linear_acceleration_covariance[4] = 0.01;   // [可调] 加速度 y 方差
    imu_msg.linear_acceleration_covariance[8] = 0.01;   // [可调] 加速度 z 方差
    for (int i = 0; i < 9; i++) imu_msg.angular_velocity_covariance[i] = 0.0;
    imu_msg.angular_velocity_covariance[0] = 0.01;      // [可调] 角速度 x 方差 (rad²/s²)
    imu_msg.angular_velocity_covariance[4] = 0.01;      // [可调] 角速度 y 方差
    imu_msg.angular_velocity_covariance[8] = 0.01;      // [可调] 角速度 z 方差
    for (int i = 0; i < 9; i++) imu_msg.orientation_covariance[i] = 0.0;
    imu_msg.orientation_covariance[0] = 0.05;           // [可调] roll 方差 (rad²)
    imu_msg.orientation_covariance[4] = 0.05;           // [可调] pitch 方差
    imu_msg.orientation_covariance[8] = 100.0;          // [重要] yaw 方差=100, 表示偏航角完全不可信. 因为 cf_yaw 是纯陀螺仪积分没有磁力计修正, EKF 不应使用此值做偏航参考
  }

  if (loop_count % 2 == 0) {
    pub_odom.publish(&odom_msg);
    if (imu_data_valid) pub_imu.publish(&imu_msg);
  }
  loop_count++;

  nh.spinOnce();
}