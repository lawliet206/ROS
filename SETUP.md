# ROS 两轮差速机器人 — 完整启动指南

## 0. 系统架构

```
PC (ROS Master) ←WiFi→ J1900 (车载) ←USB→ ESP32
                     │                   ├─ PWM → TB6612FNG → 左/右电机
                     │                   ├─ 中断 ← 编码器 (JGB37-520)
                     │                   └─ I2C  ← MPU6050 IMU
                     └─ USB ← 激光雷达 (S9-FSRD / YDLIDAR F2)
```

- **PC**：Ubuntu 20.04，跑 SLAM/导航/跟随算法，ROS Master
- **J1900**：车载迷你主机（Intel Celeron J1900, x86_64），Ubuntu 20.04，跑 rosserial + 激光雷达
- **ESP32**：下位机（PCNT硬件编码器 + PID + IMU），直发 ROS 里程计/IMU 消息
- **激光雷达**：S9-FSRD-V1.0 RX（扫地机拆机，AA55 协议，115200，~69Hz/39点/帧）

---

## 1. PC 安装（一次性）

```bash
sudo apt update
sudo apt install ros-noetic-desktop-full
sudo apt install ros-noetic-gazebo-ros-pkgs ros-noetic-gazebo-ros-control
sudo apt install ros-noetic-gmapping ros-noetic-move-base ros-noetic-amcl
sudo apt install ros-noetic-map-server ros-noetic-teleop-twist-keyboard
sudo apt install ros-noetic-robot-state-publisher ros-noetic-topic-tools
sudo apt install ros-noetic-robot-localization
sudo apt install mesa-utils libgl1-mesa-dri libgl1-mesa-glx
pip3 install pyserial pyyaml

cd ~/ROS
source /opt/ros/noetic/setup.bash
catkin_init_workspace src
catkin_make
source ~/ROS/devel/setup.bash
echo "source ~/ROS/devel/setup.bash" >> ~/.bashrc
```

---

## 2. J1900 安装（一次性）

x86 架构，装法和 PC 基本一样，但只装 ROS Base + 雷达驱动。

```bash
sudo apt update
sudo apt install ros-noetic-ros-base python3-serial python3-pip
sudo apt install ros-noetic-rosserial-python  # ESP32 通过 rosserial 通信
pip3 install pyserial pyyaml

# 直接从 PC 迁移整个工作空间（最省事）
# PC 上执行:
scp -r ~/ROS lawliet@J1900的IP:~/

# J1900 上执行:
cd ~/ROS
source /opt/ros/noetic/setup.bash
catkin_make

# 写入 .bashrc
echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
echo "source ~/ROS/devel/setup.bash" >> ~/.bashrc
```

---

## 3. ESP32 固件烧录

Arduino IDE 打开 `esp32_firmware/esp32_firmware.ino`：
- Tools → Board → ESP32 Dev Module
- Tools → Port → ESP32 的串口
- Tools → Upload Speed → 115200（烧录速度，不是 ROS 通信速度）
- 点 Upload

> ESP32 启动后通过 rosserial 直接发布 `/odom` 和 `/imu`，无需 serial_bridge.py。

---

## 4. 网络配置（PC ↔ J1900）

两台设备在同一 WiFi 下。

**PC ~/.bashrc：**
```bash
export ROS_MASTER_URI=http://10.222.149.11:11311
export ROS_IP=10.222.149.11
```

**J1900 ~/.bashrc：**
```bash
export ROS_MASTER_URI=http://10.222.149.11:11311  # 指向 PC
export ROS_IP=J1900的IP                           # hostname -I 查看
```

---

## 5. 首次上电安全流程

**车轮架起来（悬空），按顺序测试：**

1. 万用表确认 TB6612FNG VM 电压 = 电池电压（~11-12V）
2. 电池上电 → ESP32 亮灯
3. J1900 启动 rosserial 节点，看 ESP32 是否连接：
   ```bash
   rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800
   ```
   看到 `[INFO] ROS Serial Python Node connected on /dev/ttyUSB0` 表示成功
4. PC 上发布速度指令测试电机：
   ```bash
   rostopic pub /cmd_vel geometry_msgs/Twist '{linear: {x: 0.3}}' -r 1
   # 按 Ctrl-C 停止
   rostopic pub /cmd_vel geometry_msgs/Twist '{}' -1
   ```
5. 检查 `/odom` 和 `/imu` 话题是否有数据：
   ```bash
   rostopic echo /odom -n1
   ```
6. 手动转轮子，看 `/odom` 里程计是否变化
7. 全部正常 → 轮子着地 → 正式运行

---

## 6. 仿真操作（PC 单机）

### 6.1 SLAM 建图仿真

```bash
bash ~/ROS/src/robot_sim/scripts/sim_slam.sh
# 另开终端
rosrun teleop_twist_keyboard teleop_twist_keyboard.py
# RViz（可选）
rviz
```

保存地图：
```bash
mkdir -p ~/maps
rosrun map_server map_saver -f ~/maps/sim_map
```

**删除地图：**
```bash
rm ~/maps/sim_map.yaml ~/maps/sim_map.pgm
```

---

### 6.2 导航仿真

前提：已建图并保存。

```bash
# 终端1: 启动导航（自动打开 Gazebo + RViz）
bash ~/ROS/src/robot_sim/scripts/sim_navigation.sh ~/maps/sim_map.yaml

# 终端2: 一键巡点（目标点在 sim_goals.yaml 中定义）
rosrun robot_bringup send_goals.py _goal_file:=$HOME/ROS/src/robot_sim/config/sim_goals.yaml

# 或手动发送特定目标点:
rosrun robot_bringup send_goals.py _goals:="[(1.0,2.0,0.0),(3.0,4.0,1.57),(5.0,1.0,0.0)]"
# 格式: [(x, y, yaw_弧度), ...]
```

**手动导航：** RViz 已自动打开，直接用 "2D Nav Goal" 点目标位置。

**在 RViz 中查看规划路径：**
1. **Add → Map**，Topic 选 `/map` → 显示地图
2. **Add → RobotModel** → 显示机器人模型
3. **Add → Path**，Topic 选 `/move_base/GlobalPlanner/plan` → 显示全局路径（绿色）
4. **Add → Path**，Topic 选 `/move_base/TebLocalPlannerROS/local_plan` → 显示 TEB 局部路径（红色）
5. **Add → Map**，Topic 选 `/move_base/global_costmap/costmap` → 显示全局代价地图
6. **Add → Map**，Topic 选 `/move_base/local_costmap/costmap` → 显示局部代价地图
7. **Add → PoseArray**，Topic 选 `/amcl/particles` → 显示 AMCL 粒子

或者直接用预配置的 RViz：
```bash
rviz -d ~/ROS/src/robot_sim/rviz/navigation.rviz
```

---

### 6.3 人体跟随仿真

```bash
bash ~/ROS/src/robot_sim/scripts/sim_follow.sh
```

---

## 7. 实物启动流程

### 硬件接线

**供电：**
- 电池 12V 正极 → TB6612FNG VM 引脚（电机供电，注意 VM 范围 2.7V-10.8V，可能需降压）
- 电池 12V 正极 → LM2596 降压模块输入（输出调至 5.0V）→ ESP32 VIN 引脚
- 电池 12V 负极 → 所有设备 GND 共地（TB6612 / ESP32 / 编码器 / MPU6050）

**TB6612FNG 引脚定义（红色小板）：**

左侧（从上到下）：PWMA, AIN2, AIN1, STBY, BIN1, BIN2, PWMB, GND

右侧（从上到下）：VM, VCC, GND, AO1, AO2, BO2, BO1, GND

**TB6612FNG ↔ ESP32：**

| TB6612 引脚 | 接 ESP32 | 说明 |
|---|---|---|
| PWMA | GPIO 18 | 左电机 PWM 调速 |
| AIN2 | GPIO 26 | 左电机方向 2 |
| AIN1 | GPIO 25 | 左电机方向 1 |
| STBY | GPIO 4 | 接高电平使能 |
| BIN1 | GPIO 32 | 右电机方向 1 |
| BIN2 | GPIO 33 | 右电机方向 2 |
| PWMB | GPIO 19 | 右电机 PWM 调速 |
| VM | 电池 12V 正极 | 电机供电（注意耐压） |
| VCC | 3.3V | 逻辑供电 |
| GND | 电池负极 | 共地 |
| AO1 / AO2 | 左电机 M1 / M2 | 左路电机输出 |
| BO1 / BO2 | 右电机 M1 / M2 | 右路电机输出 |

**JGB37-520 电机 6pin 接口**（按线序 M1-红, GND-黑, B-黄, A-绿, Vcc-蓝, M2-白）：

| 线色 | 引脚 | 左电机接 | 右电机接 |
|---|---|---|---|
| 红 | 1 - M1（马达-） | TB6612 AO1 | TB6612 BO1 |
| 黑 | 2 - GND | GND 共地 | GND 共地 |
| 黄 | 3 - B（信号2） | GPIO 23 | GPIO 13 |
| 绿 | 4 - A（信号1） | GPIO 27 | GPIO 14 |
| 蓝 | 5 - Vcc | 3.3V | 3.3V |
| 白 | 6 - M2（马达+） | TB6612 AO2 | TB6612 BO2 |

> 改变 M1/M2 接线正负极可改变电机转向。编码器 A/B 相用 ESP32 内部上拉即可。

**MPU6050 ↔ ESP32：**

| MPU6050 | ESP32 | 说明 |
|---|---|---|
| VCC | 3.3V | 供电 |
| GND | GND | 共地 |
| SDA | GPIO 21 | I2C 数据（内部上拉） |
| SCL | GPIO 22 | I2C 时钟（内部上拉） |
| AD0 | GND | I2C 地址 = 0x68 |

**电池电压检测：**

电池 12V 正极 → 10kΩ ──┬── GPIO 34
                        │
                       1kΩ
                        │
                       GND

> 分压比 11:1，12V 电池最高约 12.6V → GPIO 约 1.15V（安全范围内）
```

### 串口权限（J1900 上一次性）

```bash
echo 'KERNEL=="ttyUSB*", MODE="0666"' | sudo tee /etc/udev/rules.d/99-usb-serial.rules
sudo udevadm control --reload-rules
```

### 每次开机确认

```bash
ls /dev/ttyUSB*   # ESP32 → ttyUSB0, 雷达 → ttyUSB1
```

---

### 场景 A：SLAM 建图

| # | 在哪 | 终端 | 命令 |
|---|------|------|------|
| 1 | **PC** | 终端1 | `source ~/ROS/devel/setup.bash && roscore` |
| 2 | **J1900** | SSH终端1 | `source ~/ROS/devel/setup.bash && rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800` |
| 3 | **J1900** | SSH终端2 | 雷达驱动（见下方） |
| 4 | **PC** | 终端2 | `source ~/ROS/devel/setup.bash && roslaunch robot_bringup slam.launch start_lidar:=false` |
| 5 | **PC** | 终端3 | `source ~/ROS/devel/setup.bash && rosrun teleop_twist_keyboard teleop_twist_keyboard.py` |
| 6 | **PC** | 终端4 | `source ~/ROS/devel/setup.bash && rviz`（可选） |

**雷达驱动（步骤3，选一个）：**
```bash
# S9-FSRD (扫地机拆机, 自带驱动):
rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB1
# YDLIDAR F2:
roslaunch ydlidar_ros F2.launch
# RPLIDAR:
roslaunch rplidar_ros rplidar.launch
```

**启动顺序：** 1 → 2 → 3 → 4 → 5

**键盘遥控：** `i` 前进、`j` 左转、`l` 右转、`,` 后退、`k` 停止

**建图完保存：**
```bash
mkdir -p ~/maps
rosrun map_server map_saver -f ~/maps/lab_map
```

**删除地图（重新建图）：**
```bash
rm ~/maps/lab_map.yaml ~/maps/lab_map.pgm
```

**确认里程计正常工作：**
```bash
rostopic echo /odom -n1  # 看 x, y, yaw 是否随小车移动变化
```

---

### 场景 B：多点导航

前提：已建图保存到 `~/maps/`。

| # | 在哪 | 终端 | 命令 |
|---|------|------|------|
| 1 | **PC** | 终端1 | `source ~/ROS/devel/setup.bash && roscore` |
| 2 | **J1900** | SSH终端1 | `source ~/ROS/devel/setup.bash && rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800` |
| 3 | **J1900** | SSH终端2 | `source ~/ROS/devel/setup.bash && rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB1` |
| 4 | **PC** | 终端2 | `source ~/ROS/devel/setup.bash && bash ~/ROS/src/robot_bringup/scripts/nav_start.sh ~/maps/lab_map.yaml` |
| 5 | **PC** | 终端3 | `source ~/ROS/devel/setup.bash && rosrun robot_bringup send_goals.py _goals:="[(2.0,0.0,0.0),(4.0,2.0,1.57)]"` |
| 6 | **PC** | 终端4 | `source ~/ROS/devel/setup.bash && rviz`（可选） |

**多点导航：**
```bash
rosrun robot_bringup send_goals.py _goals:="[(1,2,0), (3,4,1.57), (5,5,0)]"
# 格式: [(x, y, yaw弧度), ...]
```

**自定义 YAML 文件：**
```bash
cat > ~/maps/nav_goals.yaml << EOF
goals:
  - [1.0, 2.0, 0.0]
  - [3.0, 4.0, 1.57]
  - [0.0, 0.0, 0.0]
EOF
rosrun robot_bringup send_goals.py _goal_file:=~/maps/nav_goals.yaml
```

或在 RViz 用 "2D Nav Goal" 手动点目标。

---

### 场景 C：人体跟随

| # | 在哪 | 终端 | 命令 |
|---|------|------|------|
| 1 | **PC** | 终端1 | `source ~/ROS/devel/setup.bash && roscore` |
| 2 | **J1900** | SSH终端1 | `source ~/ROS/devel/setup.bash && rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800` |
| 3 | **J1900** | SSH终端2 | `source ~/ROS/devel/setup.bash && rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB1` |
| 4 | **PC** | 终端2 | `source ~/ROS/devel/setup.bash && roslaunch robot_bringup follow.launch start_lidar:=false` |

人在车前走，车自动跟。

---

## 8. 一键脚本速查

### 仿真（PC 单机）

| 步骤 | 命令 | 功能 |
|------|------|------|
| 1 | `bash ~/ROS/src/robot_sim/scripts/sim_slam.sh` | Gazebo 建图 |
| 2 | `rosrun map_server map_saver -f ~/maps/sim_map` | 保存地图 |
| 3 | `bash ~/ROS/src/robot_sim/scripts/sim_navigation.sh ~/maps/sim_map.yaml` | 导航 |
| 4 | `rosrun robot_bringup send_goals.py _goal_file:=$HOME/ROS/src/robot_sim/config/sim_goals.yaml` | 自动巡点 |
| 5 | `bash ~/ROS/src/robot_sim/scripts/sim_follow.sh` | 人体跟随 |

### 实物（PC + J1900 + ESP32）

> 以下命令假定 J1900 的 `~/.bashrc` 已配好 ROS_MASTER_URI 指向 PC。
> PC 的 `bash xxx.sh` 脚本自带 source，新的 rosrun 终端需先 `source ~/ROS/devel/setup.bash`。

| 步骤 | 在哪 | 命令 | 功能 |
|------|------|------|------|
| 1 | J1900 | `source ~/ROS/devel/setup.bash && rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0 _baud:=460800` | ESP32 rosserial |
| 2 | J1900 | `source ~/ROS/devel/setup.bash && rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB1` | 激光雷达 |
| 3 | PC | `bash ~/ROS/src/robot_bringup/scripts/slam_start.sh` | gmapping 建图 |
| 4 | PC | `bash ~/ROS/src/robot_bringup/scripts/save_map.sh my_map` | 保存地图 |
| 5 | PC | `bash ~/ROS/src/robot_bringup/scripts/nav_start.sh ~/maps/my_map.yaml` | 导航 |
| 6 | PC | `source ~/ROS/devel/setup.bash && rosrun robot_bringup send_goals.py _goals:="[(x,y,yaw),...]"` | 自动巡点 |
| 7 | PC | `source ~/ROS/devel/setup.bash && roslaunch robot_bringup follow.launch start_lidar:=false` | 人体跟随 |

---

## 9. 安装速查

| 设备 | 要装的 |
|------|--------|
| PC | ros-noetic-desktop-full + gmapping + move-base + amcl + map-server + teleop-twist-keyboard + robot-state-publisher + topic-tools + robot-localization |
| J1900 | ros-noetic-ros-base + rosserial-python + python3-serial，S9 驱动已在 robot_bringup 中 |
| ESP32 | Arduino IDE + ESP32 开发板支持包，刷 `esp32_firmware.ino` |

---

## 10. 常见问题

| 问题 | 解决 |
|------|------|
| `catkin_make` 报 No module named pyserial | `pip3 install pyserial pyyaml` |
| 串口无权限 | `sudo usermod -a -G dialout $USER` 然后重新登录 |
| 永久串口授权 | `echo 'KERNEL=="ttyUSB*", MODE="0666"' \| sudo tee /etc/udev/rules.d/99-usb-serial.rules && sudo udevadm control --reload-rules` |
| J1900 连不上 PC | 两台 ping 通，确认 ROS_MASTER_URI 和 ROS_IP 都指向 PC |
| `roslaunch` 找不到包 | `source ~/ROS/devel/setup.bash` 重新 source |
| Gazebo 打不开 | `echo "export SVGA_VGPU10=0" >> ~/.bashrc && source ~/.bashrc` |
