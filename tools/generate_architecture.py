#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
生成系统架构图 docs/system_architecture.png
==========================================
用法:
  python tools/generate_architecture.py

依赖: pip install matplotlib
输出: docs/system_architecture.png (300 DPI, 适合 GitHub 展示)

架构分层 (三机分布式):
  PC  (ROS Master): SLAM/EKF/AMCL/move_base/TEB/视觉检测
  J1900 (车载): 雷达驱动 / 摄像头采集压缩
  ESP32 (下位机): 电机 PID / 编码器里程计 / IMU
数据流: LiDAR→/scan→SLAM→map→AMCL→move_base→/cmd_vel→ESP32→PWM→Motors
"""
import os

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch, FancyArrowPatch

plt.rcParams["font.sans-serif"] = ["Microsoft YaHei", "SimHei", "Noto Sans CJK SC", "DejaVu Sans"]
plt.rcParams["axes.unicode_minus"] = False

OUT = os.path.join(os.path.dirname(__file__), "..", "docs", "system_architecture.png")

fig, ax = plt.subplots(figsize=(13, 9), dpi=300)
ax.set_xlim(0, 13)
ax.set_ylim(0, 9)
ax.axis("off")


def box(x, y, w, h, text, fc, ec, fs=11, tc="black", lw=1.5, sub=None):
    """画圆角盒子, 支持多行文字与子项列表."""
    p = FancyBboxPatch(
        (x, y), w, h, boxstyle="round,pad=0.08,rounding_size=0.15",
        fc=fc, ec=ec, lw=lw, mutation_scale=1.0,
    )
    ax.add_patch(p)
    if sub:
        ax.text(x + w / 2, y + h - 0.35, text, ha="center", va="center",
                fontsize=fs + 1, fontweight="bold", color=tc)
        ax.text(x + w / 2, y + h / 2 - 0.15, sub, ha="center", va="center",
                fontsize=fs - 1.5, color=tc, linespacing=1.6)
    else:
        ax.text(x + w / 2, y + h / 2, text, ha="center", va="center",
                fontsize=fs, fontweight="bold", color=tc)
    return (x + w / 2, y + h)


def arrow(x1, y1, x2, y2, text="", color="#555", ls="-", lw=1.8, fs=9, tdx: float = 0, tdy: float = 0.15):
    a = FancyArrowPatch(
        (x1, y1), (x2, y2), arrowstyle="-|>", mutation_scale=16,
        color=color, lw=lw, linestyle=ls, shrinkA=4, shrinkB=4,
    )
    ax.add_patch(a)
    if text:
        ax.text((x1 + x2) / 2 + tdx, (y1 + y2) / 2 + tdy, text,
                ha="center", va="center", fontsize=fs, color=color)

# ============ 分层背景 ============
ax.text(6.5, 8.55, "三机分布式架构", ha="center", fontsize=15, fontweight="bold", color="#333")

# PC 层
ax.add_patch(FancyBboxPatch((0.6, 5.6), 11.8, 2.3, boxstyle="round,pad=0.1",
                            fc="#E8F1FB", ec="#5B9BD5", lw=1.2, linestyle="--"))
ax.text(0.9, 7.6, "PC  ·  Ubuntu 20.04 · ROS Noetic (ROS Master)", ha="left",
        fontsize=10.5, color="#1F4E79", fontweight="bold")

# PC 内节点
box(0.9, 5.8, 2.1, 1.35, "SLAM 建图", "#DEEBF7", "#5B9BD5", sub="gmapping\n订阅 /scan_deskewed")
box(3.2, 5.8, 2.1, 1.35, "EKF 融合", "#DEEBF7", "#5B9BD5", sub="robot_localization\nodom + imu")
box(5.5, 5.8, 2.1, 1.35, "定位", "#DEEBF7", "#5B9BD5", sub="AMCL\nmap + scan")
box(7.8, 5.8, 2.3, 1.35, "导航", "#DEEBF7", "#5B9BD5", sub="move_base + TEB\n全局 + 局部规划")
box(10.3, 5.8, 2.0, 1.35, "视觉跟随", "#DEEBF7", "#5B9BD5", sub="YOLOv8n 检测\n视觉定方向")

# J1900 层
ax.add_patch(FancyBboxPatch((0.6, 3.1), 11.8, 1.9, boxstyle="round,pad=0.1",
                            fc="#E2EFDA", ec="#70AD47", lw=1.2, linestyle="--"))
ax.text(0.9, 4.75, "J1900 车载工控机  ·  Ubuntu 20.04 · ROS Noetic", ha="left",
        fontsize=10.5, color="#375623", fontweight="bold")

box(1.0, 3.3, 3.4, 1.15, "雷达驱动", "#E2F0D9", "#70AD47", sub="s9_lidar_driver.py (AA55 协议, 360°)\nserial → /scan", fs=10)
box(5.0, 3.3, 3.4, 1.15, "图像采集", "#E2F0D9", "#70AD47", sub="usb_cam + republish\nUSB 摄像头 → /image_raw/compressed", fs=10)
box(9.0, 3.3, 3.2, 1.15, "车载控制", "#E2F0D9", "#70AD47", sub="rosserial_python\nESP32 双向通信", fs=10)

# ESP32 层
ax.add_patch(FancyBboxPatch((0.6, 0.6), 11.8, 1.7, boxstyle="round,pad=0.1",
                            fc="#FBE5D6", ec="#ED7D31", lw=1.2, linestyle="--"))
ax.text(0.9, 2.05, "ESP32 下位机  ·  Arduino (esp32_firmware)", ha="left",
        fontsize=10.5, color="#833C00", fontweight="bold")

box(1.0, 0.8, 3.2, 1.0, "电机控制", "#FCE4D6", "#ED7D31", sub="双路 PID + PWM → TB6612FNG", fs=10)
box(5.0, 0.8, 3.2, 1.0, "编码器里程计", "#FCE4D6", "#ED7D31", sub="PCNT 硬件计数 → /odom", fs=10)
box(9.0, 0.8, 3.2, 1.0, "IMU 姿态", "#FCE4D6", "#ED7D31", sub="MPU6050 (I2C) → /imu", fs=10)

# ============ 外部设备 ============
# 左侧: LiDAR / Camera
box(0.0, 3.3, 0.0, 0.0, "", "#FFFFFF", "#FFFFFF")  # 占位保持布局
ax.text(-0.05, 5.9, "雷达\nLiDAR", ha="center", va="center", fontsize=10,
        fontweight="bold", color="#333",
        bbox=dict(boxstyle="round,pad=0.35", fc="#FFF2CC", ec="#BF9000", lw=1.2))
ax.text(-0.05, 3.4, "摄像头\nCamera", ha="center", va="center", fontsize=10,
        fontweight="bold", color="#333",
        bbox=dict(boxstyle="round,pad=0.35", fc="#FFF2CC", ec="#BF9000", lw=1.2))

# 右侧: 电机 / 编码器 / IMU
ax.text(13.05, 5.9, "地图/目标\nMap/Goal", ha="center", va="center", fontsize=10,
        fontweight="bold", color="#333",
        bbox=dict(boxstyle="round,pad=0.35", fc="#FFF2CC", ec="#BF9000", lw=1.2))
ax.text(13.05, 1.8, "电机×2\nMotors", ha="center", va="center", fontsize=10,
        fontweight="bold", color="#333",
        bbox=dict(boxstyle="round,pad=0.35", fc="#FFF2CC", ec="#BF9000", lw=1.2))

# ============ 连线 ============
# PC ↔ J1900 (WiFi)
arrow(6.5, 5.6, 6.5, 5.0, "WiFi (ROS Topics / rosbridge)", tdy=0.22)
# J1900 ↔ ESP32 (USB 串口)
arrow(6.5, 3.1, 6.5, 2.3, "USB 串口 (rosserial, 115200)", tdy=0.22)
# J1900 外部设备 (USB)
arrow(1.2, 3.1, 0.9, 4.2, "USB", tdx=-0.1)
arrow(0.95, 3.1, 0.95, 3.4, "USB", tdx=0.1, tdy=-0.15)
# ESP32 外部设备
arrow(2.6, 0.8, 2.6, 0.55, "PWM", tdy=-0.22)
arrow(9.5, 0.8, 9.5, 0.55, "I2C", tdy=-0.22)
arrow(13.0, 1.3, 12.7, 0.9, "PWM", tdx=-0.1, tdy=-0.2)

# 数据流标注 (底部)
ax.text(6.5, 0.28, "数据流:  LiDAR → /scan → SLAM → map → AMCL → move_base → /cmd_vel → ESP32 → PID → Motors",
        ha="center", fontsize=9.5, color="#555")

plt.tight_layout()
os.makedirs(os.path.dirname(OUT), exist_ok=True)
plt.savefig(OUT, bbox_inches="tight", facecolor="white")
print(f"[OK] 已生成 {os.path.abspath(OUT)}")