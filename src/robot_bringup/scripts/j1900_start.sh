#!/bin/bash
# ============================================================
# j1900_start.sh — J1900 一键启动各硬件节点 (数字模式)
# ============================================================
# 模式:
#   0 : 停止全部节点
#   1 : base   (ESP32 + 雷达)         SLAM 建图 / move_base 导航前置
#   2 : vision (ESP32 + 雷达 + 摄像头) 视觉+雷达融合跟随
#   3 : status 查看运行状态
#
# 用法:
#   bash ~/ROS/src/robot_bringup/scripts/j1900_start.sh 1
#   bash ~/ROS/src/robot_bringup/scripts/j1900_start.sh 2
#   bash ~/ROS/src/robot_bringup/scripts/j1900_start.sh 0
# ============================================================

# ROS 环境 (显式配置, 不依赖 bashrc — ssh 非交互不加载 bashrc)
PC_MASTER="10.80.147.11"                       # [可改] PC (ROS Master) 的 IP
source /opt/ros/noetic/setup.bash
[ -f ~/ROS/devel/setup.bash ] && source ~/ROS/devel/setup.bash
export ROS_MASTER_URI="http://${PC_MASTER}:11311"
export ROS_IP="$(hostname -I | awk '{print $1}')"

ESP32_LOG=/tmp/esp32.log
LIDAR_LOG=/tmp/lidar.log
CAM_LOG=/tmp/cam.log
REPUB_LOG=/tmp/repub.log

# 动态检测端口 (USB 插拔后 ttyUSB0/1 顺序会互换, 不能写死!)
# 按芯片驱动识别: CP210x = ESP32, ch341(HL-340) = 雷达
detect_ports() {
  ESP32_PORT=""
  LIDAR_PORT=""
  for dev in /dev/ttyUSB*; do
    [ -e "$dev" ] || continue
    driver=$(readlink -f "/sys/class/tty/$(basename "$dev")/device/driver" 2>/dev/null | grep -oE 'cp210x|ch341-uart')
    case "$driver" in
      cp210x)     ESP32_PORT="$dev" ;;
      ch341-uart) LIDAR_PORT="$dev" ;;
    esac
  done
  if [ -n "$ESP32_PORT" ] && [ -n "$LIDAR_PORT" ]; then
    echo "  端口检测: ESP32=$ESP32_PORT 雷达=$LIDAR_PORT"
  else
    echo "  ✗ 端口检测失败 (确认 ESP32/雷达 USB 已连接)"
  fi
}

start_esp32() {
  detect_ports
  [ -n "$ESP32_PORT" ] || return 1
  if pgrep -f "[s]erial_node" > /dev/null; then
    echo "  ESP32 已在运行"
  else
    echo "  自动复位 ESP32 ($ESP32_PORT, DTR/RTS 信号)..."
    python3 -c "
import serial, time
ser = serial.Serial('$ESP32_PORT', 230400, timeout=0.1)
ser.setDTR(False); ser.setRTS(True); time.sleep(0.6)   # EN=low 600ms 确保复位
ser.setRTS(False); time.sleep(0.8)                      # EN=high, 等 ESP32 上电
ser.reset_input_buffer()                                # 清空复位期间残留数据
ser.close()
"
    sleep 2   # 等 ESP32 完全启动 (initNode 前)
    echo "  启动 ESP32 rosserial (230400)..."
    nohup rosrun rosserial_python serial_node.py _port:=$ESP32_PORT _baud:=230400 > $ESP32_LOG 2>&1 &
    sleep 6
    pgrep -f "[s]erial_node" > /dev/null \
      && echo "  ✓ ESP32 已启动 (自动复位握手)" \
      || echo "  ✗ ESP32 启动失败 (查 $ESP32_LOG)"
  fi
}

start_lidar() {
  detect_ports
  [ -n "$LIDAR_PORT" ] || return 1
  if pgrep -f "[s]9_lidar_driver" > /dev/null; then
    echo "  雷达已在运行"
  else
    echo "  启动雷达驱动 ($LIDAR_PORT)..."
    nohup rosrun robot_bringup s9_lidar_driver.py _port:=$LIDAR_PORT > $LIDAR_LOG 2>&1 &
    sleep 4
    pgrep -f "[s]9_lidar_driver" > /dev/null \
      && echo "  ✓ 雷达已启动" \
      || echo "  ✗ 雷达启动失败 (查 $LIDAR_LOG)"
  fi
}

start_camera() {
  if pgrep -f "[u]sb_cam_node" > /dev/null; then
    echo "  摄像头已在运行"
  else
    echo "  启动摄像头推流 (320x240, 缓解 WiFi 传输瓶颈)..."
    nohup rosrun usb_cam usb_cam_node _video_device:=/dev/video0 \
      _image_width:=320 _image_height:=240 _pixel_format:=yuyv > $CAM_LOG 2>&1 &
    sleep 3
    nohup rosrun image_transport republish raw in:=/usb_cam/image_raw \
      compressed out:=/image_raw > $REPUB_LOG 2>&1 &
    sleep 3
    pgrep -f "[u]sb_cam_node" > /dev/null \
      && echo "  ✓ 摄像头已启动" \
      || echo "  ✗ 摄像头启动失败 (查 $CAM_LOG)"
  fi
}

stop_all() {
  echo "停止 J1900 节点..."
  for p in $(pgrep -f "[s]erial_node|[s]9_lidar_driver|[u]sb_cam_node|[r]epublish"); do
    kill -9 "$p" 2>/dev/null
  done
  sleep 1
  echo "  ✓ 已全部停止"
}

status() {
  echo "=== J1900 节点状态 ==="
  pgrep -f "[s]erial_node"      > /dev/null && echo "  ESP32 : 运行中" || echo "  ESP32 : 停止"
  pgrep -f "[s]9_lidar_driver"  > /dev/null && echo "  雷达  : 运行中" || echo "  雷达  : 停止"
  pgrep -f "[u]sb_cam_node"     > /dev/null && echo "  摄像头: 运行中" || echo "  摄像头: 停止"
  pgrep -f "[r]epublish"        > /dev/null && echo "  推流  : 运行中" || echo "  推流  : 停止"
}

MODE="${1:-1}"
echo "=== j1900_start [$MODE] ==="
case "$MODE" in
  0) stop_all ;;
  1) start_esp32; start_lidar ;;
  2) start_esp32; start_lidar; start_camera ;;
  3) status ;;
  *)
    echo "用法: bash $0 [0=停止 | 1=base(ESP32+雷达) | 2=vision(+摄像头) | 3=状态]"
    exit 1
    ;;
esac
echo "=== 完成 ==="
