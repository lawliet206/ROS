#!/bin/bash
# ============================================================
# nav_start.sh — move_base 多点导航一键启动
# ============================================================
# 使用:
#   bash nav_start.sh <地图文件路径>
#
# 示例:
#   bash nav_start.sh ~/maps/my_map.yaml
#
# 前提:
#   1. 地图已建好 (用 slam_start.sh 建图后 map_saver 保存)
#   2. J1900 上 rosserial 已连接 ESP32 (自动连接)
#   3. 激光雷达已连接
#
# 启动后发送导航目标:
#   方式A: RViz 中用 "2D Nav Goal" 点击
#   方式B: rosrun robot_bringup send_goals.py _goals:="[(1,2,0),(3,4,1.57)]"
# ============================================================

set -e

# ---- 地图文件 ----
MAP_FILE="${1}"
shift  # 移除第一个参数, 剩余参数传给 roslaunch
if [ -z "${MAP_FILE}" ]; then
    echo "用法: bash nav_start.sh <地图.yaml>"
    echo "示例: bash nav_start.sh ~/maps/my_map.yaml"
    exit 1
fi

if [ ! -f "${MAP_FILE}" ]; then
    echo "[ERROR] 地图文件不存在: ${MAP_FILE}"
    exit 1
fi

echo "========================================"
echo "  move_base 多点导航一键启动"
echo "  地图: ${MAP_FILE}"
echo "========================================"

# ---- 配置 ----
ROS_WS="$HOME/ROS"

MY_IP=$(hostname -I | awk '{print $1}')
export ROS_MASTER_URI="http://${MY_IP}:11311"
export ROS_IP="${MY_IP}"

source /opt/ros/noetic/setup.bash
source "${ROS_WS}/devel/setup.bash" 2>/dev/null || {
    echo "[ERROR] 工作空间未编译: ${ROS_WS}"
    exit 1
}

# 确保 roscore
if ! rostopic list > /dev/null 2>&1; then
    echo "[INFO] 启动 roscore..."
    roscore &
    sleep 3
fi

echo "[INFO] 本机 IP: ${MY_IP}"

roslaunch robot_bringup navigation.launch \
    map_file:="${MAP_FILE}" \
    start_lidar:=false \
    "$@"
