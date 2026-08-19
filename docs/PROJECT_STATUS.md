# 项目状态

> 最后更新: 2026-08-20（与 `main` 分支同步）

## 1. 组件状态矩阵

| 组件 | 状态 | 验证方式 | 备注 |
|------|------|----------|------|
| S9 雷达驱动 (`s9_lidar_driver.py`) | ✅ 稳定 | 实机 + 23 项回归测试 | AA55 协议逆向；cs 校验字段仍为黑盒（见 ARCHITECTURE.md §7） |
| IMU 激光去畸变 (`scan_deskew.py`) | ✅ 稳定 | 实机 + 11 项回归测试 | 转弯场景防地图重影 |
| EKF 里程计/IMU 融合 | ✅ 稳定 | 实机 | sensor_timeout 0.35 防 IMU 断流跳变 |
| SLAM 建图 (gmapping) | ✅ 实机验证 | 实物建图（论文 fig6-1） | 订阅 `/scan_deskewed` |
| 自主导航 (AMCL + move_base + TEB) | ✅ 实机验证 | 实物多点导航（论文 fig6-3） | 已知调参问题见 issue #1 #2 |
| 多点巡航 (`send_goals.py`) | ✅ 实机验证 | 实机巡航 + 6 项回归测试 | YAML/参数双加载 |
| 人体跟随（视觉+雷达） | ✅ 实机验证 | 实机测试 | 三态状态机 + 12 项测试；雷达断流保护 |
| 纯雷达跟随 (`laser_follower.py`) | ✅ 实机验证 | 实机测试 | 宽度约束 0.15~0.55m |
| ESP32 固件 | ✅ 稳定 | `esp32_board_test` 板级验证 | 双路 PID + 看门狗 + 堵转保护 + 零速关 STBY |
| Gazebo 仿真 | ⚠️ 可用 | 仿真运行 | 已知"幽灵障碍"问题见 issue #3 |
| 一键流程 (robot_start / j1900_start) | ✅ 稳定 | 实机流程 | stop 零速保障、SSH 联动、端口动态检测 |

## 2. 测试覆盖

```
tests/                         61 例 (pytest, 无硬件依赖, CI ros:noetic 容器运行)
├── test_s9_lidar_driver.py    23 例  帧提取(11) + 距离标定(1) + 角度换算(3) + 缓冲/跨零(8)
├── test_vision_follower.py    12 例  三态状态机 + 角度换算/选人
├── test_person_detector.py     9 例  检测帧处理 + 角度换算
├── test_scan_deskew.py        11 例  去畸变算法(7) + IMU 角速度窗均值(4)
└── test_send_goals.py          6 例  巡航目标解析(含畸形条目容错)
```

CI（`.github/workflows/ci.yml`）：
- **静态检查 job**（ubuntu，无需 ROS）：Python compileall、全部 `.sh` 的 `bash -n`、launch/URDF/world/package.xml 的 XML 校验、config YAML 校验、package.xml 元数据不变量（无占位邮箱、有版本号）。
- **ROS 容器 job**（`ros:noetic-robot`）：安装全部依赖 → `catkin_make` → `pytest tests -q`。

## 3. 已知限制（对应 open issues）

| # | 问题 | 状态 | 说明 |
|---|------|------|------|
| [#1](https://github.com/lawliet206/ROS/issues/1) | 导航偶发选路不优 | 待调参 | navfn 全局规划 + TEB 局部；与膨胀半径/代价地图参数相关 |
| [#2](https://github.com/lawliet206/ROS/issues/2) | TEB 速度收敛 | 待调参 | 与加速度限制/dt_ref/优化权重相关 |
| [#3](https://github.com/lawliet206/ROS/issues/3) | Gazebo 幽灵障碍 | 待排查 | 已排除雷达自碰（range_min 过滤），疑与 TF/定位相关 |
| [#4](https://github.com/lawliet206/ROS/issues/4) | 实物轮子验证 | 等待硬件 | 固件/参数已就绪，需真车复测 |

> 本项目承诺：**不伪造验证结果**。上表"✅"均有真实提交记录佐证；"待*"条目是真实存在的开放问题，欢迎贡献。

## 4. 版本与发布

- 当前版本: `0.1.0`（预发布，尚无 release tag）
- 版本策略: 遵循 [SemVer](https://semver.org/lang/zh-CN/)。0.x 阶段 API 可不兼容变更；
  首个 1.0.0 在"实物验证全部完成 + 文档闭环"后发布。
- 变更记录: [CHANGELOG.md](../CHANGELOG.md)

## 5. 路线图

- **短期（0.1.x）**：issue #4 实物轮子验证闭环；#2 TEB 收敛调参；#3 仿真幽灵障碍排查。
- **中期（0.2.x）**：仿真世界多样化（走廊/动态障碍）；导航参数自动调优脚本；rosbag 复现教程。
- **长期（1.0）**：完整实物验收流程；多场景回归基准；独立 ROS 包发布（bloom）。