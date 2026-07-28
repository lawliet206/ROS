#!/usr/bin/env python3
"""
S9-FSRD-V1.0 RX 激光雷达 → /scan 话题发布
==========================================
直接读串口并发布 sensor_msgs/LaserScan 到 ROS。

用法:
  python3 lidar_scan_pub.py              # 默认 /dev/ttyUSB0
  python3 lidar_scan_pub.py /dev/ttyUSB1
"""

import serial
import time
import sys
import os
import math
import signal

import rospy
from std_msgs.msg import Header
from sensor_msgs.msg import LaserScan

# ── 协议参数 ──
FRAME_HEADER = b'\xAA\x55'
DATA_FLAG = 0x28
BYTES_PER_POINT = 3
POINTS_PER_FRAME = 40
FRAME_DATA_LEN = 126
DATA_HEADER_BYTES = 6
ANGLE_STEP = 360.0 / 256.0   # 1.40625° per index
NUM_BINS = 360

PORT = sys.argv[1] if len(sys.argv) > 1 else '/dev/ttyUSB0'

if not os.path.exists(PORT):
    print(f"串口 {PORT} 不存在")
    sys.exit(1)

# ── 环形缓冲区 ──
ranges = [float('inf')] * NUM_BINS
intensities = [0.0] * NUM_BINS
coverage = 0

# ── 统计 ──
frame_count = 0
total_frames = 0
log_ts = time.time()
pub_ts = time.time()

# ── 串口 ──
try:
    ser = serial.Serial(PORT, 115200, timeout=0.03)
except serial.SerialException as e:
    print(f"[LiDAR] 无法打开 {PORT}: {e}", file=sys.stderr, flush=True)
    print("请检查: 1)雷达是否连接 2)udev 权限是否配置 3)是否被其他进程占用", file=sys.stderr, flush=True)
    sys.exit(1)
time.sleep(0.3)
ser.reset_input_buffer()
rospy.loginfo(f"[LiDAR] 串口 {PORT} 已打开 @ 115200")

# ── ROS ──
rospy.init_node('s9_lidar_pub', anonymous=False)
pub = rospy.Publisher('/scan', LaserScan, queue_size=10)

# ── 退出 ──
running = True
def cleanup(*_):
    global running
    running = False
signal.signal(signal.SIGINT, cleanup)

# ── 解析 ──
buf = bytearray()

def read_and_parse():
    global buf, coverage, frame_count, total_frames
    
    raw = ser.read(512)
    if not raw:
        return
    buf.extend(raw)
    
    max_iter = 200
    for _ in range(max_iter):
        p = buf.find(FRAME_HEADER)
        if p < 0:
            if len(buf) > 4:
                buf = buf[-3:]
            return
        
        rest = buf[p:]
        next_p = rest.find(FRAME_HEADER, 2)
        if next_p <= 0:
            return
        
        frame = bytes(rest[:next_p])
        buf = rest[next_p:]
        
        if len(frame) < 6:
            continue
        idx = frame[2]
        flag = frame[3]
        if flag != DATA_FLAG or len(frame) < 4 + FRAME_DATA_LEN:
            continue
        
        data = frame[4:4 + FRAME_DATA_LEN]
        payload = data[DATA_HEADER_BYTES:]
        
        frame_count += 1
        for i in range(POINTS_PER_FRAME):
            off = i * BYTES_PER_POINT
            if off + BYTES_PER_POINT > len(payload):
                break
            quality = payload[off]
            d0 = payload[off + 1]
            d1 = payload[off + 2]
            dist_raw = d0 | (d1 << 8)
            
            if dist_raw == 0 or dist_raw >= 0xFFF0:
                continue
            
            dist_m = dist_raw / 1000.0
            if dist_m < 0.01 or dist_m > 100.0:
                continue
            
            angle_deg = idx * ANGLE_STEP + (i / POINTS_PER_FRAME) * ANGLE_STEP
            angle_deg = angle_deg % 360
            bin_idx = int(round(angle_deg)) % NUM_BINS
            
            if ranges[bin_idx] == float('inf'):
                coverage += 1
            ranges[bin_idx] = min(ranges[bin_idx], dist_m)
            intensities[bin_idx] = max(intensities[bin_idx], float(quality))

# ── 发布 ──
def publish_scan():
    global total_frames
    
    now = rospy.Time.now()
    scan = LaserScan()
    scan.header = Header(stamp=now, frame_id='laser_link')
    scan.angle_min = -math.pi
    scan.angle_max = math.pi
    scan.angle_increment = math.radians(1.0)
    scan.time_increment = 1e-4
    scan.scan_time = 0.2
    scan.range_min = 0.05
    scan.range_max = 50.0
    scan.ranges = list(ranges)
    scan.intensities = list(intensities)
    pub.publish(scan)

# ── 主循环 ──
rospy.loginfo("[LiDAR] 开始发布 /scan")

while running and not rospy.is_shutdown():
    # 快读串口
    for _ in range(5):
        read_and_parse()
    
    # 统计
    now = time.time()
    if now - log_ts >= 3.0:
        total_frames += frame_count
        fps = frame_count / 3.0
        rospy.loginfo(f"[LiDAR] {fps:.0f} fps, {coverage}/{NUM_BINS} bins, {total_frames} total")
        frame_count = 0
        log_ts = now
    
    # 发布 (10Hz, 有数据就发)
    if coverage > 3 and now - pub_ts >= 0.1:
        publish_scan()
        pub_ts = now

# ── 清理 ──
ser.close()
rospy.loginfo("[LiDAR] 已停止")
