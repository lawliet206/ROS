#!/bin/bash
# ============================================================
# sim_follow_radar.sh — 毫米波雷达跟随仿真一键启动
# ============================================================
# 启动 Gazebo 仿真 + sim_radar 模拟 LD2402
#
# 用法:
#   bash ~/ROS/src/robot_sim/scripts/sim_follow_radar.sh
# ============================================================
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
WS_DIR="$HOME/ROS"

if [ ! -f "$WS_DIR/devel/setup.bash" ]; then
    echo "ERROR: 找不到 $WS_DIR/devel/setup.bash, 请先 catkin_make"
    exit 1
fi
source "$WS_DIR/devel/setup.bash"

echo "=== 清理旧 Gazebo 进程 ==="
killall gzserver gzclient 2>/dev/null || true
sleep 1

# GPU 检测
if ! glxinfo 2>/dev/null | grep -qi "direct rendering: Yes" || \
   glxinfo 2>/dev/null | grep -qi "llvmpipe"; then
    echo "=== 启用 Mesa 软件渲染 ==="
    export LIBGL_ALWAYS_SOFTWARE=1
    export GALLIUM_DRIVER=llvmpipe
    if [ ! -f /usr/lib/x86_64-linux-gnu/dri/llvmpipe_dri.so ]; then
        sudo apt install -y mesa-utils libgl1-mesa-dri libgl1-mesa-glx
    fi
fi

cd "$WS_DIR"

echo "=== 启动仿真 ==="
roslaunch robot_sim simulation.launch gui:=true paused:=false &
SIM_PID=$!
sleep 5

echo "=== 启动 sim_radar (模拟 LD2402) ==="
rosrun robot_sim sim_radar.py &
RADAR_PID=$!

echo ""
echo "============================================"
echo "  毫米波雷达仿真已启动!"
echo "  Gazebo 里拖物体到车前, sim_radar 会输出距离"
echo "  查看话题: rostopic echo /radar_distance"
echo "  按 Ctrl-C 停止"
echo "============================================"

trap "kill $SIM_PID $RADAR_PID 2>/dev/null; exit 0" INT TERM
wait
