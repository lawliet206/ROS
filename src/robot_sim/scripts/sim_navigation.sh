#!/bin/bash
# ============================================================
# sim_navigation.sh — Gazebo 导航仿真一键启动
# ============================================================
# 用法:
#   bash ~/ROS/src/robot_sim/scripts/sim_navigation.sh <map_file>
#   bash ~/ROS/src/robot_sim/scripts/sim_navigation.sh ~/maps/sim_map.yaml
#   bash ~/ROS/src/robot_sim/scripts/sim_navigation.sh ~/maps/sim_map.yaml nogui
# ============================================================
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
WS_DIR="$HOME/ROS"

MAP_FILE="${1:-$HOME/maps/sim_map.yaml}"
MODE="${2:-gui}"

if [ ! -f "$MAP_FILE" ]; then
    echo "ERROR: 地图文件不存在: $MAP_FILE"
    echo "请先建图: bash ~/ROS/src/robot_sim/scripts/sim_slam.sh"
    echo "或指定地图: bash $0 /path/to/map.yaml"
    exit 1
fi

# ---------- 检查 ROS 环境 ----------
if [ ! -f "$WS_DIR/devel/setup.bash" ]; then
    echo "ERROR: 找不到 $WS_DIR/devel/setup.bash, 请先 catkin_make"
    exit 1
fi
source "$WS_DIR/devel/setup.bash"

# ---------- 杀死旧进程 ----------
echo "=== 清理旧 Gazebo 进程 ==="
killall gzserver gzclient 2>/dev/null || true
sleep 1

# ---------- GPU / 软件渲染检测 ----------
if ! glxinfo 2>/dev/null | grep -qi "direct rendering: Yes" || \
   glxinfo 2>/dev/null | grep -qi "llvmpipe"; then
    echo "=== 未检测到硬件 3D 加速, 启用 Mesa 软件渲染 ==="
    export LIBGL_ALWAYS_SOFTWARE=1
    export GALLIUM_DRIVER=llvmpipe

    if [ ! -f /usr/lib/x86_64-linux-gnu/dri/llvmpipe_dri.so ]; then
        echo "正在安装 Mesa 软件渲染驱动..."
        sudo apt install -y mesa-utils libgl1-mesa-dri libgl1-mesa-glx
    fi
fi

# ---------- 启动 ----------
cd "$WS_DIR"

GUI_FLAG="true"
[ "$MODE" = "nogui" ] && GUI_FLAG="false"

roslaunch robot_sim sim_navigation.launch \
    map_file:="$MAP_FILE" \
    gui:="$GUI_FLAG" \
    paused:=false
