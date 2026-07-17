#!/usr/bin/env python3
"""
S9-FSRD-V1.0 RX 激光雷达 ROS 驱动节点 v5
==========================================
协议: 115200 8N1, AA 55 帧头
帧格式: AA 55 [ct:1B] [count:1B] [firstAngle:2B] [lastAngle:2B] [cs:2B] [nodes: count×3B]
每个点: quality(1B) + dist(2B LE, mm)
角度单位: 1/64 度 (firstAngle/64 = 角度°)

跨零检测: firstAngle 从 >260° 跳回 <100° 时触发一圈完整扫描发布
超时兜底: 0.5s 没收到跨零也发布, 保证数据不断流

使用:
  rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB0
"""
import rospy
import serial
import math
import time
import struct

from std_msgs.msg import Header
from sensor_msgs.msg import LaserScan

FRAME_HEADER = b'\xAA\x55'
NUM_BINS = 360
ANGLE_64 = 64.0        # 1 degree = 64 units
CROSS_HI = 260          # 跨零高阈值 (度)
CROSS_LO = 100          # 跨零低阈值 (度)


class S9LidarDriver:
    def __init__(self):
        port = rospy.get_param("~port", "/dev/ttyUSB0")
        self.frame_id = rospy.get_param("~frame_id", "laser_link")
        self.min_range = rospy.get_param("~min_range", 0.03)
        self.max_range = rospy.get_param("~max_range", 50.0)

        self.running = True
        self.frame_count = 0
        self.scan_count = 0
        self.cross_count = 0
        self.last_log = time.time()

        # 360° 缓冲区
        self.ranges = [float('inf')] * NUM_BINS
        self.intensities = [0.0] * NUM_BINS
        self.fill_count = 0

        self.last_first = -1     # 上一帧 firstAngle
        self.last_publish = time.time()

        self.scan_pub = rospy.Publisher("/scan", LaserScan, queue_size=3)

        # 打开串口
        try:
            import os
            os.system(f"sudo chmod 666 {port} 2>/dev/null")
            time.sleep(0.2)
            self.ser = serial.Serial(port, 115200, timeout=0.02)
            rospy.loginfo("[S9] %s @ 115200 | cross=%ddeg", port, CROSS_HI)
        except serial.SerialException as e:
            rospy.logerr("[S9] 无法打开 %s: %s", port, e)
            raise

        time.sleep(0.3)
        self.ser.reset_input_buffer()

    def publish_scan(self):
        """发布一圈完整的 360° 扫描"""
        if self.fill_count < 30:
            return

        scan = LaserScan()
        scan.header = Header(stamp=rospy.Time.now(), frame_id=self.frame_id)
        scan.angle_min = -math.pi
        scan.angle_max = math.pi
        scan.angle_increment = math.radians(1.0)
        scan.time_increment = 1e-4
        scan.scan_time = 0.1
        scan.range_min = self.min_range
        scan.range_max = self.max_range
        scan.ranges = list(self.ranges)
        scan.intensities = list(self.intensities)

        self.scan_pub.publish(scan)
        self.scan_count += 1

        # 清空缓冲区
        self.ranges = [float('inf')] * NUM_BINS
        self.intensities = [0.0] * NUM_BINS
        self.fill_count = 0
        self.last_publish = time.time()

    def parse_frame(self, frame):
        """解析 AA 55 帧, 按 firstAngle/lastAngle 逐点插值"""
        if len(frame) < 12:
            return

        cnt = frame[3]
        if cnt == 0 or cnt > 80:
            return
        if len(frame) < 10 + cnt * 3:
            return

        # 读取 firstAngle / lastAngle (1/64 度)
        fr = struct.unpack_from("<H", frame, 4)[0]
        lr = struct.unpack_from("<H", frame, 6)[0]
        first = fr >> 1    # bit0 校验位, 右移 1 得到真实 15-bit 角度
        last = lr >> 1

        self.frame_count += 1

        # 跨零检测
        cross = (self.last_first >= 0 and
                 self.last_first > CROSS_HI * ANGLE_64 and
                 first < CROSS_LO * ANGLE_64)
        self.last_first = first

        if cross:
            self.cross_count += 1
            self.publish_scan()

        # 角度区间 (处理跨零)
        if last < first and first > 17280 and last < 5760:
            span = 23040 + last - first   # 跨零
        elif last >= first:
            span = last - first
        else:
            span = 1

        step = span / (cnt - 1) if cnt > 1 else 0
        payload = frame[10:]

        for i in range(cnt):
            off = i * 3
            if off + 3 > len(payload):
                break

            quality = payload[off]
            d0 = payload[off + 1]
            d1 = payload[off + 2]
            dist_raw = d0 | (d1 << 8)

            if dist_raw == 0 or dist_raw >= 0xFFF0:
                continue

            dist_m = dist_raw / 1000.0
            if dist_m < self.min_range or dist_m > self.max_range:
                continue

            angle_deg = (first + step * i) / ANGLE_64 % 360
            bin_idx = int(round(angle_deg)) % NUM_BINS

            if self.ranges[bin_idx] == float('inf'):
                self.fill_count += 1
            if dist_m < self.ranges[bin_idx]:
                self.ranges[bin_idx] = dist_m
            self.intensities[bin_idx] = max(self.intensities[bin_idx], quality)

    def run(self):
        buf = bytearray()

        while self.running and not rospy.is_shutdown():
            try:
                raw = self.ser.read(256)
                if not raw:
                    continue
                buf.extend(raw)

                # 从缓冲区提取完整帧
                for _ in range(200):
                    p = buf.find(FRAME_HEADER)
                    if p < 0:
                        if len(buf) > 4:
                            buf = buf[-3:]
                        break

                    rest = buf[p:]
                    n = rest.find(FRAME_HEADER, 2)
                    if n <= 0:
                        break

                    frame = bytes(rest[:n])
                    buf = rest[n:]
                    self.parse_frame(frame)

                # 超时兜底: 0.5s 没跨零也发布
                now = time.time()
                if now - self.last_publish > 0.5 and self.fill_count >= 30:
                    self.publish_scan()

                # 日志
                if now - self.last_log > 3.0:
                    pts = sum(1 for r in self.ranges if r < self.max_range)
                    rospy.loginfo("[S9] %dfps | scan#%d cross=%d | %dpts/360",
                                  round(self.frame_count / 3), self.scan_count,
                                  self.cross_count, pts)
                    self.frame_count = 0
                    self.last_log = now

            except Exception as e:
                rospy.logwarn_throttle(5, "[S9] %s", e)
                time.sleep(0.1)

        self.ser.close()

    def stop(self):
        self.running = False
        if hasattr(self, 'ser') and self.ser and self.ser.is_open:
            self.ser.close()


if __name__ == "__main__":
    rospy.init_node("s9_lidar_driver")
    try:
        driver = S9LidarDriver()
        driver.run()
    except rospy.ROSInterruptException:
        pass
