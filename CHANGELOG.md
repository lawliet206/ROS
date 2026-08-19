# Changelog

本项目所有条目均来自真实 git 历史（`git log --date=short`），按日期分组。
版本遵循 [SemVer](https://semver.org/lang/zh-CN/)。

## [0.1.0] - 2026-08-20

首个正式发布（对应 `v0.1.0` 标签）。自上次快照以来的变更：

- docs: README 新增自绘硬件图（原理图 + PCB 正/反面），硬件资料随仓库发布
- fix: package.xml 元数据修复 — 占位维护者邮箱 → `lawliet <lawliet206@163.com>`；版本 1.0.0 → 0.1.0
- fix: robot_sim 补全 `exec_depend robot_bringup`（sim_follow.sh 依赖）
- ci: 增加静态检查 job（compileall / bash -n / XML / YAML）+ navfn 依赖 + 并发分组
- test: 新增回归测试 25 例 — s9 缓冲/跨零（8）+ scan_deskew 去畸变/IMU 窗均值（11）+ send_goals 目标解析（6），总计 36 → 61
- fix: send_goals 畸形条目容错（原实现遇非数值条目会使节点崩溃）
- docs: 新增 OSS 基础设施 — CONTRIBUTING / SECURITY / Issue & PR 模板
- docs: 新增 ARCHITECTURE / PROJECT_STATUS / CHANGELOG，README 测试数与 AMCL 参数修正

### 2026-08-19 · 开源规范化

- docs: README / AGENTS / SETUP 重构为开源展示版 + 架构图与开发指南
- fix: 固件 rosserial 库入库（vendored），保证可复现编译；补全 package.xml 依赖
- chore: 仓库开源规范化 — .gitignore / LICENSE / CI，清理个人与过时文件

### 2026-08-10 · 巡航与实车校准

- feat: person_detector 摄像头倒置旋转 180° 支持 + follow_vision.launch 参数 + 回归测试
- feat: SLAM 一键启动自动打开 RViz 实时建图窗口 + stop 清理 rviz 进程
- fix: ESP32 死区补偿实车校准 350→450（落地起步扭矩不足）
- feat: patrol_goals 实机巡航点（RViz 选取的真实地图坐标）
- docs: SETUP 标注各步骤执行位置（PC/J1900）+ 死区 450 校准 + RViz 建图说明
- docs: 毕业设计论文《基于ROS的两轮差速移动机器人的设计与实现》（含实验图）

### 2026-08-04 · 一键总控与稳定性

- feat: PC 端一键启动 robot_start.sh（slam/patrol/follow/stop/status，SSH 联动 J1900 + 自动 roscore）+ 巡航点模板
- refactor: j1900_start.sh 拆 base/vision 模式 + topic 健康检查 + 115200 + systemctl 冲突检测
- fix: EKF sensor_timeout 0.35，防雷达/IMU 短暂断流导致定位跳变
- fix: ESP32 固件降波特率 115200 + 发布节流（odom 8Hz / IMU 4Hz 错峰）+ 零速关 STBY
- docs: README/SETUP 更新 115200 波特率与 robot_start.sh 新启动流程
- chore: gitignore 运行时日志目录 .robot_logs/

### 2026-08-03 · 视觉跟随与核心修复（大版本日）

- feat: vision_follower 视觉+雷达融合控制器（3 态状态机 + 摇摆搜索）
- feat: person_detector YOLOv8n 人体检测节点（最中央人选帧 + 角度换算）
- feat: follow_vision.launch 一键启动 + CMakeLists/SETUP 部署配置
- fix: S9 雷达驱动距离单位标定（/4000 实测）+ 角度镜像标定（offset -90）+ 定长帧解析防载荷 AA55 截断 + 15 个回归测试
- fix: 雷达丢失时只转不前进（防盲目前冲）+ IMU 四元数 pitch 符号修正
- fix: person_detector 显式 CPU 推理（device 参数），修复无 GPU 时 CUDA 崩溃
- fix: 视觉跟随雷达断流保护 + costmap 插件命名空间修复 + send_goals 类型健壮性 + 去畸变元数据
- fix: IMU 旋转方向 R_z(-π/2) 与 URDF 对齐 + 姿态四元数 roll/pitch 互换映射
- fix: 三方审查共识清单修复
- fix: ESP32 固件降波特率 230400 + 发布 33Hz 修复位错乱（460800 握手 0xef / cmd_vel 被丢弃导致电机不转）
- feat: J1900 一键启动脚本 j1900_start.sh 数字模式 + ESP32 自动复位
- feat: j1900_start 动态检测端口（CP210x=ESP32, ch341=雷达），防 USB 插拔后 ttyUSB 顺序互换
- feat: PC 端检测可视化工具（view_detection.sh + person_viewer.py）+ 持久 roscore 脚本
- fix: person_detector YOLO 离线加载防联网卡死
- fix: 串口端口实测校准（ESP32=ttyUSB1(CP210x)，雷达=ttyUSB0(HL-340)）
- docs: AGENTS.md 行为指南
- chore: gitignore YOLO 权重文件

### 2026-07-28 · 稳定性微调

- fix: 恢复堵转检测为 2s（2000ms），过坎不误判、真堵转会停
- fix: IMU 帧旋转与 EKF 对齐 + EMA 平滑 + 线程安全 + 脚本健壮性
- fix: 降低最大速度（200RPM / 0.89m/s）+ 放宽堵转检测 + board_test 调试输出

### 2026-07-22 · 激光跟随增强

- fix: 右电机 BO1/BO2 接线反（交换 IN1/IN2）
- feat: 激光跟随加人体宽度约束（0.15~0.55m）+ 连续锁定检测

### 2026-07-19 ~ 07-20 · 清理与部署文档

- fix: CMakeLists 移除已删除的雷达脚本引用
- docs: SETUP 添加 SSH 免密登录 + ROS 主从配置步骤
- docs: SETUP/README 移除所有微波雷达内容
- refactor: ESP32 固件加调参注释；移除微波雷达；修复仿真脚本
- refactor: ld2402_publisher 移除不可靠的距离发布，只保留 presence Bool

### 2026-07-15 ~ 07-18 · 奠基

- init: ROS 两轮差速机器人项目初始提交
- fix: 轮距统一为 180mm（135→180）+ ESP32 硬件看门狗
- clean: 删除所有死代码；serial_bridge.py 停止 CMake 安装
- fix: package.xml 添加 robot_localization 依赖
- feat: IMU 输出稳定化（零偏校准 + 低通滤波 + 姿态输出）
- feat: 添加 MPU6050 稳定性测试固件
- feat: complementary filter + roll normalization + IMU init pre-read
- feat: LD2402 毫米波雷达人体跟随 + 速度参数优化 + 编码器方向修正（后于 07-19 移除）
- feat: 激光+毫米波融合跟随 + 多项核心修复（后移除毫米波部分）
- feat: IMU 激光去畸变 + EKF 加速度轴修正 + IMU URDF 朝向修正