#!/usr/bin/env python3
"""
S9-FSRD-V1.0 RX 激光雷达实时可视化
===================================
直接读取雷达串口数据，实时显示 360° 扫描效果。

用法:
  python3 lidar_view.py              # 图形界面 (默认 /dev/ttyUSB0)
  python3 lidar_view.py --cli        # 终端 ASCII 模式
  python3 lidar_view.py /dev/ttyUSB1  # 指定串口
"""

import serial
import time
import sys
import os
import math
import signal
import json

# ── 协议参数 ──
FRAME_HEADER = b'\xAA\x55'
STATUS_TYPE = 0x7D
DATA_FLAG = 0x28
BYTES_PER_POINT = 3
POINTS_PER_FRAME = 40
FRAME_DATA_LEN = 126
DATA_HEADER_BYTES = 6
ANGLE_STEP = 360.0 / 256.0   # 1.40625° per index
NUM_BINS = 360                # 显示分辨率 1°

# ── 串口 ──
args = [a for a in sys.argv[1:] if not a.startswith('--')]
flags = set(a for a in sys.argv[1:] if a.startswith('--'))
PORT = args[0] if args else '/dev/ttyUSB0'
CLI_MODE = '--cli' in flags

if not os.path.exists(PORT):
    print(f"❌ 串口 {PORT} 不存在")
    sys.exit(1)

try:
    os.system(f"sudo chmod 666 {PORT} 2>/dev/null")
    time.sleep(0.1)
    ser = serial.Serial(PORT, 115200, timeout=0.03)
    time.sleep(0.3)
    ser.reset_input_buffer()
except serial.SerialException as e:
    print(f"❌ 无法打开串口: {e}")
    sys.exit(1)

# ── 环形缓冲区 ──
ranges = [float('inf')] * NUM_BINS
intensities = [0] * NUM_BINS
coverage = 0

# ── 统计 ──
stats = {
    'fps': 0, 'total_frames': 0, 'total_points': 0,
    'frame_count': 0, 'log_ts': time.time(), 'reset_ts': time.time(),
}

running = True
def cleanup(*_):
    global running
    running = False
signal.signal(signal.SIGINT, cleanup)
signal.signal(signal.SIGTERM, cleanup)

# ── 解析帧 ──
def parse_frame(frame: bytes):
    global ranges, intensities, coverage

    if len(frame) < 6:
        return
    idx = frame[2]
    flag = frame[3]
    if idx == STATUS_TYPE or flag != DATA_FLAG:
        return
    if len(frame) < 4 + FRAME_DATA_LEN:
        return

    data = frame[4:4 + FRAME_DATA_LEN]
    payload = data[DATA_HEADER_BYTES:]

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
        intensities[bin_idx] = max(intensities[bin_idx], quality)
        stats['total_points'] += 1

# ── 读取串口 ──
buf = bytearray()

def read_serial():
    global buf
    raw = ser.read(512)
    if not raw:
        return
    buf.extend(raw)

    max_parse = 200  # 防止死循环
    parse_count = 0
    while parse_count < max_parse:
        p = buf.find(FRAME_HEADER)
        if p < 0:
            if len(buf) > 4:
                buf = buf[-3:]
            break

        remaining = buf[p:]
        next_p = remaining.find(FRAME_HEADER, 2)
        if next_p > 0:
            frame = bytes(remaining[:next_p])
            buf = remaining[next_p:]
            stats['frame_count'] += 1
            try:
                parse_frame(frame)
            except Exception:
                pass
            parse_count += 1
        else:
            break

# ── ASCII 终端可视化 ──
def draw_ascii():
    """在终端画 360° 俯视图"""
    # 清屏
    print('\033[2J\033[H', end='')

    # 有效点
    valid = [(i, ranges[i]) for i in range(NUM_BINS) if ranges[i] != float('inf')]
    if not valid:
        print("等待雷达数据...")
        return

    max_r = max(d for _, d in valid) * 1.2
    max_r = max(max_r, 0.5)

    # 统计
    dists = [d for _, d in valid]
    p95 = sorted(dists)[int(len(dists) * 0.95)]

    print(f"{'='*60}")
    print(f"  S9-FSRD-V1.0 RX  激光雷达  @  {PORT}")
    print(f"{'='*60}")
    print(f"  帧率: {stats['fps']:>5.0f} fps  |  有效点: {len(valid):>5}  |  覆盖: {coverage:>3}/360")
    print(f"  最近: {min(dists):>6.2f}m  |  最远: {max(dists):>6.2f}m  |  P95: {p95:.2f}m")
    print(f"{'-'*60}")
    print()

    # ASCII 雷达图 (极坐标投影到直角)
    R = 18
    W = 55
    H = R * 2 + 1
    cx, cy = W // 2, R

    grid = [[' ' for _ in range(W)] for _ in range(H)]

    # 画圆环
    for r in range(0, R + 1, 4):
        for t_deg in range(0, 360, 5):
            t = math.radians(t_deg)
            x = int(cx + r * math.sin(t))
            y = int(cy - r * math.cos(t))
            if 0 <= x < W and 0 <= y < H:
                grid[y][x] = '·'

    # 画轴
    for y in range(H):
        grid[y][cx] = '│'
    for x in range(W):
        if grid[cy][x] == ' ':
            grid[cy][x] = '─'
    grid[cy][cx] = '◎'

    # 画数据点
    for bin_i, d in valid:
        ratio = d / max_r
        r_px = int(ratio * R)
        r_px = min(r_px, R - 1)

        angle_rad = math.radians(bin_i)
        x = int(cx + r_px * math.sin(angle_rad))
        y = int(cy - r_px * math.cos(angle_rad))

        if 0 <= x < W and 0 <= y < H:
            intensity = intensities[bin_i]
            if intensity > 200:
                ch = '█'
            elif intensity > 100:
                ch = '▓'
            elif intensity > 50:
                ch = '▒'
            else:
                ch = '░'
            grid[y][x] = ch

    # 打印
    for row in grid:
        print(''.join(row))
    print()
    print(f"  颜色: █ 强信号  ▓ 中  ▒ 弱  ░ 微弱")
    print(f"  (数据每 2 秒重置累积)")

# ── GUI 可视化 ──
def draw_gui():
    global coverage, ranges, intensities
    import matplotlib
    matplotlib.use('TkAgg')
    import matplotlib.pyplot as plt
    import numpy as np

    plt.ion()
    fig = plt.figure(figsize=(14, 7), facecolor='#1a1a2e')
    fig.canvas.manager.set_window_title('S9 LiDAR Viewer')

    ax_polar = fig.add_subplot(121, projection='polar')
    ax_polar.set_facecolor('#1a1a2e')
    ax_polar.set_theta_zero_location('N')
    ax_polar.set_theta_direction(-1)
    ax_polar.set_title('S9-FSRD-V1.0 激光雷达扫描', color='white', fontsize=14, pad=20)
    ax_polar.grid(True, alpha=0.3, color='gray')
    ax_polar.tick_params(colors='gray')
    ax_polar.set_ylim(0, 1.0)
    ax_polar.set_rlabel_position(135)
    ax_polar.yaxis.set_tick_params(color='gray')

    scatter = ax_polar.scatter([], [], c=[], s=6, cmap='viridis',
                                vmin=0, vmax=255, alpha=0.9)

    ax_info = fig.add_subplot(122)
    ax_info.set_facecolor('#1a1a2e')
    ax_info.set_xlim(0, 1)
    ax_info.set_ylim(0, 1)
    ax_info.axis('off')
    info_texts = []

    text_handles = []
    while running:
        read_serial()
        stats_update()

        if np.any(np.isfinite(ranges)):
            valid_mask = [r != float('inf') for r in ranges]
            bin_angles = [i for i, v in enumerate(valid_mask) if v]
            valid_ranges = [ranges[i] for i in bin_angles]
            valid_intens = [intensities[i] for i in bin_angles]

            if valid_ranges:
                angles_rad = np.deg2rad(bin_angles)
                scatter.set_offsets(np.column_stack([angles_rad, valid_ranges]))
                scatter.set_array(valid_intens)

                max_r = np.percentile(valid_ranges, 95) * 1.2
                max_r = max(max_r, 0.5)
                ax_polar.set_ylim(0, max_r)

                max_d = max(valid_ranges)
                min_d = min(valid_ranges)
                mean_d = np.mean(valid_ranges)
                p95 = np.percentile(valid_ranges, 95)

                lines = [
                    f'{"━"*20}',
                    f' 协议: AA55  @ 115200',
                    f' 端口: {PORT}',
                    f'{"━"*20}',
                    f' 帧率:   {stats["fps"]:.0f} fps',
                    f' 总帧数: {stats["total_frames"]}',
                    f' 总点数: {stats["total_points"]}',
                    f' 覆盖:   {coverage}/{NUM_BINS}°',
                    f'{"━"*20}',
                    f' 最小: {min_d:.3f} m',
                    f' 最大: {max_d:.3f} m',
                    f' 平均: {mean_d:.3f} m',
                    f' P95:  {p95:.3f} m',
                    f'{"━"*20}',
                    f' 颜色=信号质量',
                    f' 亮黄=强信号',
                    f' 深紫=弱信号',
                ]

                for t in text_handles:
                    t.remove()
                text_handles.clear()
                y = 0.95
                for line in lines:
                    t = ax_info.text(0.05, y, line, color='white', fontsize=11,
                                     family='monospace', verticalalignment='top',
                                     transform=ax_info.transAxes)
                    text_handles.append(t)
                    y -= 0.05

            fig.suptitle(f'S9 LiDAR  ({stats["fps"]:.0f} fps  |  {coverage}/360°)',
                         color='white', fontsize=12)
            fig.subplots_adjust(top=0.88)
            plt.pause(0.05)

        # 每 2 秒重置
        if time.time() - stats['reset_ts'] > 2.0:
            ranges[:] = [float('inf')] * NUM_BINS
            intensities[:] = [0] * NUM_BINS
            coverage = 0
            stats['reset_ts'] = time.time()

    plt.ioff()
    plt.close('all')

# ── 统计更新 ──
def stats_update():
    elapsed = time.time() - stats['log_ts']
    if elapsed >= 1.0:
        fc = stats['frame_count']
        stats['fps'] = fc / elapsed
        stats['total_frames'] += fc
        stats['frame_count'] = 0
        stats['log_ts'] = time.time()

# ── CLI 主循环 ──
def cli_loop():
    global coverage, ranges, intensities
    while running:
        # 快速读取
        for _ in range(5):
            read_serial()
        stats_update()

        # 每秒刷新一次屏幕
        now = time.time()
        if now - stats.get('draw_ts', 0) >= 0.8:
            draw_ascii()
            stats['draw_ts'] = now

        # 每 2 秒重置累积
        if now - stats['reset_ts'] > 2.0:
            ranges[:] = [float('inf')] * NUM_BINS
            intensities[:] = [0] * NUM_BINS
            coverage = 0
            stats['reset_ts'] = now

# ── 启动 ──
print(f"🔗 连接 {PORT} @ 115200...")
try:
    if CLI_MODE:
        cli_loop()
    else:
        draw_gui()
except KeyboardInterrupt:
    pass
finally:
    running = False
    ser.close()
    print("\n✅ 已停止")
