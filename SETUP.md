# ROS 两轮差速机器人 — 完整启动指南

## 目录

- [1. 系统架构](#1-系统架构)
- [2. 环境安装](#2-环境安装)
  - [2.1 PC](#21-pc-一次性)
  - [2.2 J1900](#22-j1900-一次性)
  - [2.3 ESP32 固件](#23-esp32-固件烧录)
- [3. 网络配置](#3-网络配置pc-j1900)
- [4. 硬件接线](#4-硬件接线)
  - [4.1 供电](#41-供电)
  - [4.2 TB6612FNG 电机驱动](#42-tb6612fng-电机驱动)
  - [4.3 编码器](#43-编码器)
  - [4.4 MPU6050 IMU](#44-mpu6050-imu)
  - [4.5 电池电压检测](#45-电池电压检测)
  - [4.6 串口权限](#46-串口权限)
- [5. 首次上电](#5-首次上电安全流程)
- [6. 仿真操作](#6-仿真操作pc-单机)
  - [6.1 SLAM 建图](#61-slam-建图)
  - [6.2 导航](#62-导航)
  - [6.3 激光跟随](#63-激光跟随)
- [7. 实物操作](#7-实物操作)
  - [7.1 SLAM 建图](#71-slam-建图)
  - [7.2 多点导航](#72-多点导航)
  - [7.3 人体跟随](#73-人体跟随)
  - [7.4 腿跟踪测试](#74-腿跟踪测试leg_tracker)
  - [7.5 EKF 传感器融合](#75-ekf-传感器融合可选)
- [8. 快速参考](#8-快速参考)
- [9. 常见问题](#9-常见问题)

---

## 1. 系统架构

```
PC (ROS Master) ←WiFi→ J1900 (车载) ←USB→ ESP32
                     │                   ├─ PWM → TB6612FNG → 左/右电机
                     │                   ├─ PCNT ← 编码器 (JGB37-520)
                     │                   └─ I2C  ← MPU6050 IMU
                     └─ USB ← 激光雷达 (S9-FSRD / YDLIDAR F2)
```

| 设备 | 说明 |
|------|------|
| **PC** | Ubuntu 20.04，SLAM/导航/跟随，ROS Master |
| **J1900** | 车载 x86_64，Ubuntu 20.04，rosserial + 激光雷达驱动 |
| **ESP32** | 下位机，PCNT 编码器 + PID + IMU，rosserial |
| **激光雷达** | S9-FSRD-V1.0 RX，AA55 协议，115200 |

---

## 2. 环境安装

### 2.1 PC（一次性）

```bash
# 1. 配置 ROS apt 仓库
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
sudo apt update

# 2. 安装 ROS + 依赖
sudo apt install ros-noetic-desktop-full
sudo apt install ros-noetic-gazebo-ros-pkgs ros-noetic-gazebo-ros-control
sudo apt install ros-noetic-gmapping ros-noetic-move-base ros-noetic-amcl
sudo apt install ros-noetic-map-server ros-noetic-teleop-twist-keyboard
sudo apt install ros-noetic-robot-state-publisher ros-noetic-topic-tools
sudo apt install ros-noetic-robot-localization
sudo apt install ros-noetic-teb-local-planner
sudo apt install ros-noetic-tf ros-noetic-interactive-markers libfftw3-dev python3-scipy
sudo apt install mesa-utils libgl1-mesa-dri libgl1-mesa-glx
pip3 install pyserial pyyaml pykalman

# 3. 编译工作空间
cd ~/ROS
git clone https://github.com/angusleigh/leg_tracker.git src/leg_tracker && source /opt/ros/noetic/setup.bash
catkin_init_workspace src && catkin_make
source ~/ROS/devel/setup.bash
echo "source ~/ROS/devel/setup.bash" >> ~/.bashrc
```

### 2.2 J1900（一次性）

```bash
# 1. 配置 ROS apt 仓库（同上）
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
sudo apt update

# 2. 安装 ROS Base + 驱动依赖
sudo apt install ros-noetic-ros-base python3-serial python3-pip
sudo apt install ros-noetic-rosserial-python
pip3 install pyserial pyyaml

# 3. 从 PC 迁移工作空间
# 先在两台机器上查看 IP:
#   hostname -I
# 然后设置环境变量 (以下为示例，请替换为实际 IP):
#   export PC_IP=192.168.1.118
#   export J1900_IP=192.168.1.200
#
# PC 上执行:
#   scp -r ~/ROS lawliet@${J1900_IP}:~/
# J1900 上:
cd ~/ROS && source /opt/ros/noetic/setup.bash && catkin_make
echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
echo "source ~/ROS/devel/setup.bash" >> ~/.bashrc
```

### 2.3 ESP32 固件烧录

Arduino IDE 打开 `esp32_firmware/esp32_firmware.ino`：
- Board → ESP32 Dev Module，Upload Speed → 115200
- 烧录后通过 rosserial 自动发布 `/odom` + `/imu`

---

## 3. 网络配置（PC ↔ J1900）

两台在同一 WiFi 下。

### 3.1 查看 IP

```bash
# 两台机器都执行
hostname -I
# 记录两台 IP，设置环境变量 (以下为示例):
export PC_IP=192.168.1.118
export J1900_IP=192.168.1.200
```

### 3.2 SSH 免密登录

```bash
# PC 上执行 (只需一次)
ssh-keygen -t rsa -b 4096 -N "" -f ~/.ssh/id_rsa
ssh-copy-id lawliet@${J1900_IP}   # 输入 J1900 密码

# 测试
ssh lawliet@${J1900_IP} "echo ssh ok"
```

### 3.3 设置 ROS 主从

```bash
# ========== PC (~/.bashrc 末尾添加) ==========
export ROS_MASTER_URI=http://${PC_IP}:11311   # PC 是 Master
export ROS_IP=${PC_IP}

# ========== J1900 (~/.bashrc 末尾添加) ==========
export ROS_MASTER_URI=http://${PC_IP}:11311   # 指向 PC
export ROS_IP=${J1900_IP}

# 两台都 source 一下让配置生效
source ~/.bashrc
```

### 3.4 验证

```bash
# PC 启动 roscore
roscore

# J1900 测试能否连上 Master
rostopic list   # 应看到 /rosout 和 /rosout_agg
```

> ⚠️ 如果 J1900 的 `rostopic list` 卡住：PC 和 J1900 互相 `ping` 对方 IP，确认在同一网络。关闭 PC 防火墙：`sudo ufw disable`。

---

## 4. 硬件接线

### 4.1 供电

- 电池 12V → TB6612FNG VM（电机供电）
- 电池 12V → LM2596 降压 5V → ESP32 VIN
- 电池负极 → 所有设备 GND 共地

### 4.2 TB6612FNG 电机驱动

| TB6612 | ESP32 | 说明 |
|--------|-------|------|
| PWMA | GPIO18 | 左 PWM |
| AIN2 | GPIO26 | 左方向 |
| AIN1 | GPIO25 | 左方向 |
| STBY | GPIO4 | 使能 |
| BIN1 | GPIO32 | 右方向 |
| BIN2 | GPIO33 | 右方向 |
| PWMB | GPIO19 | 右 PWM |
| VM | 电池 12V | 电机供电 |
| VCC | 3.3V | 逻辑供电 |
| GND | GND | 共地 |

> **电机方向说明：** 左右电机为镜像安装，固件中两个电机的 IN1/IN2 逻辑均已交换。
> 即 `RPM>0` 前进时：IN1=LOW, IN2=HIGH。若下地后方向反，先检查 BO1/BO2 接线（红白方向），再调整固件。

### 4.3 编码器

JGB37-520 6pin：M1-红, GND-黑, B-黄, A-绿, Vcc-蓝, M2-白

| 线色 | 左电机 | 右电机 |
|------|--------|--------|
| 红 (M1) | TB6612 AO1 | TB6612 BO1 |
| 黑 (GND) | GND | GND |
| 黄 (B) | GPIO23 | GPIO13 |
| 绿 (A) | GPIO27 | GPIO14 |
| 蓝 (Vcc) | 3.3V | 3.3V |
| 白 (M2) | TB6612 AO2 | TB6612 BO2 |

### 4.4 MPU6050 IMU

| MPU6050 | ESP32 |
|---------|-------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO21 |
| SCL | GPIO22 |
| AD0 | GND (地址=0x68) |

### 4.5 电池电压检测

```
电池 12V → 10kΩ ─┬─ GPIO34 (分压 ≈ 1.15V @ 12.6V)
                1kΩ
                 │
                GND
```

### 4.6 串口权限

```bash
echo 'KERNEL=="ttyUSB*", MODE="0666"' | sudo tee /etc/udev/rules.d/99-usb-serial.rules
sudo udevadm control --reload-rules
```

每次开机确认：
```bash
ls /dev/ttyUSB*   # USB0=ESP32  USB1=激光雷达
```

---

## 5. 首次上电安全流程

**车轮架起来（悬空）测试：**

1. 万用表确认 TB6612FNG VM = 电池电压（~11-12V）
2. 电池上电 → ESP32 亮灯
3. J1900 启动 rosserial：
   ```bash
   rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800
   ```
   看到 `connected on /dev/ttyUSB0` 即成功

4. 测试电机：
   ```bash
   rostopic pub /cmd_vel geometry_msgs/Twist '{linear: {x: 0.3}}' -r 1
   # Ctrl-C 停止
   rostopic pub /cmd_vel geometry_msgs/Twist '{}' -1
   ```

5. 检查数据：`rostopic echo /odom -n1`
6. 手动转轮子，看里程计变化
7. 全部正常 → 轮子着地 → 正式运行

---

## 6. 仿真操作（PC 单机）

> **⚠️ 每个新终端必须先执行：`source ~/ROS/devel/setup.bash`**
> 否则 `rosrun`/`roslaunch` 会报 `package not found`。
>
> 卡住了？`bash ~/ROS/tools/kill_ros.sh` 一键清理所有 ROS/Gazebo 进程。

### 6.1 SLAM 建图

```bash
bash ~/ROS/src/robot_sim/scripts/sim_slam.sh

# 另开终端: 键盘遥控
rosrun teleop_twist_keyboard teleop_twist_keyboard.py

# 保存地图
rosrun map_server map_saver -f ~/maps/sim_map
```

### 6.2 导航

前提：已建图。

```bash
# 终端1
bash ~/ROS/src/robot_sim/scripts/sim_navigation.sh ~/maps/sim_map.yaml

# 终端2: 自动巡点
rosrun robot_bringup send_goals.py _goals:="[[1,2,0],[3,4,1.57],[5,1,0]]"
```

### 6.3 激光跟随

```bash
bash ~/ROS/src/robot_sim/scripts/sim_follow.sh
```

---

## 7. 实物操作

> **⚠️ PC 和 J1900 每个新终端都必须先执行：`source ~/ROS/devel/setup.bash`**
> J1900 的 `~/.bashrc` 需已配置 `ROS_MASTER_URI` 指向 PC。

### 7.1 SLAM 建图

| # | 设备 | 命令 |
|---|------|------|
| 1 | PC | `roscore` |
| 2 | J1900 | `rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800` |
| 3 | J1900 | `rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB1` |
| 4 | PC | `bash ~/ROS/src/robot_bringup/scripts/slam_start.sh` |
| 5 | PC | `rosrun teleop_twist_keyboard teleop_twist_keyboard.py`（i 前进 k 停） |

> **自动保护**: slam_start.sh 会等待 `/odom` + `/scan` 就绪(60s超时)，再启动 SLAM。

保存地图：
```bash
rosrun map_server map_saver -f ~/maps/lab_map
```

### 7.2 多点导航

前提：已建图。

| # | 设备 | 命令 |
|---|------|------|
| 1 | PC | `roscore` |
| 2 | J1900 | `rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800` |
| 3 | J1900 | `rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB1` |
| 4 | PC | `bash ~/ROS/src/robot_bringup/scripts/nav_start.sh ~/maps/lab_map.yaml` |
| 5 | PC | `rosrun robot_bringup send_goals.py _goals:="[[2,0,0],[4,2,1.57]]"` |

或在 RViz 用 "2D Nav Goal" 手动点目标。

### 7.3 人体跟随

| # | 设备 | 命令 |
|---|------|------|
| 1 | PC | `roscore` |
| 2 | J1900 | `rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800` |
| 3 | J1900 | `rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB1` |
| 4 | PC | `roslaunch robot_bringup follow.launch start_lidar:=false` |

---

### 7.4 腿跟踪测试（leg_tracker）

用机器学习腿检测器替代原生激光聚类，输出人的精确位置。

| # | 设备 | 命令 |
|---|------|------|
| 1 | PC | `roscore` |
| 2 | J1900 | `rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800` |
| 3 | J1900 | `rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB1` |
| 4 | PC | `python3 -c "import rospy; rospy.init_node('s'); rospy.set_param('/robot_description', open('$HOME/ROS/src/robot_bringup/urdf/robot.urdf').read())" & rosrun robot_state_publisher robot_state_publisher` |
| 5 | PC | `roslaunch leg_tracker joint_leg_tracker.launch scan:=/scan fixed_frame:=laser_link confidence_threshold_to_maintain_track:=-1.0 dist_travelled_together_to_initiate_leg_pair:=0.1 max_leg_pairing_dist:=1.2` |

验证：另开终端 `rostopic echo /people_tracked`，看到 `person_id` 即成功。

---

### 7.5 EKF 传感器融合（已内置，自动启动）

EKF 融合 ESP32 的 `/odom`（编码器）和 `/imu`（MPU6050），输出 `/odometry/filtered`。

**已内置在 `slam.launch` / `navigation.launch` 内**，无需手动启动。
同节点名 `ekf_localization` 天然防重复 — 谁先启谁生效。

如需单独调试：
```bash
roslaunch robot_bringup ekf.launch
rostopic echo /odometry/filtered -n1
```

---

## 8. 快速参考

> 卡住/报错时先清理残留：`bash ~/ROS/tools/kill_ros.sh`

### 仿真

| 命令 | 功能 |
|------|------|
| `bash ~/ROS/src/robot_sim/scripts/sim_slam.sh` | SLAM 建图 |
| `bash ~/ROS/src/robot_sim/scripts/sim_navigation.sh ~/maps/sim_map.yaml` | 导航 |
| `bash ~/ROS/src/robot_sim/scripts/sim_follow.sh` | 激光跟随 |

### 实物

| 在哪 | 命令 | 功能 |
|------|------|------|
| J1900 | `rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800` | ESP32 桥接 |
| J1900 | `rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB1` | 激光雷达 |
| PC | `roslaunch robot_bringup ekf.launch` | EKF 传感器融合 (单独调试用) |
| PC | `bash ~/ROS/src/robot_bringup/scripts/slam_start.sh` | SLAM 建图 (自动等话题) |
| PC | `rosrun map_server map_saver -f ~/maps/lab_map` | 保存地图 |
| PC | `bash ~/ROS/src/robot_bringup/scripts/nav_start.sh ~/maps/lab_map.yaml` | 导航 (自动等话题+冲突检测) |
| PC | `roslaunch robot_bringup follow.launch start_lidar:=false` | 激光跟随 |
| PC | `roslaunch leg_tracker joint_leg_tracker.launch scan:=/scan fixed_frame:=laser_link` | 腿跟踪 |

---

### 各设备任务速查

| 设备 | 负责任务 |
|------|---------|
| **ESP32** | 电机 PID、编码器 PCNT、MPU6050 IMU、里程计计算、rosserial 发布 `/odom` `/imu`、订阅 `/cmd_vel` |
| **J1900** | rosserial 桥接 (`serial_node`)、激光雷达驱动 (`s9_lidar_driver.py`) |
| **PC** | roscore (Master)、SLAM (`gmapping`)、导航 (`amcl` + `move_base`)、EKF 融合 (`ekf.launch`)、巡点 (`send_goals.py`)、仿真 (`Gazebo`)、可视化 (`RViz`) |

---

## 9. 常见问题

| 问题 | 解决 |
|------|------|
| pyserial 找不到 | `pip3 install pyserial pyyaml` |
| 串口无权限 | `sudo usermod -a -G dialout $USER` 重新登录 |
| J1900 连不上 PC | 互 ping，确认 ROS_MASTER_URI / ROS_IP 指向 PC |
| rosrun/roslaunch 报 package not found | `source ~/ROS/devel/setup.bash`（每个新终端都要执行一次） |
| Gazebo 打不开 | `export SVGA_VGPU10=0` |
| slam_start.sh 卡在"等待话题" | J1900 上的 rosserial 或雷达驱动未启动，检查 J1900 终端 |
| 导航和跟随能同时跑吗 | **不能** — 两者都写 `/cmd_vel`，会抢控。nav_start.sh 启动时会自动检测并警告 |

---

## 10. 视觉+雷达融合人体跟随（可选）

架构: J1900 摄像头采集推流 → PC YOLOv8n 检测 (person_detector.py) → 融合控制 (vision_follower.py)
分工: 视觉定方向（角度 ±2~3°），雷达定距离（人体宽度约束聚类）

### J1900 端（一次性安装）

```bash
sudo apt install ros-noetic-usb-cam ros-noetic-image-transport

# 摄像头推流 (先确认设备号: ls /dev/video*)
rosrun usb_cam usb_cam_node _video_device:=/dev/video0 _image_width:=640 _image_height:=480 _pixel_format:=yuyv
rosrun image_transport republish raw in:=/usb_cam/image_raw compressed out:=/image_raw
```

### PC 端（一次性安装）

```bash
pip3 install --user ultralytics pytest
```

### 启动

```bash
# J1900: 摄像头推流 + 雷达驱动 (见上)
# PC:
roslaunch robot_bringup follow_vision.launch
# 调试: rqt_image_view /person_overlay 查看检测框
```

### 参数（可选覆盖）

```bash
roslaunch robot_bringup follow_vision.launch follow_dist:=1.2 hfov:=70
```
