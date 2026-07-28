#!/usr/bin/env python3
"""S9-FSRD ROS 驱动 — SLAM 版"""
import serial, time, sys, os, math, struct, signal, threading
import rospy
from std_msgs.msg import Header
from sensor_msgs.msg import LaserScan

PORT = sys.argv[1] if len(sys.argv) > 1 else '/dev/ttyUSB1'
FRAME_HEADER = b'\xAA\x55'
NUM_BINS = 360
ANGLE_64 = 64.0
CROSS_HI = 260  # 降低跨零阈值：260° (原 340°太高)
CROSS_LO = 100

class S9Lidar:
    def __init__(self):
        self.lock = threading.Lock()
        self.ranges = [float('inf')] * NUM_BINS
        self.intensities = [0.0] * NUM_BINS
        self.fill_count = 0
        self.last_pub = time.time()
        self.last_first = -1
        self.scan_n = 0
        self.frame_n = 0
        self.log_t = time.time()
        self.cross_n = 0

        # 串口权限由 udev 规则管理 (见 SETUP.md 4.6)
        self.ser = serial.Serial(PORT, 115200, timeout=0.02)
        time.sleep(0.3)
        self.ser.reset_input_buffer()
        self.pub = rospy.Publisher('/scan', LaserScan, queue_size=3)
        rospy.loginfo(f"[S9] {PORT} ok | cross={CROSS_HI}deg")

    def publish(self):
        with self.lock:
            if self.fill_count < 30:
                return
            s = LaserScan()
            s.header = Header(stamp=rospy.Time.now(), frame_id='laser_link')
            s.angle_min, s.angle_max = -math.pi, math.pi
            s.angle_increment = math.radians(1.0)
            s.time_increment = 1e-4
            s.scan_time = 0.1
            s.range_min, s.range_max = 0.03, 50.0
            s.ranges = list(self.ranges)
            s.intensities = list(self.intensities)
            self.pub.publish(s)
            self.scan_n += 1
            self.ranges = [float('inf')] * NUM_BINS
            self.intensities = [0.0] * NUM_BINS
            self.fill_count = 0
        self.last_pub = time.time()

    def run(self):
        buf = bytearray()
        while not rospy.is_shutdown():
            try:
                raw = self.ser.read(256)
                if not raw: continue
                buf.extend(raw)

                for _ in range(200):
                    p = buf.find(FRAME_HEADER)
                    if p < 0:
                        if len(buf) > 4: buf = buf[-3:]
                        break
                    rest = buf[p:]
                    n = rest.find(FRAME_HEADER, 2)
                    if n <= 0: break
                    f = rest[:n]; buf = rest[n:]
                    if len(f) < 12: continue
                    cnt = f[3]
                    if cnt == 0 or cnt > 80: continue
                    if len(f) < 10 + cnt * 3: continue

                    self.frame_n += 1
                    fr = struct.unpack('<H', f[4:6])[0]
                    lr = struct.unpack('<H', f[6:8])[0]
                    first, last = fr >> 1, lr >> 1
                    pl = f[10:]

                    # 跨零检测 (降低阈值)
                    cross = (self.last_first >= 0 and
                             self.last_first > CROSS_HI * ANGLE_64 and
                             first < CROSS_LO * ANGLE_64)
                    self.last_first = first

                    if cross:
                        self.cross_n += 1
                        self.publish()

                    # 角度区间
                    if last < first and first > 17280 and last < 5760:
                        span = 23040 + last - first
                    elif last >= first:
                        span = last - first
                    else:
                        span = 1
                    step = span / (cnt-1) if cnt > 1 else 0

                    with self.lock:
                        for i in range(cnt):
                            off = i*3
                            if off+3 > len(pl): break
                            d0, d1 = pl[off+1], pl[off+2]
                            dr = d0 | (d1 << 8)
                            if dr == 0 or dr >= 0xFFF0: continue
                            dm = dr / 1000.0
                            if dm < 0.03 or dm > 50: continue
                            ang = (first + step*i) / ANGLE_64 % 360
                            bi = int(round(ang)) % 360
                            if self.ranges[bi] == float('inf'):
                                self.fill_count += 1
                            if dm < self.ranges[bi]:
                                self.ranges[bi] = dm
                            self.intensities[bi] = max(self.intensities[bi], pl[off])

                # 超时兜底
                now = time.time()
                if now - self.last_pub > 0.5:
                    self.publish()

                if now - self.log_t > 3:
                    v = sum(1 for r in self.ranges if r < 49)
                    rospy.loginfo(f"[S9] {self.frame_n/3:.0f}fps | #{self.scan_n} | cross={self.cross_n} | {v}pts")
                    self.frame_n = 0
                    self.log_t = now

            except Exception as e:
                rospy.logwarn(f"[S9] {e}")
                time.sleep(0.1)
        self.ser.close()

if __name__ == "__main__":
    rospy.init_node('s9_lidar')
    running = True
    def on_sigint(sig, frame):
        global running
        running = False
        rospy.signal_shutdown("SIGINT")
    signal.signal(signal.SIGINT, on_sigint)
    signal.signal(signal.SIGTERM, on_sigint)
    lidar = S9Lidar()
    try:
        while not rospy.is_shutdown() and running:
            lidar.run()
    finally:
        lidar.ser.close()
