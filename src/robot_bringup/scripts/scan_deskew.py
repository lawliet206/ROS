#!/usr/bin/env python3
"""
scan_deskew.py — IMU 激光扫描去畸变
====================================
用 IMU 角速度补偿激光扫描过程中的机器人旋转运动。

原理:
  S9 雷达转一圈 ~0.17s, 若车在旋转, 扫描起始点和结束点测到的是
  不同朝向的数据。用 IMU 的 ω_z 反推出每个点的真实世界角度。

输入: /scan (LaserScan) + /imu (Imu)
输出: /scan_deskewed (LaserScan, 修正后的扫描数据)

接入方式:
  rosrun robot_bringup scan_deskew.py
  roslaunch robot_bringup slam.launch deskew:=true
"""
import rospy
import math
from collections import deque
from sensor_msgs.msg import LaserScan, Imu


class ScanDeskew:
    def __init__(self):
        # IMU 环形缓冲区 (~10s)
        self.imu_buf = deque(maxlen=500)
        # 上一帧时间戳, 用于估算真实扫描周期
        self.last_stamp = 0.0
        self.scan_period = 0.17  # 初始估值, 会动态更新

        self.scan_pub = rospy.Publisher("/scan_deskewed", LaserScan,
                                        queue_size=10)
        rospy.Subscriber("/scan", LaserScan, self.scan_cb)
        rospy.Subscriber("/imu", Imu, self.imu_cb)
        rospy.loginfo("[Deskew] 已启动")

    def imu_cb(self, msg):
        self.imu_buf.append((msg.header.stamp.to_sec(),
                             msg.angular_velocity.z))

    def _get_yaw_rate(self, t):
        """取时间 t 附近 IMU 角速度均值. 无数据返回 0."""
        if not self.imu_buf:
            return 0.0
        # 取 ±50ms 窗口内的 IMU 读数, 取均值平滑
        window = [w for ts, w in self.imu_buf if abs(ts - t) < 0.05]
        if len(window) < 2:
            # 窗口不够, 直接用最近值
            best = min(self.imu_buf, key=lambda x: abs(x[0] - t))
            return best[1] if abs(best[0] - t) < 0.1 else 0.0
        return sum(window) / len(window)

    def scan_cb(self, scan):
        N = len(scan.ranges)
        if N == 0:
            return

        # 估算真实扫描周期 (drivers 给的 scan_time 经常不准)
        now = scan.header.stamp.to_sec()
        if self.last_stamp > 0:
            dt = now - self.last_stamp
            if 0.05 < dt < 0.5:  # 合理范围才更新
                self.scan_period = 0.7 * self.scan_period + 0.3 * dt
        self.last_stamp = now

        # 扫描开始时间 = stamp - 扫描周期
        # 近似: stamp 由 S9 驱动在 publish_scan() 设 rospy.Time.now(),
        #       约为扫描结束时刻。±10ms 误差被 IMU 50ms 均值窗口吸收。
        scan_start = now - self.scan_period

        # 取扫描中点 IMU 角速度
        yaw_rate = self._get_yaw_rate(scan_start + self.scan_period / 2.0)
        total_yaw = yaw_rate * self.scan_period

        corrected = LaserScan()
        corrected.header = scan.header
        corrected.angle_min = scan.angle_min
        corrected.angle_max = scan.angle_max
        corrected.angle_increment = scan.angle_increment
        corrected.time_increment = scan.time_increment
        corrected.scan_time = scan.scan_time
        corrected.range_min = scan.range_min
        corrected.range_max = scan.range_max
        corrected.intensities = list(scan.intensities) if scan.intensities else []

        # 逐点校正: 点 i 的时间偏移 → 该点的旋转角 → 修正角度
        new_ranges = [float('inf')] * N

        for i in range(N):
            r = scan.ranges[i]
            if not math.isfinite(r) or r < scan.range_min or r > scan.range_max:
                continue

            # 点 i 的采集时间偏移 (0 = 扫描开始, 1 = 扫描结束)
            frac = i / max(N - 1, 1)
            # 该点的旋转补偿角 → 加到原始角度上
            yaw_corr = total_yaw * frac
            angle = scan.angle_min + i * scan.angle_increment
            corrected_angle = angle + yaw_corr

            # 规整到 [-π, π]
            corrected_angle = math.atan2(math.sin(corrected_angle),
                                         math.cos(corrected_angle))

            # 映射到 bin, 处理 +π 边界 (atan2 可能返回 π)
            bi = int(round((corrected_angle - scan.angle_min) /
                           scan.angle_increment))
            if bi < 0:
                bi += N
            elif bi >= N:
                bi -= N
            if r < new_ranges[bi]:
                new_ranges[bi] = r

        corrected.ranges = new_ranges
        self.scan_pub.publish(corrected)

        rospy.loginfo_throttle(5.0,
            "[Deskew] yaw=%.1f°/s  corr=%.1f°  period=%.0fms",
            math.degrees(yaw_rate), math.degrees(total_yaw),
            self.scan_period * 1000)


if __name__ == "__main__":
    rospy.init_node("scan_deskew")
    ScanDeskew()
    rospy.spin()
