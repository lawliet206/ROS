#!/bin/bash
# ============================================================
# slam_start.sh — SLAM 建图一键启动
# ============================================================
# 使用:
#   bash slam_start.sh
#
# 前提:
#   1. J1900 上已启动 serial_bridge:
#      rosrun robot_bringup serial_bridge.py _port:=/dev/ttyUSB0
#   2. 激光雷达已连接 J1900
#   3. PC 与 J1900 在同一 WiFi 网络下
#
# 建图完成后保存地图:
#   rosrun map_server map_saver -f ~/maps/my_map
# ============================================================

set -e

echo "========================================"
echo "  SLAM 建图一键启动"
echo "========================================"

# ---- 配置 ----
ROS_WS="$HOME/ROS"

# 获取本机 IP (树莓派需要设 ROS_MASTER_URI=http://本机IP:11311)
MY_IP=$(hostname -I | awk '{print $1}')
export ROS_MASTER_URI="http://${MY_IP}:11311"
export ROS_IP="${MY_IP}"

source /opt/ros/noetic/setup.bash
source "${ROS_WS}/devel/setup.bash" 2>/dev/null || {
    echo "[ERROR] 工作空间未编译: ${ROS_WS}"
    echo "请先: cd ${ROS_WS} && catkin_make"
    exit 1
}

# 确保 roscore 运行
if ! rostopic list > /dev/null 2>&1; then
    echo "[INFO] 启动 roscore..."
    roscore &
    sleep 3
fi

echo ""
echo "[INFO] 本机 IP: ${MY_IP}"
echo "[INFO] J1900 需设置: export ROS_MASTER_URI=http://${MY_IP}:11311"
echo ""
echo "========================================"
echo "  操作提示:"
echo "   键盘遥控: rosrun teleop_twist_keyboard teleop_twist_keyboard.py"
echo "   查看地图: rviz"
echo "   保存地图: rosrun map_server map_saver -f ~/maps/my_map"
echo "========================================"
echo ""

roslaunch robot_bringup slam.launch start_lidar:=false "$@"
