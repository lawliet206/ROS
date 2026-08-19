# AGENTS.md

本文件是给 Coding Agent（Claude Code / Cursor / OpenCode 等）的项目级指南。
目标：让 Agent 在不破坏现有功能的前提下，正确、安全地修改本仓库。

## 项目是什么

**ROS Noetic + ESP32 + J1900 + LiDAR 两轮差速自主移动机器人平台**。
- 三机架构：**PC**（ROS Master，跑 SLAM/导航/视觉）←WiFi→ **J1900**（车载，雷达驱动 + 摄像头采集）←USB→ **ESP32**（电机 PID + 编码器里程计 + IMU，rosserial）。
- 核心功能：S9 雷达 SLAM 建图（gmapping）、AMCL + move_base + TEB 多点导航、EKF 里程计/IMU 融合、视觉+雷达融合人体跟随、Gazebo 仿真。

## 环境版本（不可随意升级/降级）

| 组件 | 版本 |
|------|------|
| OS | Ubuntu 20.04 (Focal) |
| ROS | Noetic（PC 与 J1900 均安装） |
| Python | 3.8（ROS 依赖）+ 3.x（视觉依赖 ultralytics/cv2 可在 PC 单独装） |
| 固件 IDE | Arduino IDE / arduino-cli，支持 ESP32 core 2.x |
| 编译 | `catkin_make`（不用 catkin tools 的混合布局） |

## 目录结构（catkin 工作空间根 = 仓库根）

```
src/robot_bringup/   # ★ 实物包（PC 与 J1900 都要编译部署）
  launch/            # bringup / slam / navigation / follow / follow_vision / ekf / odom_ekf
  scripts/           # robot_start.sh j1900_start.sh + 6 个 Python 节点
  config/            # ekf.yaml patrol_goals.yaml slam.rviz
  urdf/robot.urdf    # 实物模型（轮距 0.180 m）
src/robot_sim/       # 仿真包（仅 PC）
  launch/            # simulation / sim_slam / sim_navigation
  scripts/           # sim_slam.sh / sim_navigation.sh / sim_follow.sh
  urdf/worlds/rviz/config
esp32_firmware/      # ESP32 固件（独立于 ROS，用 Arduino 编译）
  esp32_firmware.ino # ★ 主固件：PCNT 编码器 + 双路 PID + MPU6050 + rosserial
  esp32_board_test/  # 板级测试固件（单项验证）
  libraries/ros_lib/ # vendored rosserial Arduino 库（已修复 ESP32 ros.h 兼容，勿随意改）
tests/               # pytest 纯逻辑回归测试（协议解析 / 状态机 / 角度换算）
tools/               # 调试工具（不入部署链）
docs/                # 架构文档 / 开发指南 / 论文
```

## 重要 launch 与脚本（命名与参数以代码为准，改文档必须同步）

- 实物一键总控（PC 执行）：`bash src/robot_bringup/scripts/robot_start.sh {slam|patrol|follow|stop|status}`
- 车载一键（J1900 执行 / robot_start.sh 通过 SSH 自动调用）：`bash src/robot_bringup/scripts/j1900_start.sh {base|vision|stop|status}`
- 仿真：`bash src/robot_sim/scripts/sim_slam.sh` / `sim_navigation.sh [map.yaml]` / `sim_follow.sh`
- 关键 launch：`slam.launch`（gmapping）、`navigation.launch`（map_server+AMCL+move_base+TEB）、`follow_vision.launch`（YOLO+雷达融合跟随）、`odom_ekf.launch`（EKF 节点唯一来源，被 slam/navigation/ekf include，**不要重复定义 EKF 节点**）。
- 雷达型号切换：`bringup.launch` 的 `lidar_model` 参数（`s9_fsrd` / `ydlidar_x4` 等）。

## 编译与测试

```bash
# 编译（Ubuntu 20.04 + ROS Noetic 环境）
source /opt/ros/noetic/setup.bash
cd <仓库根>
catkin_make
source devel/setup.bash

# 单元测试（PC 上运行，不需要硬件；CI 用 ros:noetic 容器自动执行）
python3 -m pytest tests -q
```

## 硬件安全（红线，任何修改不得削弱）

- **启动/重启后小车必须保持静止**，禁止任何无条件高速运动；遥控建图默认速度 0.3 m/s 封顶。
- **`stop` 必须发送零速度**：`robot_start.sh stop` / `j1900_start.sh stop` 都通过 `rostopic pub /cmd_vel` 发布零 Twist。
- 改底盘控制（PID、PWM 映射、方向、死区）后：先架空轮子验证，再低速（≤0.3 m/s）落地测试。
- 首次/重新接线后：跑 `esp32_board_test` 分项验证编码器/电机/IMU 方向与极性。
- 串口识别依赖驱动名：CP210x=ESP32，ch341=雷达；改硬件前先确认 `detect_ports` 逻辑。
- 不要修改 EKF 的 `odom_frame/base_frame` 命名约定（odom→base_footprint），改坏会导致 AMCL 定位跳变。
- ROS Master 只绑定局域网（PC_IP），不要暴露到不可信网络；`j1900_start.sh` 已做 rosserial.service 冲突检测。

## 修改规范

- **可随意改**：README.md、docs/、tools/、tests/、config 参数调优、launch 参数。
- **谨慎改（先理解再动）**：`s9_lidar_driver.py`（协议解析被 15 个测试锁定）、`vision_follower.py` / `person_detector.py`（状态机被 12 个测试锁定）、`robot_start.sh` / `j1900_start.sh`（一键流程被 README/SETUP 引用）。
- **不要随意改**：`esp32_firmware/libraries/ros_lib/`（vendored 官方库+本地修复）、URDF 中的轮距/关节坐标系（与固件、launch 的 180mm 轮距强耦合）。
- 改 ROS Python 节点：必须先更新或新增对应 tests 并跑通；改 ESP32 固件：必须用 `esp32_board_test` 验证硬件行为。
- 删除文件前先 `git log -- <path>` 确认用途；`.gitignore` 已忽略 build/ devel/ logs/ docx 等生成物。

## 常见坑

- 不要删除 `tests/` 的 `sys.path.insert`（它让测试直接 import scripts 目录，无需安装）。
- 不要在本仓库根创建第二套 workspace（根就是 catkin 工作空间）。
- 雷达 `s9_lidar_driver.py` 当前发布完整 360° 扫描（非切片），gmapping 订阅 `/scan_deskewed`（relay 或 deskew 节点）。
- 论文/采购清单在 docs/ 下，属学术与采购参考，勿与源码混淆。