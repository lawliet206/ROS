#!/bin/bash
# ============================================================
# slam_start.sh — SLAM 建图一键启动
# ============================================================
# 使用:
#   bash slam_start.sh
#
# 前提:
#   1. J1900 上 rosserial 已连接 ESP32 (自动连接)
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

# 获取本机 IP (优先取 wlan0/eth0 对应的 IP, 跳过 docker/lo/虚拟网卡)
get_my_ip() {
    for iface in wlan0 eth0 enp0s3 enp0s8; do
        local ip=$(ip -4 addr show "$iface" 2>/dev/null | grep -oP '(?<=inet\s)\d+(\.\d+){3}')
        if [ -n "$ip" ]; then
            echo "$ip"
            return
        fi
    done
    # fallback: hostname -I 第一个
    hostname -I | awk '{print $1}'
}
MY_IP=$(get_my_ip)
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

# 等待 J1900 端关键话题 (超时 60s)
echo "[WAIT] 等待 J1900 话题就绪..."
for topic in /odom /scan; do
    waited=0
    while ! rostopic list 2>/dev/null | grep -qx "$topic"; do
        sleep 1; waited=$((waited + 1))
        if [ $waited -ge 60 ]; then
            echo "[ERROR] 超时: 话题 $topic 未出现. 请确认 J1900 上已启动:"
            echo "  rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB1 _baud:=460800"
            echo "  rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB0"
            exit 1
        fi
        echo -n "."
    done
    echo " OK ($topic)"
done

echo ""
echo "========================================"
echo "  操作提示:"
echo "   键盘遥控: rosrun teleop_twist_keyboard teleop_twist_keyboard.py"
echo "   查看地图: rviz"
echo "   保存地图: rosrun map_server map_saver -f ~/maps/my_map"
echo "========================================"
echo ""

roslaunch robot_bringup slam.launch start_lidar:=false "$@"
