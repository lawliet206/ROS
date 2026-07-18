#!/usr/bin/env python3
"""
ld2402_calibrate.py — LD2402 背景噪声学习 + 配置测试
====================================================
功能: 学习无人背景噪声特征, 用于软件滤除
"""
import serial
import time
import math
from collections import deque

PORT = 'COM6'
LEARN_SECONDS = 10  # 学习时长 (秒)


def main():
    ser = serial.Serial(PORT, 115200, timeout=0.1)
    ser.reset_input_buffer()
    time.sleep(2)

    print('=' * 50)
    print('LD2402 背景噪声学习工具')
    print('=' * 50)
    print('\n请确保雷达前方无人/无移动物体')
    print('正在收集静置数据 %d 秒...' % LEARN_SECONDS)

    distances = []
    buf = b''
    start = time.time()

    while time.time() - start < LEARN_SECONDS:
        raw = ser.read(256)
        if raw:
            buf += raw
            while b'\n' in buf:
                line, buf = buf.split(b'\n', 1)
                try:
                    text = line.decode('ascii', errors='ignore').strip()
                    if text.startswith('distance:'):
                        val = float(text.split('distance:')[-1].strip())
                        distances.append(val / 100.0)
                except:
                    pass

        elapsed = time.time() - start
        if len(distances) > 0:
            print('\r  已收集 %d 个样本, 当前: %.2fm  (%.0fs)' %
                  (len(distances), distances[-1], elapsed), end='', flush=True)
        time.sleep(0.05)

    print('\n\n=== 背景噪声统计 ===')
    if len(distances) < 10:
        print('样本太少 (%d 个), 请增加学习时间' % len(distances))
        ser.close()
        return

    avg = sum(distances) / len(distances)
    var = sum((x - avg) ** 2 for x in distances) / len(distances)
    std = math.sqrt(var)
    min_d = min(distances)
    max_d = max(distances)
    range_d = max_d - min_d

    print('  样本数:  %d' % len(distances))
    print('  平均值:  %.3f m' % avg)
    print('  标准差:  %.3f m' % std)
    print('  最小值:  %.3f m' % min_d)
    print('  最大值:  %.3f m' % max_d)
    print('  极差:    %.3f m' % range_d)

    print('\n=== 建议软件阈值 ===')
    suggest_var = var * 2
    suggest_range = range_d * 2
    print('  confirm_var_thresh: %.4f  (方差阈值)' % suggest_var)
    print('  max_range_allowed:  %.2f   (最大允许极差)' % suggest_range)
    print('  confirm_samples:    10     (稳定判定窗口)')
    print('  confirm_duration:   1.0    (确认时间, 秒)')
    print()

    # 测试: 模拟滤波效果
    print('=== 模拟滤波效果 (前50个样本) ===')
    window = deque(maxlen=10)
    for i, d in enumerate(distances[:50]):
        window.append(d)
        if len(window) >= 10:
            m = sum(window) / len(window)
            v = sum((x - m) ** 2 for x in window) / len(window)
            r = max(window) - min(window)
            ok = v < suggest_var and r < suggest_range
            print('  #%d  %.2fm  var=%.4f  range=%.2f  %s' %
                  (i, d, v, r, 'OK' if ok else 'NOISE'))

    ser.close()


if __name__ == '__main__':
    main()
