#include <Wire.h>
#include <math.h>
#include "driver/pcnt.h"
#include "esp_timer.h"

#define PIN_L_ENC_A  27
#define PIN_L_ENC_B  23
#define PIN_R_ENC_A  14
#define PIN_R_ENC_B  13

#define I2C_SDA       21
#define I2C_SCL       22

#define MPU6050_ADDR 0x68
#define CALIB_SAMPLES 200
#define IMU_LP_ALPHA  0.10f
#define COMP_GAIN     0.85f
#define LOOP_INTERVAL_MS 20

float gbias_x = 0, gbias_y = 0, gbias_z = 0;
bool imu_calibrated = false;
bool imu_healthy = true;

float imu_ax = 0, imu_ay = 0, imu_az = 0;
float imu_gx = 0, imu_gy = 0, imu_gz = 0;

float cf_roll = 0, cf_pitch = 0, cf_yaw = 0;

bool write_reg(uint8_t reg, uint8_t val) {
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(reg); Wire.write(val);
  return Wire.endTransmission() == 0;
}

uint8_t read_reg(uint8_t reg) {
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(reg); Wire.endTransmission(false);
  if (Wire.requestFrom((uint8_t)MPU6050_ADDR, (uint8_t)1) < 1) return 0;
  return Wire.read();
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

  pcnt_set_filter_value(unit, 200);
  pcnt_filter_enable(unit);
  pcnt_counter_clear(unit);
}

long read_pcnt(pcnt_unit_t unit) {
  static int16_t last_val_0 = 0, last_val_1 = 0;
  int16_t count = 0;
  if (pcnt_get_counter_value(unit, &count) != ESP_OK) return 0;
  int16_t &last = (unit == PCNT_UNIT_0) ? last_val_0 : last_val_1;
  int32_t diff = (int32_t)count - (int32_t)last;
  if (diff > 32767) diff -= 65536;
  else if (diff < -32768) diff += 65536;
  last = count;
  return diff;
}

void setup_imu() {
  Wire.beginTransmission(MPU6050_ADDR);
  if (Wire.endTransmission() != 0) {
    Serial.println("[FAIL] MPU6050 not found");
    imu_healthy = false;
    return;
  }
  Serial.println("[OK] MPU6050 detected");

  write_reg(0x6B, 0x00); delay(50);
  write_reg(0x1A, 0x03);
  write_reg(0x1B, 0x08);
  for (int i = 0; i < 3; i++) { write_reg(0x1C, 0x08); delay(5); if (read_reg(0x1C) == 0x08) break; }
  uint8_t accel_config = read_reg(0x1C);
  Serial.printf("[OK] ACCEL_CONFIG = 0x%02X (expect 0x08)\n", accel_config);
}

bool calibrate_imu() {
  Serial.print("Calibrating gyro, keep still...");
  float sx = 0, sy = 0, sz = 0;
  int valid = 0;
  for (int i = 0; i < CALIB_SAMPLES; i++) {
    Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x3B); Wire.endTransmission(false);
    if (Wire.requestFrom((uint8_t)MPU6050_ADDR, (uint8_t)14) < 14) { delay(5); continue; }
    int16_t ax_raw = (Wire.read() << 8) | Wire.read();
    int16_t ay_raw = (Wire.read() << 8) | Wire.read();
    int16_t az_raw = (Wire.read() << 8) | Wire.read();
    Wire.read(); Wire.read();
    int16_t gx_raw = (Wire.read() << 8) | Wire.read();
    int16_t gy_raw = (Wire.read() << 8) | Wire.read();
    int16_t gz_raw = (Wire.read() << 8) | Wire.read();
    sx += gx_raw / 65.5f * (M_PI / 180.0f);
    sy += gy_raw / 65.5f * (M_PI / 180.0f);
    sz += gz_raw / 65.5f * (M_PI / 180.0f);
    valid++; delay(5);
  }
  if (valid == 0) { Serial.println(" FAILED"); return false; }
  gbias_x = sx / valid; gbias_y = sy / valid; gbias_z = sz / valid;
  imu_calibrated = true;
  Serial.printf(" done (bias: %.3f %.3f %.3f)\n", gbias_x, gbias_y, gbias_z);
  return true;
}

void read_imu() {
  if (!imu_healthy) return;
  Wire.beginTransmission(MPU6050_ADDR); Wire.write(0x3B); Wire.endTransmission(false);
  if (Wire.requestFrom((uint8_t)MPU6050_ADDR, (uint8_t)14) < 14) return;

  int16_t ax_raw = (Wire.read() << 8) | Wire.read();
  int16_t ay_raw = (Wire.read() << 8) | Wire.read();
  int16_t az_raw = (Wire.read() << 8) | Wire.read();
  Wire.read(); Wire.read();
  int16_t gx_raw = (Wire.read() << 8) | Wire.read();
  int16_t gy_raw = (Wire.read() << 8) | Wire.read();
  int16_t gz_raw = (Wire.read() << 8) | Wire.read();

  float rax = ax_raw / 8192.0f * 9.81f;
  float ray = ay_raw / 8192.0f * 9.81f;
  float raz = az_raw / 8192.0f * 9.81f;
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
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n===== ESP32 Board Test =====");

  pinMode(PIN_L_ENC_A, INPUT_PULLUP); pinMode(PIN_L_ENC_B, INPUT_PULLUP);
  pinMode(PIN_R_ENC_A, INPUT_PULLUP); pinMode(PIN_R_ENC_B, INPUT_PULLUP);
  setup_pcnt(PCNT_UNIT_0, PIN_L_ENC_A, PIN_L_ENC_B);
  setup_pcnt(PCNT_UNIT_1, PIN_R_ENC_A, PIN_R_ENC_B);
  read_pcnt(PCNT_UNIT_0); read_pcnt(PCNT_UNIT_1);
  Serial.println("[OK] Encoder PCNT initialized");

  Wire.begin(I2C_SDA, I2C_SCL, 400000);
  setup_imu();
  if (imu_healthy) {
    for (int i = 0; i < 10; i++) { read_imu(); delay(10); }
    calibrate_imu();
    for (int i = 0; i < 50; i++) { read_imu(); delay(5); }
  }

  // 互补滤波初始值 = 加速度计姿态
  float accel_roll = atan2(-imu_ay, -imu_az);
  if (accel_roll > M_PI / 2) accel_roll -= M_PI;
  else if (accel_roll < -M_PI / 2) accel_roll += M_PI;
  cf_roll = accel_roll;
  cf_pitch = atan2(imu_ax, sqrt(imu_ay * imu_ay + imu_az * imu_az));

  Serial.println("===== Start (20ms loop) =====\n");
  Serial.println("ENC_L  ENC_R | ACC_X  ACC_Y  ACC_Z (m/s2) | GYR_X  GYR_Y  GYR_Z (rad/s) | CF_ROLL  CF_PITCH  CF_YAW (deg)");
}

unsigned long last_ms = 0;
unsigned long loop_count = 0;

void loop() {
  unsigned long now = millis();
  if (now - last_ms < LOOP_INTERVAL_MS) return;
  float dt = (now - last_ms) / 1000.0f;
  last_ms = now;

  long enc_l = read_pcnt(PCNT_UNIT_0);
  long enc_r = read_pcnt(PCNT_UNIT_1);
  read_imu();

  if (imu_calibrated && imu_healthy) {
    float accel_roll = atan2(-imu_ay, -imu_az);
    if (accel_roll > M_PI / 2) accel_roll -= M_PI;
    else if (accel_roll < -M_PI / 2) accel_roll += M_PI;
    float accel_pitch = atan2(imu_ax, sqrt(imu_ay * imu_ay + imu_az * imu_az));
    cf_roll = COMP_GAIN * (cf_roll + imu_gx * dt) + (1.0f - COMP_GAIN) * accel_roll;
    cf_pitch = COMP_GAIN * (cf_pitch + imu_gy * dt) + (1.0f - COMP_GAIN) * accel_pitch;
    cf_yaw += imu_gz * dt;
  }

  if (loop_count % 5 == 0) {
    Serial.printf("%5ld  %5ld | %6.2f %6.2f %6.2f | %6.3f %6.3f %6.3f | %7.1f  %7.1f  %7.1f\n",
      enc_l, enc_r,
      imu_ax, imu_ay, imu_az,
      imu_gx, imu_gy, imu_gz,
      cf_roll * 180.0f / M_PI, cf_pitch * 180.0f / M_PI, cf_yaw * 180.0f / M_PI);
  }
  loop_count++;
}
