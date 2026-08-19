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
  - [5.1 实机验证基线](#51-实机验证基线2026-08-04)
- [6. 仿真操作](#6-仿真操作pc-单机)
  - [6.1 SLAM 建图](#61-slam-建图)
  - [6.2 导航](#62-导航)
  - [6.3 激光跟随](#63-激光跟随)
- [7. 实物操作](#7-实物操作)
  - [7.1 SLAM 建图](#71-slam-建图)
  - [7.2 多点巡航](#72-多点巡航)
  - [7.3 人体跟踪](#73-人体跟踪)
  - [7.4 EKF 传感器融合](#74-ekf-传感器融合已内置自动启动)
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

**执行位置：PC**

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

# 3. 编译工作空间
cd ~/ROS
source /opt/ros/noetic/setup.bash
catkin_init_workspace src && catkin_make
source ~/ROS/devel/setup.bash
echo "source ~/ROS/devel/setup.bash" >> ~/.bashrc

# 4. Python 依赖 (视觉跟随 YOLOv8n + 测试)
pip3 install --user pyserial pyyaml ultralytics pytest

# 5. YOLOv8n COCO 权重必须位于工作空间根目录（人体类别 class 0）
test -f ~/ROS/yolov8n.pt
```

若最后一条命令提示文件不存在，请先把 `yolov8n.pt` 放到 `~/ROS/yolov8n.pt`；人体跟踪
按离线模式加载该文件，不会在启动小车时临时下载模型。

### 2.2 J1900（一次性）

**执行位置：J1900**（安装依赖并停用旧服务）

```bash
# 1. 配置 ROS apt 仓库（同上）
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
sudo apt update

# 2. 安装 ROS Base + 驱动依赖
sudo apt install ros-noetic-ros-base python3-serial python3-pip
sudo apt install ros-noetic-rosserial-python
# 摄像头推流 (视觉跟随用, compressed_image_transport 是压缩插件必需):
sudo apt install ros-noetic-usb-cam ros-noetic-image-transport ros-noetic-compressed-image-transport
pip3 install pyserial pyyaml

# 3. 串口权限 (雷达/ESP32 必需)
echo 'KERNEL=="ttyUSB*", MODE="0666"' | sudo tee /etc/udev/rules.d/99-usb-serial.rules
sudo udevadm control --reload-rules

# 3. 停用旧的 systemd 串口服务，统一由一键脚本管理
sudo systemctl disable --now rosserial.service 2>/dev/null || true

```

**执行位置：PC**（把 `robot_bringup` 部署到 J1900；`--delete` 会同步删除远端旧文件）

```bash
rsync -av --delete --exclude='__pycache__/' --exclude='*.pyc' \
  ~/ROS/src/robot_bringup/ lawliet@lawliet.local:~/ROS/src/robot_bringup/
```

**执行位置：J1900**（部署后编译）

```bash
cd ~/ROS && source /opt/ros/noetic/setup.bash
catkin_init_workspace src && catkin_make
echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
echo "source ~/ROS/devel/setup.bash" >> ~/.bashrc
```

### 2.3 ESP32 固件烧录

**执行位置：PC**（ESP32 用 USB 直接连接 PC；正常使用时不要从 J1900 远程烧录）

Arduino IDE 打开 `esp32_firmware/esp32_firmware.ino`：
- Board → ESP32 Dev Module，Upload Speed → 115200
- 烧录后通过 rosserial 自动发布 `/odom` + `/imu`，订阅 `/cmd_vel`

**注意**：固件内 rosserial 运行波特率固定为 **115200**（`ROSSERIAL_BAUD`），J1900
启动 `serial_node.py` 时必须带 `_baud:=115200`，否则无法协商话题。
（Arduino 上传速度 115200 是烧录用的，与 rosserial 运行波特率无关。）

固件功能：PCNT 硬件编码器（不漏脉冲）、双路 PID、MPU6050 IMU（已旋转到 base_link 帧）、
看门狗 800ms（无指令自动停车）、堵转检测（命令≥80RPM 但实际<1RPM 持续 2s 切断 PWM）。

**⚠️ ESP32 连接注意（实车经验）**：
- `serial_node.py` 要**保持常驻**（不要反复重启）
- `j1900_start.sh` 启动桥接前会使用 DTR/RTS 复位 ESP32，使其重新发送 TopicInfo
- `/odom` 的 ROS 序列化长度是 718 bytes，必须使用固件中的 1024-byte 发布缓冲：
  `ros::NodeHandle_<ArduinoHardware, 25, 25, 512, 1024>`
- 使用默认 512-byte 发布缓冲会在第一帧 `/odom` 序列化时破坏内存，表现为话题注册成功、
  启动日志只有几帧，随后 `/odom` 无新消息且电机不响应；这不是 MPU6050/I2C 卡死
- 连接日志必须出现 `publish buffer size is 1024 bytes`。正常情况下 `/odom` 约 8Hz、
  `/imu` 约 4Hz；两者错峰发送，合计约 7.3KB/s，为 115200 串口保留吞吐余量

---

## 3. 网络配置（PC ↔ J1900）

两台在同一 WiFi 下。

### 3.1 查看 IP

**执行位置：PC 和 J1900**（两台机器分别执行一次）

```bash
# 两台机器都执行
hostname -I
# 记录两台 IP，设置环境变量 (以下为示例):
export PC_IP=192.168.1.118
export J1900_IP=192.168.1.200
```

### 3.2 SSH 免密登录

**执行位置：PC**

```bash
# PC 上执行 (只需一次)
ssh-keygen -t rsa -b 4096 -N "" -f ~/.ssh/id_rsa
ssh-copy-id lawliet@${J1900_IP}   # 输入 J1900 密码

# 测试
ssh lawliet@${J1900_IP} "echo ssh ok"
```

### 3.3 设置 ROS 主从

下面两段分别写入对应设备的 `~/.bashrc`，不要互换：

```bash
# ========== PC (~/.bashrc 末尾添加) ==========
export ROS_MASTER_URI=http://${PC_IP}:11311   # PC 是 Master
export ROS_IP=${PC_IP}

# ========== J1900 (~/.bashrc 末尾添加) ==========
export ROS_MASTER_URI=http://${PC_IP}:11311   # 指向 PC
# ROS_IP 用动态获取! J1900 的 WiFi IP 会浮动 (DHCP), 写死会导致节点间无法通信
export ROS_IP=$(hostname -I | awk '{print $1}')

# 两台都 source 一下让配置生效
source ~/.bashrc
```

### 3.4 验证

**执行位置：PC**

```bash
# PC 验证免密 SSH；一键脚本会自行启动 roscore
ssh -o BatchMode=yes lawliet@lawliet.local "echo ssh-ok"

# 查看 PC/J1900 状态（不启动电机控制）
bash ~/ROS/src/robot_bringup/scripts/robot_start.sh status
```

> ⚠️ 如果 J1900 的 `rostopic list` 卡住：PC 和 J1900 互相 `ping` 对方 IP，确认在同一网络。关闭 PC 防火墙：`sudo ufw disable`。
> ⚠️ **J1900 IP 浮动**：`lawliet.local` 每次重连解析的 IP 可能不同（如 .211 ↔ .212），SSH 前先 `ping lawliet.local` 拿当前 IP。

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

**执行位置：J1900**

```bash
echo 'KERNEL=="ttyUSB*", MODE="0666"' | sudo tee /etc/udev/rules.d/99-usb-serial.rules
sudo udevadm control --reload-rules
```

每次开机确认：

**执行位置：J1900**

```bash
ls -l /dev/serial/by-id/
readlink -f /dev/serial/by-id/*
```

不要假设 `/dev/ttyUSB0` 和 `/dev/ttyUSB1` 的编号固定，USB 插拔后编号可能互换。按 USB
芯片识别设备：

| USB 标识/驱动 | 设备 |
|---------------|------|
| `Silicon_Labs_CP2102` / `cp210x` | ESP32 |
| `1a86_USB_Serial` / `ch341-uart` | S9 雷达 |

`j1900_start.sh` 会按驱动动态检测端口，不依赖 `/dev/ttyUSB0`、`/dev/ttyUSB1` 的顺序。

---

## 5. 首次上电安全流程

**必须先把车轮架起来（悬空）测试，并保证有人可以立即断电。**

1. 万用表确认 TB6612FNG VM = 电池电压（~11-12V）
2. 电池上电 → ESP32 亮灯
3. 确认 J1900 的旧串口服务已停用（只需做一次）：
   **执行位置：PC**（命令通过 SSH 在 J1900 执行 `systemctl`）

   ```bash
   ssh -t lawliet@lawliet.local 'sudo systemctl disable --now rosserial.service'
   ```
4. 在 PC 一键启动 SLAM。脚本会自动启动 roscore、通过 SSH 启动 J1900 的 ESP32 和雷达、
   等待真实消息，再启动 gmapping 和键盘窗口：
   **执行位置：PC**

   ```bash
   bash ~/ROS/src/robot_bringup/scripts/robot_start.sh slam
   ```
5. PC 验证通信，必须连续观察而不是只确认话题名称存在：
   **执行位置：PC**

   ```bash
   export ROS_IP=10.80.147.11
   export ROS_MASTER_URI=http://10.80.147.11:11311
   rosnode ping -c 2 /serial_node
   rostopic hz /odom
   rostopic hz /imu
   ```

   预期：`/serial_node` 的 URI 指向 J1900，`/odom` 约 8Hz、`/imu` 约 4Hz，持续数十秒
   无断流。若 PC 的 `ROS_IP` 误设为 `127.0.0.1`，J1900 无法回连 PC 上的话题节点。

6. 分别点动左右电机。以下每段只运行 2 秒，`timeout` 结束后必须发送零速度：
   **执行位置：PC**

   ```bash
   # 左轮单独正转：v_left=0.40m/s, v_right=0
   timeout --signal=INT 2s rostopic pub -r 20 /cmd_vel geometry_msgs/Twist \
     '{linear: {x: 0.2}, angular: {z: -2.222222}}'
   rostopic pub -1 /cmd_vel geometry_msgs/Twist '{}'

   # 右轮单独正转：v_left=0, v_right=0.40m/s
   timeout --signal=INT 2s rostopic pub -r 20 /cmd_vel geometry_msgs/Twist \
     '{linear: {x: 0.2}, angular: {z: 2.222222}}'
   rostopic pub -1 /cmd_vel geometry_msgs/Twist '{}'
   ```

7. 停止全部实物功能节点：
   **执行位置：PC**

   ```bash
   bash ~/ROS/src/robot_bringup/scripts/robot_start.sh stop
   ```
8. 确认停止后检查：`rostopic echo /odom -n1` 中 `linear.x` 和 `angular.z` 都为 0。
9. 手动转轮子确认编码器方向，再让车轮着地正式运行。

### 5.1 实机验证基线（2026-08-04）

验证拓扑：PC (`10.80.147.11`, ROS Master) → Wi-Fi → J1900 (`10.80.147.211`) →
USB CP2102 → ESP32。

| 检查项 | 实测结果 |
|--------|----------|
| rosserial 发布缓冲 | 1024 bytes |
| `/odom` | 约 8Hz，分轮测试期间序号无丢失 |
| `/imu` | 约 4Hz |
| 左轮单独点动 2s | 编码器估算约 0.224m，峰值约 0.243m/s |
| 右轮单独点动 2s | 编码器估算约 0.188m，峰值约 0.182m/s |
| 启动死区补偿 | `MIN_START_PWM=450`，用于提高落地起步和原地转向扭矩 |
| 停车 | 零命令立即清 PWM 并拉低 STBY；微小编码器残速不重新驱动电机 |

`450 PWM` 是当前实车落地标定值，会同时提高直行和转向的最低驱动力。若更换电机、轮胎、
电池或车重，需要重新标定，不能直接沿用。

---

## 6. 仿真操作（PC 单机）

本章所有命令的**执行位置均为 PC**，J1900 和实车硬件不参与仿真。

> **⚠️ 每个新终端必须先执行：`source ~/ROS/devel/setup.bash`**
> 否则 `rosrun`/`roslaunch` 会报 `package not found`。
>
> 卡住了？重新运行 `sim_*.sh` 会自动清理旧 Gazebo/ROS 进程；或手动
> `killall -9 gzserver gzclient roslaunch rosmaster rviz`（仿真模式专用，勿在实物模式使用）。

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

所有正式功能都从 **PC** 执行。`robot_start.sh` 会自动启动 roscore、通过免密 SSH 切换
J1900 硬件模式、等待真实话题、停止上一个互斥功能，然后启动 PC 节点。

> **位置规则：** 本章所有 `robot_start.sh`、`save_map.sh`、`rostopic`、RViz 和配置编辑
> 命令均在 **PC** 终端执行。J1900 只连接 ESP32、雷达和摄像头，由 PC 脚本通过 SSH
> 自动管理；除底层排障外，不需要在 J1900 手动启动节点。

截至 2026-08-04，一键启动实机验证状态如下：

| 功能 | 命令 | 实测状态 |
|------|------|----------|
| SLAM 建图 | `robot_start.sh slam` | 已通过：底盘、雷达、EKF、gmapping、TF、地图和键盘控制正常 |
| 人体跟踪 | `robot_start.sh follow` | 已通过：摄像头、YOLO、雷达融合、跟随控制和丢失目标停车正常 |
| 多点巡航 | `robot_start.sh patrol` | 部分通过：导航链可一键启动；尚未使用合格实地图和真实安全点完成落地巡航 |
| 状态检查 | `robot_start.sh status` | 已通过 |
| 安全停止 | `robot_start.sh stop` | 已通过：先发送零速度，再停止 PC 与 J1900 功能节点 |

因此，目前只有多点巡航的真实路线行驶未完成最终实机验证，不能将其标记为完整通过。

任何时候需要停车并关闭功能节点：

**执行位置：PC**

```bash
bash ~/ROS/src/robot_bringup/scripts/robot_start.sh stop
```

### 7.1 SLAM 建图

**执行位置：PC**

```bash
bash ~/ROS/src/robot_bringup/scripts/robot_start.sh slam
```

启动成功后会在 PC 自动打开 RViz 实时显示地图、激光和机器人位置，同时打开键盘窗口。
`i` 前进，`j/l` 原地转向，`k` 或空格停车。脚本只有在 `/odom`、`/imu`、`/scan` 和
`/map` 都有真实数据后才报告成功。

保存地图：

**执行位置：PC**（地图保存到 PC 的 `~/maps/`）

```bash
bash ~/ROS/src/robot_bringup/scripts/save_map.sh lab_map
```

### 7.2 多点巡航

前提：已保存地图，并确认机器人初始位置与地图坐标一致。首次使用必须编辑实机巡航点；
仓库模板故意保持为空，避免未经确认的坐标让实车自动运动：

**执行位置：PC**

```bash
nano ~/ROS/src/robot_bringup/config/patrol_goals.yaml
```

格式为地图坐标 `[x(m), y(m), yaw(rad)]`，至少两个点：

```yaml
goals:
  - [0.0, 0.0, 0.0]
  - [1.0, 0.0, 0.0]
```

确认各点位于地图可通行区域后，一键循环巡航：

**执行位置：PC**

```bash
bash ~/ROS/src/robot_bringup/scripts/robot_start.sh patrol
```

指定其他地图和巡航点文件：

**执行位置：PC**

```bash
bash ~/ROS/src/robot_bringup/scripts/robot_start.sh patrol \
  ~/maps/other_map.yaml ~/path/to/other_goals.yaml
```

### 7.3 人体跟踪

一键启动 J1900 的 ESP32、雷达和摄像头压缩流，以及 PC 的 YOLOv8n 人体检测与
视觉+雷达融合控制：

**执行位置：PC**

```bash
bash ~/ROS/src/robot_bringup/scripts/robot_start.sh follow
```

当前摄像头为倒置安装，检测节点默认在 PC 上将画面旋转 `180°` 后再执行 YOLO、计算人体
偏角并发布叠加画面。以后改为正装时使用：

```bash
bash ~/ROS/src/robot_bringup/scripts/robot_start.sh follow rotate_180:=false
```

视觉确定人体方向，雷达聚类确定距离；雷达数据过期时只允许转向，不会使用旧距离前冲。
默认跟随距离为 `1.0m`，可覆盖参数：

**执行位置：PC**

```bash
bash ~/ROS/src/robot_bringup/scripts/robot_start.sh follow follow_dist:=1.2 hfov:=70
```

调试检测结果：

**执行位置：PC**

```bash
rostopic echo /person_visible
rostopic echo /person_angle
bash ~/ROS/tools/view_detection.sh
```

### 7.4 EKF 传感器融合（已内置，自动启动）

EKF 融合 ESP32 的 `/odom`（编码器）和 `/imu`（MPU6050），输出 `/odometry/filtered` 与 odom→base_footprint TF。

**节点定义在共享文件 `odom_ekf.launch` 中（单一来源）**，由 `ekf.launch` / `slam.launch` / `navigation.launch` 三者 include，无需手动启动。

> ⚠️ **勿同时启动多个含 EKF 的 launch**——同名节点 `ekf_localization` 后注册者会抢占踢掉先注册者，期间 TF 短暂中断，AMCL 可能失锁。

如需单独调试：

**执行位置：PC**

```bash
roslaunch robot_bringup ekf.launch
rostopic echo /odometry/filtered -n1
```

---

## 8. 快速参考

实物模式不要使用会连 roscore 一起杀掉的暴力 `killall` 方式；统一使用
`robot_start.sh stop`，它会先发送零速度，再停止 PC 与 J1900 功能节点。

### 仿真

| 在哪 | 命令 | 功能 |
|------|------|------|
| PC | `bash ~/ROS/src/robot_sim/scripts/sim_slam.sh` | SLAM 建图 |
| PC | `bash ~/ROS/src/robot_sim/scripts/sim_navigation.sh ~/maps/sim_map.yaml` | 导航 |
| PC | `bash ~/ROS/src/robot_sim/scripts/sim_follow.sh` | 激光跟随 |

### 实物

| 在哪 | 命令 | 功能 |
|------|------|------|
| PC | `bash ~/ROS/src/robot_bringup/scripts/robot_start.sh slam` | 一键 SLAM + 键盘窗口 |
| PC | `bash ~/ROS/src/robot_bringup/scripts/save_map.sh lab_map` | 保存地图 |
| PC | `bash ~/ROS/src/robot_bringup/scripts/robot_start.sh patrol` | 一键循环多点巡航 |
| PC | `bash ~/ROS/src/robot_bringup/scripts/robot_start.sh follow` | 一键视觉+雷达人体跟踪 |
| PC | `bash ~/ROS/src/robot_bringup/scripts/robot_start.sh status` | 查看 PC/J1900 状态 |
| PC | `bash ~/ROS/src/robot_bringup/scripts/robot_start.sh stop` | 发零速度并停止全部实物功能节点 |

仅在排查 J1900 时直接使用底层脚本。

**执行位置：J1900**（先执行 `ssh lawliet@lawliet.local` 登录 J1900）：

```bash
bash ~/ROS/src/robot_bringup/scripts/j1900_start.sh base
bash ~/ROS/src/robot_bringup/scripts/j1900_start.sh vision
bash ~/ROS/src/robot_bringup/scripts/j1900_start.sh status
bash ~/ROS/src/robot_bringup/scripts/j1900_start.sh stop
```

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
| 一键脚本提示无法免密 SSH | 执行 `ssh-copy-id lawliet@lawliet.local`，再用 `ssh -o BatchMode=yes lawliet@lawliet.local true` 验证 |
| J1900 连不上 PC | 互 ping；一键脚本会把当前 `PC_IP` 传给 J1900，也可手动执行 `PC_IP=<地址> robot_start.sh ...` |
| `/serial_node` 存在但 `/odom` 无新消息 | 先检查 `/tmp/esp32.log` 必须显示 `publish buffer size is 1024 bytes`；512-byte 缓冲无法容纳 718-byte `/odom` |
| rosserial 日志反复出现 `wrong checksum for topic id and msg` | 这是 ESP32→J1900 串口大帧损坏，不是 ROS 网络或 I2C 卡死。确认固件与 `j1900_start.sh` 都使用 115200，检查 CP2102、USB 线、USB Hub 和供电；不要只改上位机波特率 |
| `/odom` 有时有、随后两个 `/serial_node` 互相掉线 | PC 和 J1900 同时启动了串口桥接；全网只能保留一个 `/serial_node` |
| 插拔 USB 后串口打不开或读到雷达乱码 | 用 `/dev/serial/by-id` 或驱动识别设备，不要依赖 `ttyUSB0/1` 编号 |
| PC 能看到话题但 J1900 数据回不来 | PC 的 `ROS_IP` 不能是 `127.0.0.1`，应设置为 Wi-Fi 地址（当前为 `10.80.147.11`） |
| 巡航入口提示至少需要 2 个点 | 编辑 `config/patrol_goals.yaml`，只填写实机地图中已确认可通行的坐标 |
| 巡航启动后定位不准 | 机器人起点必须与地图初始位姿一致；否则先用 RViz 的 `2D Pose Estimate` 校正，再启动巡航 |
| 左右轮架空启动速度差异明显 | 先检查供电/接线；当前 `MIN_START_PWM=450` 是落地标定值，更换负载后需重标 |
| rosrun/roslaunch 报 package not found | `source ~/ROS/devel/setup.bash`（每个新终端都要执行一次） |
| Gazebo 打不开 | `export SVGA_VGPU10=0` |
| 一键启动后超时 | 查看 `~/.robot_logs/`；J1900 硬件日志位于 `/tmp/esp32.log`、`/tmp/lidar.log`、`/tmp/cam.log` |
| 导航、跟踪、键盘能同时跑吗 | 不能；它们都会发布 `/cmd_vel`。`robot_start.sh` 切换模式前会先发零速度并停止旧模式 |
| 导航偶发选路不优（[issue #1](https://github.com/lawliet206/ROS/issues/1)） | 属调参问题而非功能故障。先确认地图/代价地图正确：`rviz` 中查看 `/map` 与 footprint；再试调 `navigation.launch` 中 global/local costmap 的 `inflation_radius`（当前 0.20）与 `robot_radius`；TEB 的 `min_obstacle_dist` 若过大也会绕远。改前记录原参数，一次只改一个 |
| TEB 速度收敛慢/抖动（[issue #2](https://github.com/lawliet206/ROS/issues/2)） | 与动力学限制相关：检查 `navigation.launch` 中 TEB 的 `max_vel_x`/`acc_lim_x`/`acc_lim_theta` 是否匹配底盘实际能力；`dt_ref` 越大计算越快但精度降低；可尝试 `penalty_epsilon` 与 `oscillation_reduction` 缓解摆动。务必架空轮子验证后再落地 |
| Gazebo 出现"幽灵障碍"（[issue #3](https://github.com/lawliet206/ROS/issues/3)） | 已排除雷达自碰（sim URDF 中 laser_link 自带 0.04m 碰撞圆柱，但 `range_min=0.50` 已过滤）。优先检查：`/tf` 是否跳变（`rviz` 里看 odom→laser_link）、`odom_ekf` 是否正常、AMCL 初始位姿是否与地图一致；其次尝试移除 laser_link 的 `<collision>` 复测 |
| 实物轮子验证待补（[issue #4](https://github.com/lawliet206/ROS/issues/4)） | 固件/参数已就绪，缺真车复测数据。复测前先跑 `esp32_board_test` 逐项验证编码器/电机/IMU 方向极性，再低速（≤0.3 m/s）落地 |
