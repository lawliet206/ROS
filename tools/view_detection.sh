#!/bin/bash
# ============================================================
# view_detection.sh — PC 端查看小车摄像头图像 + YOLO 检测结果
# ============================================================
# 功能: 启动 person_detector (YOLO 检测) + 显示窗口
#       窗口实时显示小车摄像头画面 + 人体检测框
#
# 前提:
#   1. J1900 已推流 /image_raw/compressed (usb_cam + republish)
#   2. PC 是 ROS Master
#
# 用法:
#   bash ~/ROS/tools/view_detection.sh
#   退出: Ctrl-C
# ============================================================
set -e
source /opt/ros/noetic/setup.bash
source ~/ROS/devel/setup.bash
export ROS_MASTER_URI=http://10.80.147.11:11311
export ROS_IP=10.80.147.11

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# 清理残留 (精确匹配 python 脚本名, 杀 rosrun 包装的子进程)
for p in $(pgrep -f "[p]erson_detector.py|[p]erson_viewer.py"); do
  kill -9 "$p" 2>/dev/null
done
sleep 1

# 1. 启动 person_detector (YOLO 检测, 发布 /person_angle /person_visible /person_overlay)
echo "=== 启动 YOLO 人体检测 ==="
rosrun robot_bringup person_detector.py &
DET_PID=$!
sleep 3

# 2. 启动显示窗口
echo "=== 打开显示窗口 ==="
python3 "$SCRIPT_DIR/person_viewer.py" &
VIEW_PID=$!

trap "kill $DET_PID $VIEW_PID 2>/dev/null; echo ''; echo '已退出'; exit 0" INT TERM
echo ""
echo "============================================"
echo "  小车摄像头 + YOLO 检测 已启动!"
echo "  窗口标题: 小车摄像头 - YOLO 人体检测"
echo "  摄像头前有人时显示绿色检测框 + 红色中心线"
echo "  按 Ctrl-C 退出"
echo "============================================"
wait
