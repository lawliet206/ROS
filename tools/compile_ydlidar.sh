#!/bin/bash
# ============================================================
# compile_ydlidar.sh — 在 J1900(x86_64)上编译激光雷达驱动
# ============================================================
# 用法: bash compile_ydlidar.sh
# ============================================================
set -e

WS_DIR="$HOME/catkin_ws"
ROS_DISTRO="noetic"

echo "=== 编译 YDLIDAR 驱动 (ARM64) ==="

# ---------- 1. 确保依赖 ----------
sudo apt update
sudo apt install -y \
    git cmake build-essential \
    ros-$ROS_DISTRO-rosbash \
    libboost-system-dev \
    libboost-thread-dev \
    python3-serial

# ---------- 2. 创建/进入工作空间 ----------
mkdir -p "$WS_DIR/src"
cd "$WS_DIR"
source /opt/ros/$ROS_DISTRO/setup.bash
catkin_make -DCMAKE_BUILD_TYPE=Release
source devel/setup.bash

# ---------- 3. 下载驱动 ----------
cd "$WS_DIR/src"
if [ -d "ydlidar_ros" ]; then
    echo "[存在] 更新 ydlidar_ros ..."
    cd ydlidar_ros && git pull && cd ..
else
    echo "[下载] ydlidar_ros ..."
    git clone https://github.com/YDLIDAR/ydlidar_ros.git
fi

# ---------- 4. 编译 ----------
cd "$WS_DIR"
catkin_make -DCMAKE_BUILD_TYPE=Release

# ---------- 5. 给串口权限 ----------
echo "=== 添加串口权限 ==="
sudo usermod -a -G dialout $USER
sudo chmod 666 /dev/ttyUSB0 2>/dev/null || true

# ---------- 6. 检测 F2 雷达 ----------
echo ""
echo "=== 检测雷达 ==="
ls /dev/ttyU* 2>/dev/null || echo "未检测到 /dev/ttyUSB*，插上 USB 转 TTL 后重试"
echo ""

# 看有哪些 launch 文件
echo "可用 launch 文件:"
ls "$WS_DIR/src/ydlidar_ros/launch/" 2>/dev/null || echo "launch 目录未找到, 检查 ydlidar_ros 是否下载完全"

echo ""
echo "启动命令 (F2 用):"
echo "  source ~/catkin_ws/devel/setup.bash"
echo "  roslaunch ydlidar_ros F2.launch"
echo ""
echo "如果 F2.launch 不存在, 用:"
echo "  ls ~/catkin_ws/src/ydlidar_ros/launch/"
echo "看有什么 launch 文件, 选对应雷达型号的"
echo ""
echo "编译完成!"
