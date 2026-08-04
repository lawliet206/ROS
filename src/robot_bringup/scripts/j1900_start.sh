#!/bin/bash
# J1900 hardware lifecycle: ESP32, lidar, and optional camera stream.

MODE="${1:-base}"
ROS_WS="${ROS_WS:-$HOME/ROS}"
PC_MASTER="${PC_MASTER:-10.80.147.11}"
START_TIMEOUT="${START_TIMEOUT:-20}"

ESP32_LOG=/tmp/esp32.log
LIDAR_LOG=/tmp/lidar.log
CAM_LOG=/tmp/cam.log
REPUB_LOG=/tmp/repub.log

source /opt/ros/noetic/setup.bash
if [ ! -f "${ROS_WS}/devel/setup.bash" ]; then
  echo "[ERROR] J1900 工作空间未编译: ${ROS_WS}"
  exit 1
fi
source "${ROS_WS}/devel/setup.bash"
set -u

export ROS_MASTER_URI="http://${PC_MASTER}:11311"
export ROS_IP="${ROS_IP:-$(hostname -I | awk '{print $1}')}"

topic_has_message() {
  local topic="$1"
  local timeout_sec="${2:-2}"
  timeout --kill-after=1 "${timeout_sec}" rostopic echo -n 1 "${topic}" >/dev/null 2>&1
}

wait_for_topic() {
  local topic="$1"
  local waited=0
  local probe_timeout=5
  while [ "${waited}" -lt "${START_TIMEOUT}" ]; do
    if topic_has_message "${topic}" "${probe_timeout}"; then
      echo "  [OK] ${topic} 有数据"
      return 0
    fi
    waited=$((waited + probe_timeout))
  done
  echo "  [ERROR] ${topic} 在 ${START_TIMEOUT}s 内没有数据"
  return 1
}

master_ready() {
  if ! timeout --kill-after=1 8 rostopic list >/dev/null 2>&1; then
    echo "[ERROR] 无法连接 PC ROS Master: ${ROS_MASTER_URI}"
    return 1
  fi
}

check_service_conflict() {
  if systemctl is-active --quiet rosserial.service 2>/dev/null; then
    echo "[ERROR] rosserial.service 正在运行，会与本脚本争抢 ESP32 串口"
    echo "        先执行: sudo systemctl disable --now rosserial.service"
    return 1
  fi
}

detect_ports() {
  ESP32_PORT=""
  LIDAR_PORT=""
  local dev driver
  for dev in /dev/ttyUSB*; do
    [ -e "${dev}" ] || continue
    driver=$(readlink -f "/sys/class/tty/$(basename "${dev}")/device/driver" 2>/dev/null || true)
    case "${driver}" in
      *cp210x*)     ESP32_PORT="${dev}" ;;
      *ch341-uart*) LIDAR_PORT="${dev}" ;;
    esac
  done

  if [ -z "${ESP32_PORT}" ] || [ -z "${LIDAR_PORT}" ]; then
    echo "[ERROR] 硬件端口不完整: ESP32=${ESP32_PORT:-未找到} 雷达=${LIDAR_PORT:-未找到}"
    return 1
  fi
  echo "  端口: ESP32=${ESP32_PORT} 雷达=${LIDAR_PORT}"
}

stop_process() {
  local pattern="$1"
  local i
  pkill -TERM -f "${pattern}" 2>/dev/null || true
  for i in 1 2 3 4 5 6 7 8 9 10; do
    pgrep -f "${pattern}" >/dev/null || return 0
    sleep 0.2
  done
  pkill -KILL -f "${pattern}" 2>/dev/null || true
}

stop_camera() {
  stop_process "/image_transport/republish"
  stop_process "/usb_cam/usb_cam_node"
}

stop_all() {
  echo "停止 J1900 硬件节点..."
  timeout --kill-after=1 2 rostopic pub -1 /cmd_vel geometry_msgs/Twist '{}' >/dev/null 2>&1 || true
  stop_camera
  stop_process "/robot_bringup/s9_lidar_driver.py"
  stop_process "/rosserial_python/serial_node.py"
  if systemctl is-active --quiet rosserial.service 2>/dev/null; then
    echo "  [WARN] rosserial.service 仍在运行，请手动执行 sudo systemctl disable --now rosserial.service"
    return 1
  fi
  echo "  [OK] J1900 硬件节点已停止"
}

start_esp32() {
  if pgrep -f "/rosserial_python/serial_node.py" >/dev/null; then
    if topic_has_message /odom 5; then
      echo "  [OK] ESP32 rosserial 已在运行"
      return 0
    fi
    echo "  [WARN] rosserial 进程无 /odom 数据，重新启动"
    stop_process "/rosserial_python/serial_node.py"
  fi

  echo "  复位 ESP32 (${ESP32_PORT})..."
  python3 - "${ESP32_PORT}" <<'PY'
import serial
import sys
import time

port = sys.argv[1]
with serial.Serial(port, 115200, timeout=0.1) as ser:
    ser.setDTR(False)
    ser.setRTS(True)
    time.sleep(0.6)
    ser.setRTS(False)
    time.sleep(0.8)
    ser.reset_input_buffer()
PY
  sleep 2

  echo "  启动 rosserial (115200)..."
  nohup rosrun rosserial_python serial_node.py \
    _port:="${ESP32_PORT}" _baud:=115200 >"${ESP32_LOG}" 2>&1 < /dev/null &

  if ! wait_for_topic /odom; then
    tail -n 20 "${ESP32_LOG}" 2>/dev/null || true
    stop_process "/rosserial_python/serial_node.py"
    return 1
  fi
}

start_lidar() {
  if pgrep -f "/robot_bringup/s9_lidar_driver.py" >/dev/null; then
    if topic_has_message /scan 5; then
      echo "  [OK] 雷达驱动已在运行"
      return 0
    fi
    echo "  [WARN] 雷达进程无 /scan 数据，重新启动"
    stop_process "/robot_bringup/s9_lidar_driver.py"
  fi

  echo "  启动 S9 雷达 (${LIDAR_PORT})..."
  nohup rosrun robot_bringup s9_lidar_driver.py \
    _port:="${LIDAR_PORT}" >"${LIDAR_LOG}" 2>&1 < /dev/null &

  if ! wait_for_topic /scan; then
    tail -n 20 "${LIDAR_LOG}" 2>/dev/null || true
    stop_process "/robot_bringup/s9_lidar_driver.py"
    return 1
  fi
}

start_camera() {
  if [ ! -e /dev/video0 ]; then
    echo "[ERROR] 未找到摄像头 /dev/video0"
    return 1
  fi
  for package in usb_cam image_transport compressed_image_transport; do
    if ! rospack find "${package}" >/dev/null 2>&1; then
      echo "[ERROR] J1900 缺少 ROS 包: ${package}"
      return 1
    fi
  done

  if topic_has_message /image_raw/compressed 5; then
    echo "  [OK] 摄像头压缩流已在运行"
    return 0
  fi

  stop_camera
  echo "  启动摄像头压缩流 (320x240)..."
  nohup rosrun usb_cam usb_cam_node \
    _video_device:=/dev/video0 _image_width:=320 _image_height:=240 \
    _pixel_format:=yuyv >"${CAM_LOG}" 2>&1 < /dev/null &
  sleep 2
  nohup rosrun image_transport republish \
    raw in:=/usb_cam/image_raw compressed out:=/image_raw \
    >"${REPUB_LOG}" 2>&1 < /dev/null &

  if ! wait_for_topic /image_raw/compressed; then
    tail -n 20 "${CAM_LOG}" 2>/dev/null || true
    tail -n 20 "${REPUB_LOG}" 2>/dev/null || true
    stop_camera
    return 1
  fi
}

status() {
  echo "=== J1900 状态 ==="
  echo "ROS_MASTER_URI=${ROS_MASTER_URI}"
  pgrep -f "/rosserial_python/serial_node.py" >/dev/null \
    && echo "ESP32 : 运行" || echo "ESP32 : 停止"
  pgrep -f "/robot_bringup/s9_lidar_driver.py" >/dev/null \
    && echo "雷达  : 运行" || echo "雷达  : 停止"
  pgrep -f "/usb_cam/usb_cam_node" >/dev/null \
    && echo "摄像头: 运行" || echo "摄像头: 停止"
  pgrep -f "/image_transport/republish" >/dev/null \
    && echo "压缩流: 运行" || echo "压缩流: 停止"
  systemctl is-active --quiet rosserial.service 2>/dev/null \
    && echo "systemd rosserial: 冲突（运行中）" || echo "systemd rosserial: 停止"
}

case "${MODE}" in
  0) MODE=stop ;;
  1) MODE=base ;;
  2) MODE=vision ;;
  3) MODE=status ;;
esac

case "${MODE}" in
  base)
    check_service_conflict && master_ready && detect_ports || exit 1
    stop_camera
    if ! start_esp32 || ! start_lidar; then
      stop_all
      exit 1
    fi
    ;;
  vision)
    check_service_conflict && master_ready && detect_ports || exit 1
    if ! start_esp32 || ! start_lidar || ! start_camera; then
      stop_all
      exit 1
    fi
    ;;
  stop)
    stop_all
    ;;
  status)
    status
    ;;
  *)
    echo "用法: bash $0 {base|vision|stop|status}"
    exit 1
    ;;
esac
