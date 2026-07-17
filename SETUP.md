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
  - [4.5 LD2402 毫米波雷达](#45-ld2402-毫米波雷达)
  - [4.6 电池电压检测](#46-电池电压检测)
  - [4.7 串口权限](#47-串口权限)
- [5. 首次上电](#5-首次上电安全流程)
- [6. 仿真操作](#6-仿真操作pc-单机)
  - [6.1 SLAM 建图](#61-slam-建图)
  - [6.2 导航](#62-导航)
  - [6.3 激光跟随](#63-激光跟随)
  - [6.4 毫米波雷达跟随](#64-毫米波雷达跟随)
  - [6.5 融合跟随](#65-融合跟随激光毫米波)
- [7. 实物操作](#7-实物操作)
  - [7.1 SLAM 建图](#71-slam-建图)
  - [7.2 多点导航](#72-多点导航)
  - [7.3 人体跟随](#73-人体跟随)
- [8. 快速参考](#8-快速参考)
- [9. 常见问题](#9-常见问题)

---

## 1. 系统架构

```
PC (ROS Master) ←WiFi→ J1900 (车载) ←USB→ ESP32
                     │                   ├─ PWM → TB6612FNG → 左/右电机
                     │                   ├─ PCNT ← 编码器 (JGB37-520)
                     │                   └─ I2C  ← MPU6050 IMU
                     ├─ USB ← 激光雷达 (S9-FSRD / YDLIDAR F2)
                     └─ USB ← 毫米波雷达 (HLK-LD2402, 24GHz)
```

| 设备 | 说明 |
|------|------|
| **PC** | Ubuntu 20.04，SLAM/导航/跟随，ROS Master |
| **J1900** | 车载 x86_64，Ubuntu 20.04，rosserial + 雷达驱动 |
| **ESP32** | 下位机，PCNT 编码器 + PID + IMU，rosserial |
| **激光雷达** | S9-FSRD-V1.0 RX，AA55 协议，115200 |
| **毫米波雷达** | HLK-LD2402，24GHz，±60°，接 J1900 USB |

---

## 2. 环境安装

### 2.1 PC（一次性）

```bash
sudo apt install ros-noetic-desktop-full
sudo apt install ros-noetic-gazebo-ros-pkgs ros-noetic-gazebo-ros-control
sudo apt install ros-noetic-gmapping ros-noetic-move-base ros-noetic-amcl
sudo apt install ros-noetic-map-server ros-noetic-teleop-twist-keyboard
sudo apt install ros-noetic-robot-state-publisher ros-noetic-topic-tools
sudo apt install ros-noetic-robot-localization
sudo apt install ros-noetic-teb-local-planner
sudo apt install mesa-utils libgl1-mesa-dri libgl1-mesa-glx
pip3 install pyserial pyyaml

cd ~/ROS && source /opt/ros/noetic/setup.bash
catkin_init_workspace src && catkin_make
source ~/ROS/devel/setup.bash
echo "source ~/ROS/devel/setup.bash" >> ~/.bashrc
```

### 2.2 J1900（一次性）

```bash
sudo apt install ros-noetic-ros-base python3-serial python3-pip
sudo apt install ros-noetic-rosserial-python
pip3 install pyserial pyyaml

# 从 PC 迁移工作空间
# PC 上: scp -r ~/ROS lawliet@<J1900_IP>:~/
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

```bash
# PC ~/.bashrc
export ROS_MASTER_URI=http://10.222.149.11:11311
export ROS_IP=192.168.1.118

# J1900 ~/.bashrc
export ROS_MASTER_URI=http://10.222.149.11:11311
export ROS_IP=<J1900的IP>   # hostname -I 查看
```

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

### 4.5 LD2402 毫米波雷达

接 J1900 的 USB（不接 ESP32）。

| LD2402 | USB-UART |
|--------|----------|
| VCC | 3.3V |
| GND | GND |
| TX | RX |
| RX | TX（可选） |

> 上电后需等待 30-60s 初始化。波特率 115200 8N1。

### 4.6 电池电压检测

```
电池 12V → 10kΩ ─┬─ GPIO34 (分压 ≈ 1.15V @ 12.6V)
                1kΩ
                 │
                GND
```

### 4.7 串口权限

```bash
echo 'KERNEL=="ttyUSB*", MODE="0666"' | sudo tee /etc/udev/rules.d/99-usb-serial.rules
sudo udevadm control --reload-rules
```

每次开机确认：
```bash
ls /dev/ttyUSB*   # USB0=ESP32  USB1=激光雷达  USB2=LD2402
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

### 6.4 毫米波雷达跟随

模拟 LD2402，纯距离跟踪（不转弯）。

```bash
# 终端1
bash ~/ROS/src/robot_sim/scripts/sim_follow_radar.sh

# 终端2
rosrun robot_bringup radar_follower.py _sim_mode:=true
```

### 6.5 融合跟随（激光 + 毫米波）

激光提供角度（转弯），雷达提供距离。**能转弯跟踪**。

```bash
# 终端1
bash ~/ROS/src/robot_sim/scripts/sim_follow_radar.sh

# 终端2
rosrun robot_bringup fusion_follower.py
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
| 4 | PC | `roslaunch robot_bringup slam.launch start_lidar:=false` |
| 5 | PC | `rosrun teleop_twist_keyboard teleop_twist_keyboard.py`（键盘 `i` 前进 `k` 停） |

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

**方案一：纯激光**

| # | 设备 | 命令 |
|---|------|------|
| 1 | PC | `roscore` |
| 2 | J1900 | `rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800` |
| 3 | J1900 | `rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB1` |
| 4 | PC | `roslaunch robot_bringup follow.launch sensor:=laser start_lidar:=false` |

**方案二：纯毫米波**（LD2402，不转弯，只前进/后退）

| # | 设备 | 命令 |
|---|------|------|
| 1 | PC | `roscore` |
| 2 | J1900 | `rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800` |
| 3 | J1900 | `roslaunch robot_bringup follow.launch sensor:=radar radar_port:=/dev/ttyUSB2` |

**方案三：激光 + 毫米波融合**（推荐，能转弯）

| # | 设备 | 命令 |
|---|------|------|
| 1 | PC | `roscore` |
| 2 | J1900 | `rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800` |
| 3 | J1900 | `roslaunch robot_bringup follow.launch sensor:=fusion lidar_port:=/dev/ttyUSB1 radar_port:=/dev/ttyUSB2` |

> 激光（红外光）和毫米波（24GHz 射频）物理频段不同，互不干扰。

**融合跟随可调参数：**

| 参数 | 默认 | 说明 |
|------|------|------|
| `target_dist` | 1.0 | 目标保持距离 (m) |
| `max_linear` | 0.5 | 最大线速度 (m/s) |
| `kp_linear` | 0.4 | 距离 P 增益 |
| `kp_angular` | 0.5 | 角度 P 增益 |
| `deadzone` | 0.2 | 死区 (m) |
| `dist_filter_alpha` | 0.3 | 距离 EMA 滤波 |

---

## 8. 快速参考

> 卡住/报错时先清理残留：`bash ~/ROS/tools/kill_ros.sh`

### 仿真

| 命令 | 功能 |
|------|------|
| `bash ~/ROS/src/robot_sim/scripts/sim_slam.sh` | SLAM 建图 |
| `bash ~/ROS/src/robot_sim/scripts/sim_navigation.sh ~/maps/sim_map.yaml` | 导航 |
| `bash ~/ROS/src/robot_sim/scripts/sim_follow.sh` | 激光跟随 |
| `bash ~/ROS/src/robot_sim/scripts/sim_follow_radar.sh` | 雷达仿真 |
| `rosrun robot_bringup fusion_follower.py` | 融合跟随仿真 |

### 实物

| 在哪 | 命令 | 功能 |
|------|------|------|
| J1900 | `rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800` | ESP32 |
| J1900 | `rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB1` | 激光雷达 |
| PC | `roslaunch robot_bringup slam.launch start_lidar:=false` | SLAM 建图 |
| PC | `rosrun map_server map_saver -f ~/maps/lab_map` | 保存地图 |
| PC | `bash ~/ROS/src/robot_bringup/scripts/nav_start.sh ~/maps/lab_map.yaml` | 导航 |
| PC | `roslaunch robot_bringup follow.launch sensor:=laser` | 激光跟随 |
| PC | `roslaunch robot_bringup follow.launch sensor:=radar radar_port:=/dev/ttyUSB2` | 毫米波跟随 |
| PC | `roslaunch robot_bringup follow.launch sensor:=fusion` | 融合跟随 |

---

## 9. 常见问题

| 问题 | 解决 |
|------|------|
| pyserial 找不到 | `pip3 install pyserial pyyaml` |
| 串口无权限 | `sudo usermod -a -G dialout $USER` 重新登录 |
| J1900 连不上 PC | 互 ping，确认 ROS_MASTER_URI / ROS_IP 指向 PC |
| rosrun/roslaunch 报 package not found | `source ~/ROS/devel/setup.bash`（每个新终端都要执行一次） |
| Gazebo 打不开 | `export SVGA_VGPU10=0` |
| LD2402 上电无数据 | 需等 30-60s 初始化 |
| 激光和毫米波冲突？ | 不冲突——红外光 vs 24GHz 射频，不同物理频段 |
