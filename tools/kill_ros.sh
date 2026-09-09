#!/bin/bash
# ============================================================
# kill_ros.sh — 关闭所有 ROS 相关进程
# ============================================================
echo "=== 关闭所有 ROS 进程 ==="

# Gazebo
killall -9 gzserver gzclient 2>/dev/null; echo "  ✓ Gazebo"

# ROS core
killall -9 roscore rosmaster rosout 2>/dev/null; echo "  ✓ roscore"

# 导航节点
killall -9 amcl move_base 2>/dev/null
killall -9 map_server gmapping 2>/dev/null

# 机器人节点
killall -9 robot_state_publisher ekf_localization_node 2>/dev/null
killall -9 rviz rqt 2>/dev/null

# Python ROS 节点 (进程名匹配)
pkill -9 -f "roslaunch" 2>/dev/null
pkill -9 -f "__name:=" 2>/dev/null
pkill -9 -f "rosserial_python" 2>/dev/null
pkill -9 -f "s9_lidar" 2>/dev/null
pkill -9 -f "laser_follower" 2>/dev/null
pkill -9 -f "send_goals" 2>/dev/null
echo "  ✓ ROS nodes"

sleep 1
echo "=== 完成 ==="
