#!/bin/bash
# PC entry point for real-robot SLAM, patrol, and person following.

MODE="${1:-help}"
[ "$#" -gt 0 ] && shift

ROS_WS="${ROS_WS:-$HOME/ROS}"
PC_IP="${PC_IP:-$(hostname -I | awk '{print $1}')}"
J1900_HOST="${J1900_HOST:-lawliet@lawliet.local}"
REMOTE_ROS_WS="${REMOTE_ROS_WS:-/home/lawliet/ROS}"
REMOTE_SCRIPT="${REMOTE_ROS_WS}/src/robot_bringup/scripts/j1900_start.sh"
START_TIMEOUT="${START_TIMEOUT:-60}"
LOG_DIR="${ROS_WS}/.robot_logs"

source /opt/ros/noetic/setup.bash
if [ ! -f "${ROS_WS}/devel/setup.bash" ]; then
  echo "[ERROR] PC 工作空间未编译: ${ROS_WS}"
  echo "        执行: cd ${ROS_WS} && catkin_make"
  exit 1
fi
source "${ROS_WS}/devel/setup.bash"
set -u

export ROS_MASTER_URI="http://${PC_IP}:11311"
export ROS_IP="${PC_IP}"
mkdir -p "${LOG_DIR}"

print_help() {
  cat <<EOF
用法:
  bash $0 slam [roslaunch参数...]
  bash $0 patrol [地图.yaml] [巡航点.yaml]
  bash $0 follow [roslaunch参数...]
  bash $0 stop
  bash $0 status

默认文件:
  地图:   ~/maps/lab_map.yaml
  巡航点: ~/ROS/src/robot_bringup/config/patrol_goals.yaml

环境覆盖:
  PC_IP=${PC_IP}
  J1900_HOST=${J1900_HOST}
EOF
}

ensure_roscore() {
  if timeout --kill-after=1 3 rosnode list >/dev/null 2>&1; then
    return 0
  fi

  echo "[INFO] 启动 PC roscore (${PC_IP}:11311)..."
  setsid roscore >"${LOG_DIR}/roscore.log" 2>&1 < /dev/null &
  local waited=0
  while [ "${waited}" -lt 15 ]; do
    if timeout --kill-after=1 2 rosnode list >/dev/null 2>&1; then
      echo "[OK] roscore 已启动"
      return 0
    fi
    sleep 1
    waited=$((waited + 1))
  done
  echo "[ERROR] roscore 启动失败，日志: ${LOG_DIR}/roscore.log"
  tail -n 20 "${LOG_DIR}/roscore.log" 2>/dev/null || true
  return 1
}

check_ssh() {
  if ! ssh -o BatchMode=yes -o ConnectTimeout=5 "${J1900_HOST}" true; then
    echo "[ERROR] 无法免密 SSH 到 ${J1900_HOST}"
    echo "        先执行: ssh-copy-id ${J1900_HOST}"
    return 1
  fi
}

remote_hardware() {
  local mode="$1"
  ssh -o BatchMode=yes -o ConnectTimeout=5 "${J1900_HOST}" \
    "PC_MASTER='${PC_IP}' ROS_WS='${REMOTE_ROS_WS}' bash '${REMOTE_SCRIPT}' '${mode}'"
}

topic_has_message() {
  local topic="$1"
  local timeout_sec="${2:-2}"
  timeout --kill-after=1 "${timeout_sec}" rostopic echo -n 1 "${topic}" >/dev/null 2>&1
}

wait_for_topic() {
  local topic="$1"
  local timeout_sec="${2:-${START_TIMEOUT}}"
  local waited=0
  echo "[WAIT] ${topic}..."
  while [ "${waited}" -lt "${timeout_sec}" ]; do
    if topic_has_message "${topic}" 2; then
      echo "[OK] ${topic} 有数据"
      return 0
    fi
    waited=$((waited + 2))
  done
  echo "[ERROR] ${topic} 在 ${timeout_sec}s 内没有数据"
  return 1
}

publish_stop() {
  if timeout --kill-after=1 2 rosnode list >/dev/null 2>&1; then
    timeout --kill-after=1 2 rostopic pub -r 20 /cmd_vel geometry_msgs/Twist '{}' \
      >/dev/null 2>&1 || true
  fi
}

stop_local_pattern() {
  local pattern="$1"
  if pkill -TERM -f "${pattern}" 2>/dev/null; then
    sleep 0.2
    pkill -KILL -f "${pattern}" 2>/dev/null || true
  fi
}

stop_pc_modes() {
  local active_nodes node
  if timeout --kill-after=1 2 rosnode list >/dev/null 2>&1; then
    publish_stop
    active_nodes=$(rosnode list 2>/dev/null || true)
    for node in \
      /teleop_twist_keyboard /slam_gmapping /scan_deskew /scan_deskew_relay \
      /ekf_localization /robot_state_publisher /map_server /amcl /move_base \
      /nav_cmd_vel_relay /send_goals /person_detector /follower; do
      if printf '%s\n' "${active_nodes}" | grep -qx "${node}"; then
        rosnode kill "${node}" >/dev/null 2>&1 || true
      fi
    done
  fi

  stop_local_pattern "roslaunch robot_bringup slam.launch"
  stop_local_pattern "roslaunch robot_bringup navigation.launch"
  stop_local_pattern "roslaunch robot_bringup follow_vision.launch"
  stop_local_pattern "/teleop_twist_keyboard/teleop_twist_keyboard.py"
  stop_local_pattern "/robot_bringup/send_goals.py"
  sleep 1
}

cleanup_failed_start() {
  echo "[INFO] 启动失败，回滚本地与 J1900 节点..."
  stop_pc_modes
  remote_hardware stop >/dev/null 2>&1 || true
}

require_ros_packages() {
  local package
  for package in "$@"; do
    if ! rospack find "${package}" >/dev/null 2>&1; then
      echo "[ERROR] PC 缺少 ROS 包: ${package}"
      return 1
    fi
  done
}

prepare_hardware() {
  local remote_mode="$1"
  ensure_roscore && check_ssh || return 1
  stop_pc_modes
  echo "[INFO] 启动 J1900 ${remote_mode} 模式..."
  if ! remote_hardware "${remote_mode}"; then
    cleanup_failed_start
    return 1
  fi
  if ! wait_for_topic /odom || ! wait_for_topic /imu || ! wait_for_topic /scan; then
    cleanup_failed_start
    return 1
  fi
}

start_detached() {
  local log_file="$1"
  shift
  setsid "$@" >"${log_file}" 2>&1 < /dev/null &
}

validate_goal_file() {
  local goal_file="$1"
  python3 - "${goal_file}" <<'PY'
import math
import os
import sys
import yaml

path = sys.argv[1]
if not os.path.isfile(path):
    raise SystemExit("[ERROR] 巡航点文件不存在: " + path)
with open(path, "r", encoding="utf-8") as stream:
    data = yaml.safe_load(stream) or {}
goals = data.get("goals", []) if isinstance(data, dict) else []
if len(goals) < 2:
    raise SystemExit("[ERROR] 巡航点至少需要 2 个，请编辑: " + path)
for index, goal in enumerate(goals, 1):
    if not isinstance(goal, (list, tuple)) or len(goal) not in (2, 3):
        raise SystemExit("[ERROR] 第 %d 个巡航点必须是 [x, y] 或 [x, y, yaw]" % index)
    try:
        values = [float(value) for value in goal]
    except (TypeError, ValueError):
        raise SystemExit("[ERROR] 第 %d 个巡航点包含非数字" % index)
    if not all(math.isfinite(value) for value in values):
        raise SystemExit("[ERROR] 第 %d 个巡航点包含无效数值" % index)
print("[OK] 已加载 %d 个巡航点: %s" % (len(goals), path))
PY
}

run_slam() {
  require_ros_packages gmapping robot_localization topic_tools teleop_twist_keyboard || return 1
  prepare_hardware base || return 1

  local log_file="${LOG_DIR}/slam.log"
  echo "[INFO] 启动 gmapping，日志: ${log_file}"
  start_detached "${log_file}" roslaunch robot_bringup slam.launch start_lidar:=false "$@"
  if ! wait_for_topic /map 30; then
    tail -n 30 "${log_file}" 2>/dev/null || true
    cleanup_failed_start
    return 1
  fi

  if command -v gnome-terminal >/dev/null 2>&1 && [ -n "${DISPLAY:-}" ]; then
    gnome-terminal --title="SLAM keyboard control" -- bash -lc \
      "source /opt/ros/noetic/setup.bash; source '${ROS_WS}/devel/setup.bash'; export ROS_MASTER_URI='${ROS_MASTER_URI}'; export ROS_IP='${ROS_IP}'; rosrun teleop_twist_keyboard teleop_twist_keyboard.py _speed:=0.30 _turn:=0.80 _repeat_rate:=20.0 _key_timeout:=1.0"
    echo "[OK] SLAM 与键盘窗口已启动"
  else
    echo "[WARN] 当前没有图形终端，SLAM 已启动；请另开终端运行键盘控制"
    echo "       rosrun teleop_twist_keyboard teleop_twist_keyboard.py"
  fi
  echo "保存地图: bash ${ROS_WS}/src/robot_bringup/scripts/save_map.sh <地图名>"
}

run_patrol() {
  local map_file="${1:-$HOME/maps/lab_map.yaml}"
  local goal_file="${2:-${ROS_WS}/src/robot_bringup/config/patrol_goals.yaml}"

  if [ ! -f "${map_file}" ]; then
    echo "[ERROR] 地图不存在: ${map_file}"
    return 1
  fi
  validate_goal_file "${goal_file}" || return 1
  require_ros_packages map_server amcl move_base teb_local_planner robot_localization topic_tools || return 1
  prepare_hardware base || return 1

  local nav_log="${LOG_DIR}/navigation.log"
  local patrol_log="${LOG_DIR}/patrol.log"
  echo "[INFO] 启动导航，地图: ${map_file}"
  start_detached "${nav_log}" roslaunch robot_bringup navigation.launch \
    map_file:="${map_file}" start_lidar:=false
  if ! wait_for_topic /move_base/status 45; then
    tail -n 30 "${nav_log}" 2>/dev/null || true
    cleanup_failed_start
    return 1
  fi

  start_detached "${patrol_log}" rosrun robot_bringup send_goals.py \
    _goal_file:="${goal_file}" _loop:=true
  sleep 2
  if ! rosnode list 2>/dev/null | grep -qx /send_goals; then
    echo "[ERROR] 巡航节点启动失败，日志: ${patrol_log}"
    tail -n 30 "${patrol_log}" 2>/dev/null || true
    cleanup_failed_start
    return 1
  fi
  echo "[OK] 多点循环巡航已启动"
  echo "导航日志: ${nav_log}"
  echo "巡航日志: ${patrol_log}"
}

run_follow() {
  if [ ! -f "${ROS_WS}/yolov8n.pt" ]; then
    echo "[ERROR] YOLO 权重不存在: ${ROS_WS}/yolov8n.pt"
    return 1
  fi
  if ! python3 -c "import cv2, ultralytics" >/dev/null 2>&1; then
    echo "[ERROR] PC 缺少视觉依赖，执行: pip3 install --user ultralytics opencv-python"
    return 1
  fi
  prepare_hardware vision || return 1
  if ! wait_for_topic /image_raw/compressed 30; then
    cleanup_failed_start
    return 1
  fi

  local log_file="${LOG_DIR}/follow.log"
  echo "[INFO] 启动视觉+雷达人体跟踪，日志: ${log_file}"
  start_detached "${log_file}" roslaunch robot_bringup follow_vision.launch "$@"
  if ! wait_for_topic /person_visible 60; then
    tail -n 30 "${log_file}" 2>/dev/null || true
    cleanup_failed_start
    return 1
  fi
  echo "[OK] 人体跟踪已启动"
}

show_status() {
  echo "=== PC ==="
  echo "ROS_MASTER_URI=${ROS_MASTER_URI}"
  if timeout --kill-after=1 3 rosnode list >/dev/null 2>&1; then
    rosnode list | grep -E '^/(slam_gmapping|send_goals|move_base|person_detector|follower|serial_node|s9_lidar_driver)$' || true
  else
    echo "roscore: 停止"
  fi
  echo "=== J1900 ==="
  if check_ssh >/dev/null 2>&1; then
    remote_hardware status
  else
    echo "SSH: 无法连接 ${J1900_HOST}"
  fi
}

case "${MODE}" in
  slam)
    run_slam "$@"
    ;;
  patrol)
    run_patrol "$@"
    ;;
  follow)
    run_follow "$@"
    ;;
  stop)
    stop_pc_modes
    if check_ssh >/dev/null 2>&1; then
      remote_hardware stop || true
    fi
    echo "[OK] 实物功能节点已停止"
    ;;
  status)
    show_status
    ;;
  help|-h|--help)
    print_help
    ;;
  *)
    echo "[ERROR] 未知模式: ${MODE}"
    print_help
    exit 1
    ;;
esac
