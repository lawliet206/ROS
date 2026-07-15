#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
serial_bridge.py — J1900 串口桥接节点
======================================
运行位置: J1900 (车载)
功能:
  1. 通过 USB 串口与 ESP32 通信 (读取编码器/IMU/电池, 发送电机指令)
  2. 根据编码器+IMU实时计算里程计 (odometry)
  3. 发布 /odom 和 /imu 话题
  4. 订阅 /cmd_vel, 转换为轮速(RPM)后下发给 ESP32

使用方式:
  rosrun robot_bringup serial_bridge.py _port:=/dev/ttyUSB0

参数:
  _port        串口设备路径 (默认 /dev/ttyUSB0)
  _baud        波特率 (默认 115200)
  _wheel_dia   轮径 米 (默认 0.085)
  _wheel_base  轮距 米 (默认 0.135)
  _gear_ratio  减速比 (默认 10.0)
  _enc_ppr     编码器 PPR (默认 11.0)

串口协议 (带 XOR 校验和):
  格式: "<tag> <data...> <CK>\\n"
  CK = payload 所有字节 XOR, 2位大写十六进制

接线:
  J1900 USB 口 → ESP32 Micro USB 口
        但推荐通过 USB 连接, 简单可靠
"""

import rospy
import serial
import math
import time
import threading
from collections import deque

from std_msgs.msg import Header
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Imu
from geometry_msgs.msg import (
    Twist, Pose, Point, Quaternion,
    Vector3, TransformStamped,
    PoseWithCovariance, TwistWithCovariance
)
import tf2_ros
import tf


def xor_checksum(s):
    c = 0
    for ch in s:
        c ^= ord(ch)
    return c


def verify_frame(line):
    """校验帧: 返回 payload (不含校验和), 校验失败返回 None"""
    last_sp = line.rfind(' ')
    if last_sp < 3:
        return None
    payload = line[:last_sp]
    ck_hex  = line[last_sp + 1:]
    try:
        expected = xor_checksum(payload)
        received = int(ck_hex, 16)
        return payload if expected == received else None
    except ValueError:
        return None


class SerialBridge:
    """串口桥接 + 里程计计算"""

    def __init__(self):
        # ===== 读取参数 =====
        port = rospy.get_param("~port", "/dev/ttyUSB0")
        baud = rospy.get_param("~baud", 115200)
        self.wheel_dia   = rospy.get_param("~wheel_dia",   0.085)  # 轮径 85mm
        self.wheel_base  = rospy.get_param("~wheel_base",  0.180)  # 轮距 180mm
        self.gear_ratio  = rospy.get_param("~gear_ratio",  10.0)   # 减速比 1:10
        self.enc_ppr     = rospy.get_param("~enc_ppr",     11.0)   # PPR

        # 编码器每轮转的 tick 数 (ISR CHANGE on A phase: PPR * gear * 2)
        self.ticks_per_rev = self.enc_ppr * self.gear_ratio * 2.0

        # ===== 里程计状态 =====
        self.x     = 0.0   # 位置 X (m)
        self.y     = 0.0   # 位置 Y (m)
        self.yaw   = 0.0   # 朝向 (rad)

        # IMU 角度 (用于修正)
        self.imu_yaw = 0.0
        self.imu_yaw_init = None
        self.imu_yaw_raw = 0.0

        # 线速度/角速度
        self.vx = 0.0
        self.vth = 0.0

        # 电池
        self.battery_v = 0.0

        # 滤波队列 (滑动窗口去噪)
        self.enc_window = deque(maxlen=5)
        self.imu_gz_history = deque(maxlen=10)

        # ===== 时间戳 =====
        self.last_time = rospy.Time.now()
        self.last_imu_time = rospy.Time.now()

        # ===== 发布者 =====
        self.odom_pub = rospy.Publisher("/odom", Odometry, queue_size=50)
        self.imu_pub  = rospy.Publisher("/imu",  Imu,      queue_size=50)

        # ===== 订阅者 =====
        rospy.Subscriber("/cmd_vel", Twist, self.cmd_vel_callback)

        # ===== TF 广播 =====
        self.publish_tf = rospy.get_param("~publish_tf", True)
        self.tf_broadcaster = tf2_ros.TransformBroadcaster()

        # ===== 打开串口 =====
        self.ser_port = port
        self.ser_baud = baud
        self.ser_lock = threading.Lock()
        self.ser = None
        if not self._open_serial():
            rospy.logerr("[Bridge] 无法打开串口 %s", port)
        else:
            rospy.loginfo("[Bridge] 串口 %s 已打开, 波特率 %d", port, baud)

        # 等待 ESP32 启动
        time.sleep(2)
        rospy.loginfo("[Bridge] 初始化完成, 开始主循环")

    # ================================================================
    # cmd_vel 回调: 将 Twist 转为左右轮目标转速(RPM), 发给 ESP32
    # ================================================================
    # 电机转速上限 (RPM) — 保护电机和驱动
    MAX_MOTOR_RPM = 8000.0

    def cmd_vel_callback(self, msg):
        """
        运动学逆解: cmd_vel (v, ω) → 左右轮转速 (RPM)
          v_l = (v - ω * L/2) / (π * D) * 60    (RPM, 轮端)
          v_r = (v + ω * L/2) / (π * D) * 60
        电机轴转速 = 轮端转速 * 减速比
        """
        v = msg.linear.x
        w = msg.angular.z

        # 轮端线速度 (m/s)
        wheel_circumference = math.pi * self.wheel_dia
        v_l = v - w * self.wheel_base / 2.0
        v_r = v + w * self.wheel_base / 2.0

        # 轮端 → 电机轴 RPM
        motor_rpm_l = (v_l / wheel_circumference) * 60.0 * self.gear_ratio
        motor_rpm_r = (v_r / wheel_circumference) * 60.0 * self.gear_ratio

        # 限幅
        motor_rpm_l = max(-self.MAX_MOTOR_RPM, min(self.MAX_MOTOR_RPM, motor_rpm_l))
        motor_rpm_r = max(-self.MAX_MOTOR_RPM, min(self.MAX_MOTOR_RPM, motor_rpm_r))

        payload = "m %.1f %.1f" % (motor_rpm_l, motor_rpm_r)
        cmd = "%s %02X\n" % (payload, xor_checksum(payload))
        try:
            with self.ser_lock:
                self.ser.write(cmd.encode())
        except serial.SerialException:
            rospy.logwarn_throttle(5, "[Bridge] 串口写入失败")

    # ================================================================
    # 解析 ESP32 发来的数据
    # ================================================================
    def parse_line(self, line):
        """解析一行串口数据: e/i/b 三种帧 (含 XOR 校验)"""
        line = line.strip()
        if not line:
            return

        payload = verify_frame(line)
        if payload is None:
            rospy.logwarn_throttle(10, "[Bridge] 校验失败: %s", line)
            return

        parts = payload.split()
        tag = parts[0]

        try:
            if tag == 'e' and len(parts) >= 3:
                self.handle_encoder(int(parts[1]), int(parts[2]))

            elif tag == 'i' and len(parts) >= 7:
                self.handle_imu(
                    float(parts[1]), float(parts[2]), float(parts[3]),
                    float(parts[4]), float(parts[5]), float(parts[6])
                )

            elif tag == 'b' and len(parts) >= 2:
                self.battery_v = float(parts[1])
        except ValueError:
            rospy.logwarn_throttle(10, "[Bridge] 解析失败: %s", payload)

    # ================================================================
    # 编码器处理 → 里程计更新
    # ================================================================
    def handle_encoder(self, delta_left, delta_right):
        """根据编码器增量更新里程计"""
        now = rospy.Time.now()
        dt = (now - self.last_time).to_sec()

        if dt <= 0 or dt > 0.5:
            self.last_time = now
            return

        # 编码器增量 → 轮子转过的弧度
        revs_l = delta_left  / self.ticks_per_rev   # 轮转圈数
        revs_r = delta_right / self.ticks_per_rev

        dist_l = revs_l * math.pi * self.wheel_dia  # 左轮移动距离 (m)
        dist_r = revs_r * math.pi * self.wheel_dia  # 右轮移动距离 (m)

        # 滑动窗口滤波
        self.enc_window.append((dist_l, dist_r, dt))
        if len(self.enc_window) < 3:
            return

        # 用窗口平均
        sum_l = sum_r = sum_dt = 0.0
        for dl, dr, d in self.enc_window:
            sum_l += dl; sum_r += dr; sum_dt += d
        avg_l = sum_l / len(self.enc_window)
        avg_r = sum_r / len(self.enc_window)
        avg_dt = sum_dt / len(self.enc_window)

        if avg_dt <= 0:
            return

        # 差速运动学
        d_center = (avg_l + avg_r) / 2.0          # 车体中心位移
        d_theta  = (avg_r - avg_l) / self.wheel_base  # 转角

        # 更新位姿 (里程计世界坐标系)
        self.x   += d_center * math.cos(self.yaw + d_theta / 2.0)
        self.y   += d_center * math.sin(self.yaw + d_theta / 2.0)
        self.yaw += d_theta

        # 归一化角度 [-π, π]
        self.yaw = math.atan2(math.sin(self.yaw), math.cos(self.yaw))

        # 速度
        self.vx  = d_center / avg_dt
        self.vth = d_theta  / avg_dt

        self.last_time = now

        # 发布里程计和 TF
        self.publish_odometry(now)
        self.publish_odom_tf(now)

    # ================================================================
    # IMU 处理
    # ================================================================
    def handle_imu(self, ax, ay, az, gx, gy, gz):
        """发布 IMU 消息, 并用陀螺仪 Z 轴修正里程计 yaw"""
        now = rospy.Time.now()

        # 累积陀螺仪 z 轴 (角速度 °/s → rad/s)
        dt = (now - self.last_imu_time).to_sec()
        if 0 < dt < 0.5:
            self.imu_yaw_raw += math.radians(gz) * dt
        self.last_imu_time = now

        # 初始化 IMU yaw 偏置 (首次 IMU 数据时, 以里程计当前 yaw 为基准)
        if self.imu_yaw_init is None:
            self.imu_yaw_init = self.yaw - self.imu_yaw_raw

        self.imu_yaw = self.imu_yaw_raw + self.imu_yaw_init

        # 发布 /imu
        imu_msg = Imu()
        imu_msg.header = Header(stamp=now, frame_id="imu_link")
        imu_msg.linear_acceleration.x  = ax
        imu_msg.linear_acceleration.y  = ay
        imu_msg.linear_acceleration.z  = az
        imu_msg.angular_velocity.x = math.radians(gx)
        imu_msg.angular_velocity.y = math.radians(gy)
        imu_msg.angular_velocity.z = math.radians(gz)

        # 协方差 (MPU6050 粗略值)
        imu_msg.linear_acceleration_covariance[0] = 0.01
        imu_msg.linear_acceleration_covariance[4] = 0.01
        imu_msg.linear_acceleration_covariance[8] = 0.01
        imu_msg.angular_velocity_covariance[0] = 0.001
        imu_msg.angular_velocity_covariance[4] = 0.001
        imu_msg.angular_velocity_covariance[8] = 0.001

        self.imu_pub.publish(imu_msg)

    # ================================================================
    # 发布里程计消息
    # ================================================================
    def publish_odometry(self, now):
        """发布 /odom (里程计坐标系下的位姿和速度)"""
        odom = Odometry()
        odom.header = Header(stamp=now, frame_id="odom")
        odom.child_frame_id = "base_footprint"

        # 位姿
        odom.pose.pose = Pose(
            position=Point(self.x, self.y, 0.0),
            orientation=Quaternion(*tf.transformations.quaternion_from_euler(0, 0, self.yaw))
        )
        # 速度
        odom.twist.twist = Twist(
            linear=Vector3(self.vx, 0, 0),
            angular=Vector3(0, 0, self.vth)
        )

        # 协方差 (编码器里程计典型值)
        # pose covariance (x, y, yaw 各有小误差)
        odom.pose.covariance[0]  = 0.01   # x
        odom.pose.covariance[7]  = 0.01   # y
        odom.pose.covariance[35] = 0.02   # yaw
        # twist covariance
        odom.twist.covariance[0]  = 0.005  # vx
        odom.twist.covariance[35] = 0.01   # vth

        self.odom_pub.publish(odom)

    # ================================================================
    # 发布 TF 变换 (odom → base_footprint)
    # ================================================================
    def publish_odom_tf(self, now):
        """广播 odom → base_footprint 的坐标变换 (EKF 模式下不发布)"""
        if not self.publish_tf:
            return
        t = TransformStamped()
        t.header = Header(stamp=now, frame_id="odom")
        t.child_frame_id = "base_footprint"
        t.transform.translation.x = self.x
        t.transform.translation.y = self.y
        t.transform.translation.z = 0.0
        t.transform.rotation = Quaternion(
            *tf.transformations.quaternion_from_euler(0, 0, self.yaw)
        )
        self.tf_broadcaster.sendTransform(t)

    def _open_serial(self):
        """尝试打开串口, 返回 True 成功"""
        try:
            self.ser = serial.Serial(self.ser_port, self.ser_baud, timeout=0.05)
            return True
        except serial.SerialException:
            return False

    # ================================================================
    # 主循环
    # ================================================================
    def run(self):
        """读取串口数据, 解析并发布 (带断线重连)"""
        rate = rospy.Rate(200)
        buf = ""

        while not rospy.is_shutdown():
            if not self.ser or not self.ser.is_open:
                rospy.logwarn_throttle(5, "[Bridge] 串口断开, 尝试重连 %s...", self.ser_port)
                if self._open_serial():
                    rospy.loginfo("[Bridge] 串口已重连")
                    buf = ""
                else:
                    rospy.sleep(1.0)
                    continue

            try:
                while self.ser.in_waiting > 0:
                    raw = self.ser.read(self.ser.in_waiting).decode('utf-8', errors='ignore')
                    buf += raw
                    while '\n' in buf:
                        idx = buf.index('\n')
                        line = buf[:idx]
                        buf = buf[idx + 1:]
                        self.parse_line(line)
            except (serial.SerialException, OSError) as e:
                rospy.logerr("[Bridge] 串口读取错误: %s", e)
                try:
                    self.ser.close()
                except Exception:
                    pass
            except UnicodeDecodeError:
                pass

            rate.sleep()


# ================================================================
# 入口
# ================================================================
if __name__ == "__main__":
    rospy.init_node("serial_bridge")
    try:
        bridge = SerialBridge()
        bridge.run()
    except rospy.ROSInterruptException:
        pass
    except serial.SerialException as e:
        rospy.logerr("[Bridge] 启动失败: %s", e)
