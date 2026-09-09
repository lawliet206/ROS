#!/usr/bin/env python3
"""
S9-FSRD-V1.0 雷达 — 协议分析 + 连续显示
==========================================
已确认: 115200 波特率, AA 55 帧头
Ctrl-C 停止
"""

import serial
import time
import sys
import os
from collections import deque

PORT = sys.argv[1] if len(sys.argv) > 1 else '/dev/ttyUSB0'

if not os.path.exists(PORT):
    print(f"❌ {PORT} 不存在")
    sys.exit(1)

# 串口权限由 udev 规则管理 (见 SETUP.md 4.6)
try:
    ser = serial.Serial(PORT, 115200, timeout=0.1)
except serial.SerialException as e:
    print(f"无法打开 {PORT}: {e}", file=sys.stderr)
    sys.exit(1)
time.sleep(0.5)
ser.reset_input_buffer()

print(f"🔍 监听 {PORT} @ 115200 baud")
print(f"   按帧头 AA 55 分割数据包\n")

buf = bytearray()
pkt_count = 0
pkt_sizes = deque(maxlen=20)
start = time.time()

try:
    while True:
        raw = ser.read(512)
        if not raw:
            continue
        buf.extend(raw)

        # 按 AA 55 分割
        while True:
            idx = buf.find(b'\xAA\x55')
            if idx < 0:
                # 没找到, 但保留尾部以防跨边界
                if len(buf) > 4:
                    buf = buf[-3:]
                break

            if idx > 0:
                # AA 55 之前的字节(可能是前一帧的校验/尾部)
                prev = buf[:idx]

            if idx + 2 < len(buf):
                pkt = buf[idx:]
                # 找下一个 AA 55
                next_idx = pkt.find(b'\xAA\x55', 2)
                if next_idx > 0:
                    frame = pkt[:next_idx]
                    buf = pkt[next_idx:]
                else:
                    # 不完整, 等数据
                    break

                pkt_count += 1
                pkt_sizes.append(len(frame))

                # 显示帧
                if pkt_count <= 10 or pkt_count % 50 == 0:
                    hex_str = ' '.join(f'{b:02X}' for b in frame[:20])
                    extra = f'... +{len(frame)-20}B' if len(frame) > 20 else ''
                    sz_dist = list(pkt_sizes)
                    sizes = f'[帧大小: {min(sz_dist)}-{max(sz_dist)}B]' if sz_dist else ''
                    print(f"  #{pkt_count:5d}  {hex_str}{extra}  {sizes}")

            else:
                break

except KeyboardInterrupt:
    elapsed = time.time() - start
    print(f"\n\n📊 统计:")
    print(f"  运行时间: {elapsed:.1f}s")
    print(f"  总帧数:   {pkt_count}")
    print(f"  帧速率:   {pkt_count/elapsed:.0f} 帧/秒")
    ser.close()
