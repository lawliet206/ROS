#!/usr/bin/env python3
"""
S9-FSRD-V1.0 RX 激光雷达 ROS 驱动节点 v4
==========================================
协议: 115200 8N1, AA 55 帧头
每帧 40 个点 (强度1B + 距离2B LE)
累积多帧合成完整 360° 扫描, 5Hz 发布 → 给 gmapping 正常使用

使用:
  rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB0
"""

import rospy
import serial
import math
import time
import threading

from std_msgs.msg import Header
from sensor_msgs.msg import LaserScan

FRAME_HEADER = b'\xAA\x55'
STATUS_FRAME_TYPE = 0x7D
LARGE_FRAME_FLAG = 0x28
DATA_HEADER_BYTES = 6
BYTES_PER_POINT = 3
POINTS_PER_FRAME = 40
FRAME_DATA_LEN = 126
ANGLE_STEP_DEG = 360.0 / 256.0  # 1.40625°
NUM_BINS = 360  # 1° 分辨率


class S9LidarDriver:
    def __init__(self):
        port = rospy.get_param("~port", "/dev/ttyUSB0")
        self.frame_id = rospy.get_param("~frame_id", "laser_link")
        self.angle_offset = rospy.get_param("~angle_offset", 0.0)
        self.min_range = rospy.get_param("~min_range", 0.10)
        self.max_range = rospy.get_param("~max_range", 12.0)

        self.running = True
        self.total_frames = 0
        self.last_log = time.time()

        # 360° 环形缓冲区, 持续累积
        self.ranges = [float('inf')] * NUM_BINS
        self.intensities = [0] * NUM_BINS
        self.coverage = 0  # 已覆盖的不同角度数
        self.last_publish = time.time()

        self.scan_pub = rospy.Publisher("/scan", LaserScan, queue_size=10)

        # 打开串口
        try:
            import os
            if not os.access(port, os.R_OK | os.W_OK):
                os.system(f"sudo chmod 666 {port} 2>/dev/null")
                time.sleep(0.3)
            self.ser = serial.Serial(port, 115200, timeout=0.05)
            rospy.loginfo(f"[S9] 串口 {port} 已打开 @ 115200")
        except serial.SerialException as e:
            rospy.logerr(f"[S9] 无法打开串口: {e}")
            raise

        time.sleep(0.5)
        self.ser.reset_input_buffer()

        self.read_thread = threading.Thread(target=self.read_loop)
        self.read_thread.daemon = True
        self.read_thread.start()

        # 定时发布线程
        self.pub_thread = threading.Thread(target=self.pub_loop)
        self.pub_thread.daemon = True
        self.pub_thread.start()

    def add_points(self, idx, points):
        """将一帧的点合并到 360° 环形缓冲区"""
        n = len(points)
        for i, (dist_m, quality) in enumerate(points):
            # 精确角度 = 角度索引 + 帧内偏移
            angle_deg = idx * ANGLE_STEP_DEG + (i / n) * ANGLE_STEP_DEG
            angle_deg = (angle_deg + self.angle_offset) % 360

            bin_idx = int(round(angle_deg)) % NUM_BINS

            # 更新: 取更小的距离 (靠近物体优先)
            if self.ranges[bin_idx] == float('inf'):
                self.coverage += 1
            self.ranges[bin_idx] = dist_m
            self.intensities[bin_idx] = quality

    def publish_scan(self):
        """发布 360° 扫描"""
        now = rospy.Time.now()

        scan = LaserScan()
        scan.header = Header(stamp=now, frame_id=self.frame_id)
        scan.angle_min = -math.pi
        scan.angle_max = math.pi
        scan.angle_increment = math.radians(1.0)
        scan.time_increment = 0.0001
        scan.scan_time = 0.2
        scan.range_min = self.min_range
        scan.range_max = self.max_range
        scan.ranges = list(self.ranges)
        scan.intensities = list(self.intensities)

        self.scan_pub.publish(scan)

    def parse_frame(self, frame):
        """解析一帧, 存入缓冲区"""
        if len(frame) < 6:
            return

        idx = frame[2]
        flag = frame[3]

        if idx == STATUS_FRAME_TYPE:
            return
        if flag != LARGE_FRAME_FLAG:
            return
        if len(frame) < 4 + FRAME_DATA_LEN:
            return

        data = frame[4:4 + FRAME_DATA_LEN]
        payload = data[DATA_HEADER_BYTES:]

        points = []
        for i in range(POINTS_PER_FRAME):
            offset = i * BYTES_PER_POINT
            if offset + BYTES_PER_POINT > len(payload):
                break

            quality = payload[offset]
            d0 = payload[offset + 1]
            d1 = payload[offset + 2]
            dist_raw = d0 | (d1 << 8)

            if dist_raw == 0 or dist_raw >= 0xFFF0:
                continue

            dist_m = dist_raw / 1000.0
            if dist_m < self.min_range or dist_m > self.max_range:
                continue

            points.append((dist_m, quality))

        if points:
            self.add_points(idx, points)

    def pub_loop(self):
        """每 0.2 秒发布一次扫描 (5Hz)"""
        rate = rospy.Rate(5)
        while self.running and not rospy.is_shutdown():
            if self.coverage > 30:  # 至少有 30 个角度有数据
                self.publish_scan()
                rate.sleep()

    def read_loop(self):
        buf = bytearray()

        while self.running and not rospy.is_shutdown():
            try:
                raw = self.ser.read(512)
                if not raw:
                    continue
                buf.extend(raw)

                while True:
                    p = buf.find(FRAME_HEADER)
                    if p < 0:
                        if len(buf) > 4:
                            buf = buf[-3:]
                        break

                    remaining = buf[p:]
                    next_p = remaining.find(FRAME_HEADER, 2)
                    if next_p > 0:
                        frame = bytes(remaining[:next_p])
                        buf = buf[p + next_p:]
                        self.total_frames += 1
                        self.parse_frame(frame)
                    else:
                        break

                # 日志
                now = time.time()
                if now - self.last_log > 3.0:
                    rospy.loginfo(f"[S9] {self.total_frames/3:.0f}帧/秒, {self.coverage}/360角度有数据")
                    self.total_frames = 0
                    self.last_log = now

            except Exception as e:
                rospy.logwarn_throttle(5, f"[S9] 异常: {e}")
                time.sleep(0.1)

    def spin(self):
        while self.running and not rospy.is_shutdown():
            time.sleep(1.0)

    def stop(self):
        self.running = False
        if hasattr(self, 'ser') and self.ser and self.ser.is_open:
            self.ser.close()


if __name__ == "__main__":
    rospy.init_node("s9_lidar_driver")
    try:
        driver = S9LidarDriver()
        driver.spin()
    except rospy.ROSInterruptException:
        pass
    except Exception as e:
        rospy.logerr(f"[S9] 启动失败: {e}")
