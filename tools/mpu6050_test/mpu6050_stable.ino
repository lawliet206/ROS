#include <Wire.h>

#define MPU6050_ADDR 0x68

// 寄存器
#define WHO_AM_I      0x75
#define PWR_MGMT_1    0x6B
#define ACCEL_XOUT_H  0x3B
#define GYRO_XOUT_H   0x43

// 滤波参数
#define LP_ALPHA       0.10f     // 低通系数（越小越平滑）
#define CALIB_SAMPLES  200       // 零偏校准采样数

// 校准后零偏
float gbias_x = 0, gbias_y = 0, gbias_z = 0;
bool calibrated = false;

// 滤波后值
float fax = 0, fay = 0, faz = 0;
float fgx = 0, fgy = 0, fgz = 0;
float froll = 0, fpitch = 0;

// ============================================================
// I2C 读多字节辅助函数
// ============================================================
bool i2c_read_bytes(uint8_t reg, uint8_t *buf, uint8_t len) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(MPU6050_ADDR, len) < len) return false;
  for (uint8_t i = 0; i < len; i++) buf[i] = Wire.read();
  return true;
}

int16_t i2c_read16(uint8_t reg) {
  uint8_t buf[2];
  if (!i2c_read_bytes(reg, buf, 2)) return 0;
  return (buf[0] << 8) | buf[1];
}

// ============================================================
// 1. 初始化
// ============================================================
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  delay(100);

  // 1.1 检查 I2C 设备
  Wire.beginTransmission(MPU6050_ADDR);
  byte error = Wire.endTransmission();
  if (error != 0) {
    Serial.println("❌ I2C 总线上未找到 MPU6050！请检查接线。");
    Serial.print("错误码: "); Serial.println(error);
    while (1);
  }
  Serial.println("✅ MPU6050 已连接 (0x68)");

  // 1.2 校验芯片 ID
  byte whoami = i2c_read16(WHO_AM_I) & 0xFF;
  if (whoami == 0x68) {
    Serial.println("✅ 芯片 ID 正确 (0x68)");
  } else {
    Serial.printf("⚠️  芯片 ID 异常 (期望 0x68, 读到 0x%02X)\n", whoami);
  }

  // 1.3 唤醒
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00);
  Wire.endTransmission();

  // 1.4 配置量程
  // 陀螺仪 ±500°/s (0x08 → FS_SEL=1)
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1B);
  Wire.write(0x08);
  Wire.endTransmission();
  // 加速度计 ±4g (0x08 → AFS_SEL=1)
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1C);
  Wire.write(0x08);
  Wire.endTransmission();
  // DLPF 44Hz (0x1A → 0x03)
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1A);
  Wire.write(0x03);
  Wire.endTransmission();

  delay(100);
  Serial.println("\n🔧 正在校准陀螺仪零偏，请保持传感器静止...");

  // 1.5 陀螺仪零偏校准
  float sx = 0, sy = 0, sz = 0;
  int valid = 0;
  for (int i = 0; i < CALIB_SAMPLES; i++) {
    int16_t gx = i2c_read16(GYRO_XOUT_H);
    int16_t gy = i2c_read16(GYRO_XOUT_H + 2);
    int16_t gz = i2c_read16(GYRO_XOUT_H + 4);
    // 转换为 rad/s
    sx += gx / 65.5f * (PI / 180.0f);
    sy += gy / 65.5f * (PI / 180.0f);
    sz += gz / 65.5f * (PI / 180.0f);
    valid++;
    delay(5);
  }
  gbias_x = sx / valid;
  gbias_y = sy / valid;
  gbias_z = sz / valid;
  calibrated = true;

  Serial.printf("校准完成。零偏: gx=%.4f  gy=%.4f  gz=%.4f (rad/s)\n",
                gbias_x, gbias_y, gbias_z);
  Serial.println("\n========== 开始输出 ==========");
  Serial.println("每行两列：左侧原始值 | 右侧滤波后值");
  Serial.println("格式: AX_RAW  AY_RAW  AZ_RAW | AX_FILT  AY_FILT  AZ_FILT");
  Serial.println("       GX_RAW  GY_RAW  GZ_RAW | GX_FILT  GY_FILT  GZ_FILT");
  Serial.println("       ROLL    PITCH          | FROLL    FPITCH");
  Serial.println();
}

// ============================================================
// 2. 主循环
// ============================================================
void loop() {
  // 2.1 读取原始数据
  int16_t ax_raw = i2c_read16(ACCEL_XOUT_H);
  int16_t ay_raw = i2c_read16(ACCEL_XOUT_H + 2);
  int16_t az_raw = i2c_read16(ACCEL_XOUT_H + 4);
  int16_t gx_raw = i2c_read16(GYRO_XOUT_H);
  int16_t gy_raw = i2c_read16(GYRO_XOUT_H + 2);
  int16_t gz_raw = i2c_read16(GYRO_XOUT_H + 4);

  // 转换为物理单位
  // 加速度 ±4g → 8192 LSB/g → m/s²
  float rax = ax_raw / 8192.0f * 9.81f;
  float ray = ay_raw / 8192.0f * 9.81f;
  float raz = az_raw / 8192.0f * 9.81f;
  // 陀螺仪 ±500°/s → 65.5 LSB/(°/s) → rad/s
  float rgx = gx_raw / 65.5f * (PI / 180.0f);
  float rgy = gy_raw / 65.5f * (PI / 180.0f);
  float rgz = gz_raw / 65.5f * (PI / 180.0f);

  // 2.2 低通 IIR 滤波 + 零偏补偿
  if (!calibrated) {
    fax = rax; fay = ray; faz = raz;
    fgx = rgx; fgy = rgy; fgz = rgz;
  } else {
    fax = LP_ALPHA * rax + (1.0f - LP_ALPHA) * fax;
    fay = LP_ALPHA * ray + (1.0f - LP_ALPHA) * fay;
    faz = LP_ALPHA * raz + (1.0f - LP_ALPHA) * faz;
    fgx = LP_ALPHA * (rgx - gbias_x) + (1.0f - LP_ALPHA) * fgx;
    fgy = LP_ALPHA * (rgy - gbias_y) + (1.0f - LP_ALPHA) * fgy;
    fgz = LP_ALPHA * (rgz - gbias_z) + (1.0f - LP_ALPHA) * fgz;
  }

  // 2.3 从加速度计估计 roll/pitch
  froll = atan2(-fay, -faz) * 180.0f / PI;
  fpitch = atan2(fax, sqrt(fay * fay + faz * faz)) * 180.0f / PI;

  // 2.4 打印（原始 | 滤波后）
  // 加速度
  Serial.printf("ACC: %8.2f %8.2f %8.2f | %8.2f %8.2f %8.2f  (m/s²)\n",
                rax, ray, raz, fax, fay, faz);
  // 角速度
  Serial.printf("GYR: %8.2f %8.2f %8.2f | %8.2f %8.2f %8.2f  (rad/s)\n",
                rgx, rgy, rgz, fgx, fgy, fgz);
  // 姿态
  Serial.printf("ATT: %8.2f %8.2f       | %8.2f %8.2f  (deg)\n",
                atan2(-ray, -raz) * 180.0f / PI,
                atan2(rax, sqrt(ray * ray + raz * raz)) * 180.0f / PI,
                froll, fpitch);
  Serial.println("----------------------------------------");

  delay(200);
}
