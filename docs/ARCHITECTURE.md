# 系统架构

> 本文档描述本项目**当前实现**的架构（与源码一致，2026-08 快照）。
> 架构图: [docs/system_architecture.png](system_architecture.png)（`tools/generate_architecture.py` 可重新生成）。

## 1. 总体结构

三机分布式架构：

```
PC (ROS Master, 算力)  ←WiFi→  J1900 (车载采集)  ←USB→  ESP32 (底盘控制)
```

| 机器 | 职责 | 运行内容 |
|------|------|----------|
| **PC** | 建图/定位/规划/视觉 | roscore、gmapping、AMCL、move_base+TEB、EKF、YOLOv8n 视觉、RViz |
| **J1900** | 传感器采集 | s9_lidar_driver（雷达）、usb_cam（摄像头）、rosserial_python（ESP32 桥） |
| **ESP32** | 底盘执行 | 双路 PID 速度闭环、PCNT 编码器里程计、MPU6050 IMU、看门狗、堵转保护 |

一键总控: PC 执行 `robot_start.sh {slam|patrol|follow|stop|status}`，
通过 SSH 联动 J1900 执行 `j1900_start.sh {base|vision|stop|status}`。

## 2. 数据流

### 2.1 SLAM / 导航链路

```
LiDAR ─(USB, 115200, AA55)─> s9_lidar_driver ─/scan─> scan_deskew ─/scan_deskewed─> gmapping
                                                                                      │
                                               move_base+TEB <─ /map ────────────────┘
                                                   │  (AMCL 订阅 /scan_deskewed + /map)
                                                   ▼
                                              /cmd_vel ─> rosserial_python ─(USB)─> ESP32 PID ─PWM─> TB6612FNG ─> 电机
```

里程计/IMU 链路：

```
ESP32 PCNT 编码器 ─> rosserial /odom ─┐
MPU6050 IMU       ─> rosserial /imu  ─┼─> robot_localization EKF ─> /odometry/filtered
                                      │                               (odom→base_footprint TF)
ESP32 发布节流 (odom 8Hz + IMU 4Hz 错峰) ─┘
```

### 2.2 视觉跟随链路

```
usb_cam ─/image_raw─> republish ─/image_raw/compressed─> person_detector (YOLOv8n)
                                                          │ 输出 /person_angle + /person_visible + 可视化
                                                          ▼
      laser_follower (/scan 距离) ─┐                 vision_follower (3态状态机: SEARCH/FOLLOW/ROTATE)
                                   └─> 融合决策 ────────┘
                                                          │
                                                          ▼
                                                     /cmd_vel
```

- 视觉定方向（最中央人选），雷达定距离（保持 0.5~1.2m）。
- 雷达断流保护：只转不前进，防盲目前冲。
- 雷达丢失保护：vision_follower 在雷达数据超时时降级为纯旋转搜索。

## 3. TF 树

```
map ──> odom ──> base_footprint ──> base_link
                                    ├──> laser_link   (激光, 安装偏航并入 angle_offset, URDF 无旋转)
                                    ├──> imu_link     (IMU, R_z(-π/2) 与 URDF 对齐)
                                    └──> camera_link  (摄像头, 倒置支持 180° 旋转)
```

> 约定不可更改: EKF 输出 `odom→base_footprint`。轮距 180mm 在 URDF/固件/launch 中强耦合。

## 4. 节点清单

### robot_bringup（实物包，PC 与 J1900 均部署）

| 节点/脚本 | 位置 | 说明 |
|-----------|------|------|
| `s9_lidar_driver.py` | J1900 | S9-FSRD AA55 协议解析、定长帧提取、360° 缓冲、跨零发布、镜像+offset 角度标定、距离 /4000 标定 |
| `scan_deskew.py` | PC | IMU 角速度激光去畸变，stamp 改扫描起始时刻，time_increment 置 0 |
| `send_goals.py` | PC | 多点巡航目标发送（YAML/参数加载，Action 重试+超时） |
| `laser_follower.py` | PC | 纯雷达人体跟随（宽度约束 0.15~0.55m + 连续锁定） |
| `person_detector.py` | PC | YOLOv8n 人体检测，最中央人选帧 + 角度换算，CPU 推理 |
| `vision_follower.py` | PC | 视觉+雷达融合三态状态机，摇摆搜索 |
| `robot_start.sh` | PC | 一键总控（自动 roscore、SSH 联动、stop 发零速、进程清理） |
| `j1900_start.sh` | J1900 | 车载一键（base/vision 模式、topic 健康检查、动态端口检测 CP210x/ch341、rosserial.service 冲突检测） |

### robot_sim（仿真包，仅 PC）

| 内容 | 说明 |
|------|------|
| `simulation.launch` | Gazebo 空房间 + 差速插件 + 同款 URDF |
| `sim_slam.sh` / `sim_navigation.sh` / `sim_follow.sh` | 仿真一键流程，复用实物导航栈与 laser_follower |

## 5. Launch 组织

| launch | 包含 | 说明 |
|--------|------|------|
| `bringup.launch` | lidar 驱动(按 `lidar_model`: s9_fsrd/ydlidar_x4) + ESP32 rosserial | 车载基础 |
| `odom_ekf.launch` | robot_localization EKF | **EKF 唯一来源**，被 slam/navigation/ekf include，不要重复定义 |
| `slam.launch` | bringup + odom_ekf + gmapping + RViz (+ 可选 deskew) | 建图 |
| `navigation.launch` | map_server + AMCL + move_base + TEB + odom_ekf | 导航 |
| `follow_vision.launch` | 视觉+雷达融合跟随 + odom_ekf | 跟随 |
| `follow.launch` | 纯雷达跟随 | 备用 |

## 6. 安全机制

- 启动/重启后小车静止；遥控建图默认 0.3 m/s 封顶。
- `stop` 通过 `rostopic pub /cmd_vel` 发布零 Twist（robot_start.sh 连续发布 2s）。
- ESP32 固件: 800ms 看门狗（收不到 cmd_vel 即停）、堵转保护（CMD≥80 且实际<1 转速 5s）、零速关 STBY。
- ROS Master 仅绑定局域网 IP。

## 7. 已知协议盲区

- S9 帧头 AA55 已逆向（定长 cnt 驱动 + 帧尾边界验证），但 `cs` 校验字段为专有算法，
  黑盒实测累加/XOR/CRC 均不匹配，待官方协议文档确认。
- ESP32 串口波特率历经 460800→230400→115200 修复，当前 115200（SETUP.md 有完整排查记录）。