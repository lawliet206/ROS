#!/usr/bin/env python3
"""S9-FSRD 专用可视化 — 模拟 Windows 工具效果"""
import math, time, collections, threading, struct, sys, os
import numpy as np
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import matplotlib.animation as anim
import serial

FRAME_HEADER = b'\xAA\x55'
PORT = sys.argv[1] if len(sys.argv) > 1 else '/dev/ttyUSB0'

try:
    ser = serial.Serial(PORT, 115200, timeout=0.02)
except serial.SerialException as e:
    print(f"无法打开串口 {PORT}: {e}", file=sys.stderr)
    sys.exit(1)
time.sleep(0.3)
ser.reset_input_buffer()

# 共享数据
lock = threading.Lock()
all_points = []  # [(angle_deg, dist_m, quality, timestamp)]
running = True

def read_loop():
    global all_points
    buf = bytearray()
    while running:
        try:
            raw = ser.read(512)
            if not raw: continue
            buf.extend(raw)
            for _ in range(300):
                p = buf.find(FRAME_HEADER)
                if p < 0:
                    if len(buf) > 4: buf = buf[-3:]
                    break
                rest = buf[p:]
                n = rest.find(FRAME_HEADER, 2)
                if n <= 0: break
                f = rest[:n]
                buf = rest[n:]
                if len(f) < 12: continue
                ct, cnt = f[2], f[3]
                if cnt == 0 or cnt > 80: continue
                if len(f) < 10 + cnt * 3 + 2: continue
                first_raw = struct.unpack('<H', f[4:6])[0]
                last_raw = struct.unpack('<H', f[6:8])[0]
                first, last = first_raw >> 1, last_raw >> 1
                payload = f[10:]
                now = time.time()

                if last < first and first > 17280 and last < 5760:
                    span = 23040 + last - first
                elif last >= first:
                    span = last - first
                else:
                    span = 1
                step = span / (cnt - 1) if cnt > 1 else 0

                pts = []
                for i in range(cnt):
                    off = i * 3
                    if off + 3 > len(payload): break
                    dr = payload[off+1] | (payload[off+2] << 8)
                    if dr == 0 or dr >= 0xFFF0: continue
                    dm = dr / 1000.0
                    if dm < 0.03 or dm > 50.0: continue
                    ang = (first + step * i) / 64.0 % 360
                    pts.append((ang, dm, float(payload[off]), now))

                with lock:
                    all_points.extend(pts)
                    # 只保留最近0.5秒
                    cutoff = now - 0.5
                    all_points = [p for p in all_points if p[3] > cutoff]

        except Exception:
            time.sleep(0.1)

t = threading.Thread(target=read_loop, daemon=True)
t.start()
time.sleep(1)

fig, ax = plt.subplots(subplot_kw={'projection': 'polar'}, figsize=(10, 9),
                        facecolor='#1a1a2e')
ax.set_facecolor('#1a1a2e')
ax.set_theta_zero_location('N')
ax.set_theta_direction(-1)
ax.set_ylim(0, 4)
ax.grid(True, alpha=0.2, color='gray')
ax.tick_params(colors='gray')
ax.set_title('S9-FSRD-V1.0  LiDAR Live View', color='white', fontsize=14, pad=20)

scat = ax.scatter([], [], s=40, c=[], cmap='inferno', vmin=0, vmax=255, alpha=0.85)

def update(frame):
    with lock:
        if not all_points:
            return [scat]
        pts = list(all_points)
        angles = [math.radians(p[0]) for p in pts]
        dists = [p[1] for p in pts]
        quals = [p[2] for p in pts]
        max_d = max(dists) * 1.1 if dists else 4
        ax.set_ylim(0, max(0.5, max_d))
    scat.set_offsets(np.column_stack([angles, dists]))
    scat.set_array(np.array(quals))
    fig.suptitle(f'{len(pts)} points | {len(set(int(p[0]) for p in pts))}/360 bins',
                 color='white', fontsize=12)
    return [scat]

ani = anim.FuncAnimation(fig, update, interval=50, blit=False, cache_frame_data=False)
plt.show()
ser.close()
