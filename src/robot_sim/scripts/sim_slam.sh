#!/bin/bash
# ============================================================
# sim_slam.sh — Gazebo SLAM 仿真一键启动
# ============================================================
# 自动检测环境并配置 Mesa 软件渲染 (VM 无 GPU 时也能跑)
#
# 用法:
#   bash ~/ROS/src/robot_sim/scripts/sim_slam.sh
#   bash ~/ROS/src/robot_sim/scripts/sim_slam.sh nogui   # 无界面 + RViz
# ============================================================
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
WS_DIR="$HOME/ROS"

# ---------- 解析参数 ----------
MODE="${1:-gui}"          # gui | nogui

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
HAS_GL=false
if glxinfo 2>/dev/null | grep -qi "direct rendering: Yes"; then
    if ! glxinfo 2>/dev/null | grep -qi "llvmpipe"; then
        HAS_GL=true
    fi
fi

if [ "$HAS_GL" = false ]; then
    echo "=== 未检测到硬件 3D 加速, 启用 Mesa 软件渲染 ==="
    export LIBGL_ALWAYS_SOFTWARE=1
    export GALLIUM_DRIVER=llvmpipe

    # 确保 Mesa 已安装
    if [ ! -f /usr/lib/x86_64-linux-gnu/dri/llvmpipe_dri.so ]; then
        echo "正在安装 Mesa 软件渲染驱动..."
        sudo apt install -y mesa-utils libgl1-mesa-dri libgl1-mesa-glx
    fi
else
    echo "=== 检测到硬件 GPU, 使用硬件渲染 ==="
fi

# ---------- 启动 ----------
cd "$WS_DIR"

if [ "$MODE" = "nogui" ]; then
    echo "=== 模式: 无界面 + RViz (虚拟机推荐) ==="
    echo "请在另一个终端运行: rviz"
    roslaunch robot_sim sim_slam.launch paused:=false gui:=false
else
    echo "=== 模式: Gazebo GUI ==="
    echo "注意: 软件渲染会比较慢, 但能看到 Gazebo 界面"
    roslaunch robot_sim sim_slam.launch paused:=false gui:=true
fi
