#!/bin/bash
# 启动 PC roscore (Master) — 持久化
source /opt/ros/noetic/setup.bash
export ROS_MASTER_URI=http://10.80.147.11:11311
export ROS_IP=10.80.147.11

for p in $(pgrep -f "[r]osmaster"); do kill -9 "$p" 2>/dev/null; done
sleep 1
setsid roscore > /home/lawliet/ROS/.roscore.log 2>&1 < /dev/null &
sleep 5
ss -tlnp 2>/dev/null | grep 11311 && echo "=== roscore OK ===" && rostopic list 2>&1 | head -2
