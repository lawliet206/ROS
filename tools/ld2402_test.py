#!/usr/bin/env python3
"""
ld2402_test.py — LD2402 雷达检测测试 (v2)
===========================================
纯 ASCII 解析版——LD2402 输出 "distance:233\r\n" 格式

用法:
  python3 ld2402_test.py [/dev/ttyUSB0]
"""
import serial
import time
import sys


def main():
    port = sys.argv[1] if len(sys.argv) > 1 else "/dev/ttyUSB0"

    ser = serial.Serial(port, 115200, timeout=0.1)
    print(f"[OK] {port} @ 115200")
    print("等待雷达数据... (30-60s 初始化)")
    print("-" * 50)

    buf = b""
    distance = -1.0
    last_detect = 0.0
    timeout = 3.0

    while True:
        try:
            raw = ser.read(256)
        except Exception:
            continue

        if not raw:
            continue
        buf += raw

        # 按行分割
        while b"\n" in buf:
            line, buf = buf.split(b"\n", 1)
            line = line.strip()

            try:
                text = line.decode("ascii", errors="ignore")
            except Exception:
                continue

            if text.startswith("distance:"):
                try:
                    val = float(text.split("distance:")[-1].strip())
                    distance = val / 100.0
                    last_detect = time.time()
                except (ValueError, IndexError):
                    pass
            elif text == "OFF":
                distance = -1.0

        # 超时清除
        now = time.time()
        presence = (now - last_detect) < timeout

        d = f"{distance:6.2f}" if presence else "  -   "
        print(f"\r距离: {d}m  {'有人' if presence else '无人'}  ", end="", flush=True)


if __name__ == "__main__":
    main()
