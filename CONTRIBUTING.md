# 贡献指南 · Contributing

感谢你对本项目的兴趣！在提交 Issue / PR 之前，请先阅读本文档与本仓库根目录的 [AGENTS.md](AGENTS.md)。

For English contributors: a short English summary is at the bottom of this file.
维护者使用 AI 工具链（Codex 等）的规范见 [docs/MAINTAINER_WORKFLOW.md](docs/MAINTAINER_WORKFLOW.md)。

## 项目定位

这是一个**真实运行的两轮差速自主移动机器人平台**（ROS Noetic + ESP32 + J1900 + LiDAR），
涉及真实硬件（电机、编码器、IMU、雷达、电池）。任何改动都可能影响实体机器人的安全，
请把"不破坏现有功能、不引入安全风险"作为第一原则。

## 环境要求

| 组件 | 版本 |
|------|------|
| OS | Ubuntu 20.04 (Focal) |
| ROS | Noetic（PC 与 J1900 均安装） |
| Python | 3.8（ROS 依赖） |
| 编译 | `catkin_make`（不要用 catkin tools 的混合布局） |

```bash
source /opt/ros/noetic/setup.bash
cd <工作空间根>
catkin_make
source devel/setup.bash
```

## 测试

```bash
# 单元测试（纯逻辑回归，无需硬件；CI 用 ros:noetic 容器自动执行）
python3 -m pip install -r requirements-dev.txt
python3 -m pytest tests -q
```

- 测试直接 import `src/*/scripts` 下的节点（通过 `sys.path.insert`），**不要删除 tests/ 里的该行**。
- `tests/conftest.py` 提供无 ROS 环境下的 rospy/消息类型替身，使纯逻辑测试可在任意机器运行。
- 修改 ROS Python 节点 → **必须**同步新增/更新对应测试并跑通。
- 修改 ESP32 固件 → 必须用 `esp32_board_test` 在硬件上验证。
- 涉及底盘控制（PID、PWM 映射、方向、死区）→ 先架空轮子验证，再低速（≤0.3 m/s）落地测试。

## 硬件安全红线（不可削弱）

- 启动/重启后小车必须保持静止，禁止无条件高速运动。
- `stop` 必须发送零速度（`robot_start.sh stop` / `j1900_start.sh stop`）。
- 不要修改 EKF 的 `odom_frame/base_frame` 命名约定（odom→base_footprint），改坏会导致 AMCL 定位跳变。
- 不要随意修改 `esp32_firmware/libraries/ros_lib/`（vendored 官方库+本地修复）。
- 不要修改 URDF 中的轮距/关节坐标系（与固件、launch 的 180mm 轮距强耦合）。

## 提交 Issue

### Bug 报告

请使用 [Bug 模板](.github/ISSUE_TEMPLATE/bug_report.md)，并尽量包含：

- 运行环境：实物机器人 / Gazebo 仿真；PC / J1900 / ESP32
- 完整命令与启动方式（如 `bash src/robot_bringup/scripts/robot_start.sh slam`）
- 日志/截图/话题数据（`rostopic echo`）
- 是否首次出现、是否可复现

### 功能建议

请使用 [Feature 模板](.github/ISSUE_TEMPLATE/feature_request.md)，说明动机、期望行为与可接受的验收方式。

## 提交 PR

1. 从 `main` 创建分支，命名建议：`fix/<简述>` / `feat/<简述>` / `docs/<简述>`。
2. 变更尽量小而聚焦；不相关的重构请拆成独立 PR。
3. 运行测试并确保通过（至少 `python3 -m pytest tests -q` 与 CI 静态检查）。
4. 在 PR 描述中说明：改了什么、为什么、如何验证（本机/仿真/实机）。
5. 等待 CI 通过后由维护者 review 合并。

## 代码风格

- 保持现有风格：中文注释、4 空格缩进、函数/变量命名 snake_case。
- 不要引入新依赖；优先复用现有模式。
- 新增纯逻辑（可无 ROS 环境测试的部分）尽量提取为模块级纯函数，便于单测。

## 文档

- 修改 launch 参数 / 脚本行为后，同步更新 README.md、SETUP.md、AGENTS.md 中对应的描述。
- 新增可复现的坑与解法，优先写入 SETUP.md 的 FAQ 而非仅留在 Issue 评论里。

---

## English summary

This project targets ROS Noetic on Ubuntu 20.04 and includes code that can command a physical mobile robot. Read [SETUP.md](SETUP.md) before running hardware-related scripts.

Suggested workflow:

1. Open an issue or describe the proposed change before starting work that affects hardware behavior, launch defaults, or navigation parameters.
2. Keep each pull request focused on one behavior or documentation topic.
3. Add or update a test when changing pure Python logic.
4. Run the checks below and include their results in the pull request.
5. Do not commit ROS logs, model weights, credentials, maps containing sensitive locations, or machine-specific network addresses.

Validation:

```bash
python3 -m pip install -r requirements-dev.txt
python3 -m pytest tests -q
```

For ROS changes, also build the workspace with `catkin_make` after sourcing `/opt/ros/noetic/setup.bash`.

Hardware safety: never weaken the stop workflow (`robot_start.sh stop` publishes zero velocity); lift the robot or stay below 0.3 m/s for first motor tests; keep ROS Master bound to the local network only.