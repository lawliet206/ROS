#!/bin/bash
# ============================================================
# kill_ros.sh — 关闭所有 ROS 相关进程
# ============================================================
echo "=== 关闭所有 ROS 进程 ==="

# Gazebo
killall -9 gzserver gzclient 2>/dev/null && echo "  ✓ Gazebo"

# ROS core
killall -9 roscore rosmaster 2>/dev/null && echo "  ✓ roscore"

# Python ROS 节点
pkill -9 -f "roslaunch" 2>/dev/null
pkill -9 -f "__name:=" 2>/dev/null
pkill -9 -f "rosserial_python" 2>/dev/null
echo "  ✓ ROS nodes"

sleep 1
echo "=== 完成 ==="
