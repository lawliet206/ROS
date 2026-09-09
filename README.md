# ROS 两轮差速机器人 🤖

两轮差速 ROS 机器人项目，ROS Noetic。

## 📋 系统架构

```
PC (ROS Master, 10.222.149.11) ←WiFi→ J1900 (车载) ←USB→ ESP32 (rosserial)
                                          │              ├─ PWM → TB6612FNG → 左/右电机
                                          │              ├─ PCNT ← 编码器 (JGB37-520)
                                          │              └─ I2C  ← MPU6050 IMU
                                          └─ USB ← 激光雷达 (S9-FSRD-V1.0, 115200, AA55协议)
```

## 🔧 硬件参数

| 组件 | 型号 | 参数 |
|------|------|------|
| 电机 | JGB37-520 | 12V, 减速比 1:10, 11 PPR 霍尔编码器 |
| 驱动 | TB6612FNG | 双路 H 桥, 3.3V 逻辑直连 ESP32 |
| 下位机 | ESP32-WROOM-32 | PCNT 硬件编码器 + PID + MPU6050 IMU |
| J1900 | Intel Celeron J1900 | x86_64, Ubuntu 20.04, ROS Noetic Base |
| 轮子 | 85mm 橡胶轮 | 轮距 **180mm** |
| 电池 | 3S LiPo 11.1V 5200mAh | XT60 接口 |
| 激光雷达 | S9-FSRD-V1.0 RX | 115200, AA55, ~69Hz, 39点/帧 |

### 引脚接线

```
TB6612FNG ↔ ESP32:
  AIN1→GPIO25  AIN2→GPIO26  PWMA→GPIO18
  BIN1→GPIO32  BIN2→GPIO33  PWMB→GPIO19
  STBY→GPIO4(拉高)  VCC→3.3V  VM→电池12V

编码器:
  左A→GPIO27  左B→GPIO23  右A→GPIO14  右B→GPIO13

MPU6050: SDA→GPIO21  SCL→GPIO22
```

## 📁 项目结构

```
ROS/
├── src/
│   ├── robot_bringup/          # 实物包 (J1900 需要这个)
│   │   ├── launch/             # bringup/slam/navigation/ekf/follow
│   │   ├── scripts/            # s9_lidar_driver/laser_follower/send_goals
│   │   ├── urdf/robot.urdf    # 实物 URDF (轮距 0.180)
│   │   └── config/ekf.yaml
│   └── robot_sim/              # 仿真包 (仅 PC 用)
│       ├── launch/             # simulation/sim_slam/sim_navigation
│       ├── urdf/robot_sim.urdf # 仿真 URDF (含 Gazebo 插件)
│       ├── worlds/room.world   # 15m×15m 房间
│       └── scripts/            # sim_slam/sim_navigation/sim_follow
├── esp32_firmware/esp32_firmware.ino  # PCNT 版固件 (rosserial)
└── tools/                      # test_lidar.py / compile_ydlidar.sh
```

## 🚀 启动方式

### 仿真 (PC 单机)

```bash
# 建图 (慢速 0.2-0.3m/s 绕场)
bash ~/ROS/src/robot_sim/scripts/sim_slam.sh
rosrun map_server map_saver -f ~/maps/sim_map

# 导航 (需先建图)
bash ~/ROS/src/robot_sim/scripts/sim_navigation.sh ~/maps/sim_map.yaml
```

### 实物 (PC + J1900 + ESP32 + 激光雷达)

```bash
# J1900 终端1: ESP32 rosserial
rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB1 _baud:=115200

# J1900 终端2: 雷达
rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB0

# PC 终端: 建图或导航
roslaunch robot_bringup slam.launch start_lidar:=false
```

### 激光人体跟随

```bash
roslaunch robot_bringup follow.launch
```

### EKF 传感器融合（可选，提升定位精度）

```bash
# 安装（只需一次）
sudo apt install ros-noetic-robot-localization

# 启动 EKF 融合（替代纯编码器里程计）
roslaunch robot_bringup ekf.launch
```

## 🐛 当前状态 (2026-07-14)

### 已解决
- ✅ 轮距统一为 180mm（URDF/launch/固件/仿真全部对齐）
- ✅ ESP32 固件从软件中断升级为 PCNT 硬件编码器（不漏脉冲）
- ✅ 通信从 serial_bridge 改为 rosserial（省一个节点）
- ✅ 电机驱动从 BTS7960 → TB6612FNG（更便宜，3.3V 直连）
- ✅ 上位机从电视盒(Armbian) → J1900(x86_64)
- ✅ S9 雷达驱动完成（累积 360° 扫描，5Hz 发布）
- ✅ 导航参数已调优（TEB 规划器，max_vel_x=0.6）

### 未解决
- ❌ 导航路径有时绕远路（navfn 全局规划问题）
- ❌ 到达目标前速度不稳定（TEB 优化收敛问题）
- ❌ 仿真地图有轻微鬼影障碍物（gmapping 参数 minimumScore=300）
- ✅ 实物轮距已确认为 180mm

### 注意事项
  - ~~`serial_bridge.py`~~ 已被 rosserial 取代，已删除
  - ~~`wheel_controller.py`~~ 仿真用 Gazebo diff_drive 插件，已删除
- `s9_lidar_driver.py` 当前发布完整 360° 扫描，不是 1.4° 切片
- 仿真导航用 `sim_navigation.launch`，实物用 `navigation.launch`

### 后续待做
1. 装小车、接所有硬件
2. 烧录 ESP32 固件（PCNT 版）
3. J1900 部署 robot_bringup + rosserial + 雷达驱动
4. 实物建图测试
5. 考核：建图 → 多点导航 → 人体跟随

---

## 开发环境配置

本项目使用 **OpenCode** 作为 AI 辅助开发工具，安装了两个插件：
- **[oh-my-openagent](https://github.com/code-yeongyu/oh-my-openagent)** — 多 Agent 协作编排（Sisyphus, Hephaestus, Oracle 等）
- **[superpowers](https://github.com/obra/superpowers)** — 技能系统（Skills），提供开发流程规约

---

## MCP 服务器大全

MCP（Model Context Protocol）为 AI 提供外部工具调用能力。以下 MCP 均已配置：

### 1. context7 — 官方文档查询

| 属性 | 值 |
|------|-----|
| 类型 | 远程 |
| 协议 | `https://mcp.context7.com/mcp` |
| 用途 | 查询任意库/框架/工具的**官方文档**（ROS, Python, React, Next.js, Express 等） |
| 用法 | AI 自动触发，或提示中提及 `use context7` |

**原理**：先通过 `context7_resolve-library-id` 解析库名获取 ID，再用 `context7_query-docs` 查询具体 API 用法。比 Web 搜索更准确且不受训练数据时效限制。

### 2. gh_grep / grep_app — GitHub 开源代码搜索

| 属性 | 值 |
|------|-----|
| 类型 | 远程 |
| 协议 | `https://mcp.grep.app` |
| 用途 | 搜索 GitHub 上百万公开仓库的**真实代码片段** |
| 用法 | AI 自动触发，搜索实际代码模式（非关键字） |

**适用场景**：
- 不确定某个 API 怎么用 → 搜真实项目中的用法
- 需要生产级模式参考 → 搜成熟开源项目的实现
- 跨库集成模式 → 搜组合使用示例

### 3. websearch (Exa) — 联网搜索

| 属性 | 值 |
|------|-----|
| 类型 | 内置 |
| 用途 | 联网搜索任何主题，返回清洁文本内容 |
| 用法 | 通过 `websearch_web_search_exa` 工具调用 |

**适用场景**：查找最新信息、新闻、人物、公司、技术方案对比等。

### 4. ros_mcp — ROS 本地工具集

| 属性 | 值 |
|------|-----|
| 类型 | 本地（Python 实现） |
| 路径 | `~/.config/opencode/mcp_ros.py` |
| 用途 | 提供 ROS 开发全流程工具 |
| 启用 | 对 Sisyphus Agent 默认启用 |

**工具清单**：

| 工具名 | 功能 | 超时 |
|--------|------|------|
| `ros_build` | 执行 `catkin_make` 编译 | 180s |
| `ros_launch` | 启动 launch 文件 | 15s |
| `ros_list_nodes` | 查看运行中的 ROS 节点 | 30s |
| `ros_list_topics` | 查看活跃话题（含类型统计） | 30s |
| `ros_echo` | 订阅并返回一条话题消息 | 10s |
| `ros_service_call` | 调用 ROS 服务 | 15s |
| `ros_list_launch_files` | 递归列出所有 `.launch` 文件 | — |
| `ros_show_launch` | 查看 launch 文件内容 | — |

---

## Skills 完全手册

Skills 是带领域指令的封装模块，**在对话中命中关键词时自动触发**（使用前必须加载）。分为三大类：

---

### 一、流程规约类（Superpowers 提供）

这些 Skill 定义**做事的方式**——先加载 Skill，再执行任务。

#### 1. brainstorming — 创意设计

| 属性 | 值 |
|------|-----|
| 触发 | 任何创造性工作（建功能、改行为、加特性） |
| 阶段 | **必须**在设计/实现前触发 |

**工作流**：探索项目上下文 → 逐一问澄清问题 → 提出 2-3 种方案 → 分段呈现设计 → 用户批准 → 写入设计文档 → 自我审查 → 用户审查 → 生成实施计划。

**核心规则**：在用户批准设计之前，禁止写任何代码。不触发此 Skill 的"小功能"通常出最多问题。

#### 2. systematic-debugging — 系统化调试

| 属性 | 值 |
|------|-----|
| 触发 | 任何技术问题（bug、测试失败、构建失败、性能问题） |
| 铁律 | **没有根因分析之前禁止修复** |

**四阶段**：
1. **根因调查** — 读错误信息、稳定复现、检查近期变更、多组件系统加诊断埋点、追踪数据流
2. **模式分析** — 找工作示例、对比参考实现、识别差异
3. **假设与验证** — 单一假设、最小改动验证、一项变量一次
4. **实施** — 先写失败测试 → 单点修复 → 验证 → 3 次失败后质疑架构

#### 3. verification-before-completion — 完成前验证

| 属性 | 值 |
|------|-----|
| 触发 | 声称工作完成/修复/通过之前 |
| 铁律 | **无验证证据，无完成声明** |

**核心模式**：识别验证命令 → 完整运行 → 读取完整输出 → 确认声明成立 → 然后才做声明。禁止"应该没问题"、"看起来对了"等说法。

#### 4. test-driven-development — 测试驱动开发

| 属性 | 值 |
|------|-----|
| 触发 | 任何新功能、bug 修复、重构、行为变更 |
| 铁律 | **没有先行失败测试的生产代码** |

**红-绿-重构循环**：
1. RED — 写一个失败测试，验证它因正确原因失败
2. GREEN — 写最简代码使其通过
3. REFACTOR — 保持绿色前提下的清理

**禁止**：先写代码再补测试、批量测试不逐一验证、用 mock 代替真实行为。

#### 5. writing-plans — 编写实施计划

| 属性 | 值 |
|------|-----|
| 触发 | 有规格说明或需求后，在碰代码之前 |
| 输出 | `docs/superpowers/plans/YYYY-MM-DD-<功能名>.md` |

**规范**：每个步骤是可独立测试的原子操作（2-5 分钟），包含精确文件路径、完整代码、预期输出。禁止"TBD"、"添加错误处理"等占位符。

#### 6. executing-plans — 执行计划（当前会话）

| 属性 | 值 |
|------|-----|
| 触发 | 已有实施计划，在当前会话中执行 |

**流程**：加载计划 → 关键审查 → 按步骤执行（每步验证） → 完成后调用 `finishing-a-development-branch`。

#### 7. subagent-driven-development — 子代理驱动开发（推荐）

| 属性 | 值 |
|------|-----|
| 触发 | 已有实施计划，任务相对独立 |
| 优势 | 每个任务派新子代理，上下文不污染，任务级审查 |

**流程**：每个任务分配独立子代理 → 子代理实施、测试、自审 → 生成 diff 文件 → 任务审查员审查 → 修复关键问题 → 最终全局审查 → `finishing-a-development-branch`。

#### 8. finishing-a-development-branch — 完成开发分支

| 属性 | 值 |
|------|-----|
| 触发 | 实施完成、测试通过后 |
| 输出 | 四种选项供用户选择 |

**选项**：1. 合并到主分支 2. 推送并创建 PR 3. 保持分支现状 4. 丢弃本次工作。

#### 9. requesting-code-review — 请求代码审查

| 属性 | 值 |
|------|-----|
| 触发 | 每个任务完成后、合并前 |

派发专门审查子代理，包含 BASE/HEAD SHA、功能描述、需求规格。

#### 10. receiving-code-review — 接收代码审查反馈

| 属性 | 值 |
|------|-----|
| 触发 | 收到代码审查反馈时 |
| 规则 | 先理解再实施、先验证再行动、先推敲再同意 |

**禁止**：表演性同意（"说得对！"）、盲目实施、批量化不改测试。

#### 11. using-git-worktrees — Git 工作树隔离

| 属性 | 值 |
|------|-----|
| 触发 | 开始需要隔离工作区的功能开发 |

确保实施在主工作树之外进行，避免污染。

---

### 二、领域实施类（oh-my-openagent 提供）

这些 Skill 定义**做什么**——针对特定领域提供深度指导和规范。

#### 12. programming — 通用编程

| 属性 | 值 |
|------|-----|
| 触发 | 任何 `.py` `.rs` `.ts` `.tsx` `.go` 文件 |
| 加载 | 根据语言再加载对应的 `references/<language>/README.md` |

**编程哲学**：
- **类型即证明** — 让非法状态不可表达
- **边界解析** — 非信任输入在边界解析为类型化值，内部不复检
- **每个概念一个名字** — `UserId` 不是 `string`
- **穷尽匹配** — 永远用 `match`/`switch` + `assert_never`
- **TDD** — 红→绿→重构，不可协商

**支持语言**：Python（Pydantic v2、FastAPI、SQLAlchemy）、Rust（serde、axum、tokio）、TypeScript（Zod、Hono、Biome）、Go（gin、sqlc、pgx、slog）。

**代码异味检测**（自动审查触发器）：
- 文件超过 250 纯 LOC → **缺陷**，必须拆分
- 函数超过 3 个参数 → 用结构体封装
- 破坏操作后冗余验证 → 信任合约
- 否定形式命名 → 改肯定形式
- 日志分级 → 按消费者分级，非按严重程度

#### 13. debugging — 实战调试

| 属性 | 值 |
|------|-----|
| 触发 | 任何运行时调试（崩溃、静默失败、内存泄漏、async 异常） |
| 铁律 | **运行时真相胜过代码阅读** |

**十阶段循环**：环境评估 → 日志初始化 → 假设构建（最少 3 个正交假设） → 并行调查 → Oracle 三重奏（2 轮失败后） → 用户升级 → 根因确认 → TDD 修复 → 手动 QA → 清理 → 最终验证。

**运行时支持**：Python（pdb/ipdb/debugpy）、Node/tsx（注意 source-map 坑）、Rust（tokio-console、miri）、Go（delve、pprof、race）、原生二进制（Ghidra、pwndbg、pwntools）。

#### 14. frontend — 前端 UI/UX

| 属性 | 值 |
|------|-----|
| 触发 | 任何前端/UI/UX/样式/设计工作 |
| 标准 | Linear、Stripe、Supabase 级的设计质量 |

**设计系统门禁**：没有 `DESIGN.md` 禁止写组件。每个颜色、字号、间距必须可追溯到 token。

**四个规则集**：
- `design/` — 12 个风格技能 + 70 个品牌设计系统
- `perfection/` — Lighthouse 100 性能审计
- `ui-ux-db/` — 调色板/字体/布局参考数据库
- `designpowers/` — 人物画像/可访问性/设计评审

#### 15. refactor — 智能重构

| 属性 | 值 |
|------|-----|
| 触发 | `refactor`、`重构`、`清理` |
| 用法 | `/refactor <目标> --scope=<范围> --strategy=<策略>` |

**六阶段**：意图门禁 → 并行探索代码库 → 构建依赖关系图 → 测试覆盖率评估 → 计划生成 → 逐步执行（每步 LSP 验证 + 测试）。

**安全保证**：用 `lsp_rename` 做符号重命名、`ast-grep` 预览结构变换、每步验证通过才继续。

#### 16. remove-ai-slops — 清除 AI 代码异味

| 属性 | 值 |
|------|-----|
| 触发 | `remove ai slops`、`清理 AI 代码` |
| 安全 | **先用回归测试锁定行为了再删除** |

**十大分类**：
1. 冗余注释（不说 WHY 的注释、注释掉的代码、模糊 TODO）
2. 过度防御（在类型安全区域做 null 检查、空的 `catch {}`）
3. 过度复杂（>3 层嵌套、>5 参数、>50 行函数、`if/elif` 代替 `match`）
4. 无谓抽象（透传包装、单次使用帮助函数、为"以后"加的间接层）
5. 边界违反（UI 层导入 DB 驱动、Handler 做业务逻辑）
6. 死代码（未用导入、私有函数、不可达分支、`console.log` 残留）
7. 重复（复制粘贴、魔数重复出现）
8. 性能等价（O(n²)→O(n)、循环外提、list→generator、join 代替拼接）
9. 缺失测试（行为无回归测试锁定）
10. 模块过大（>250 纯 LOC，必须拆分）

**流程**：锁定行为（写回归测试） → 删除梯子（删整块 → 复用 → 平台原生 → 简化） → 并行分批清理（每批 5 个 deep agent） → 质量门禁验证。

#### 17. git-master — Git 专家

| 属性 | 值 |
|------|-----|
| 触发 | 任何 git 操作（commit、rebase、历史搜索） |
| 所有命令必须前缀 `GIT_MASTER=1` |

**三大模式**：
- **Commit** — 默认多提交（3+ 文件必须 ≥2 提交）、检测仓库提交风格（semantic/plain/short）、原子化拆分
- **Rebase** — 安全检测、交互式变基、autosquash、冲突处理、恢复流程
- **History Search** — `git log -S`（pickaxe）、`git log -G`（正则）、`git blame`、`git bisect`

#### 18. ast-grep — AST 感知的代码搜索与重写

| 属性 | 值 |
|------|-----|
| 触发 | 按代码结构（非文本）搜索/重写 |
| 支持 | 25 种语言，通过 `sg`（ast-grep）命令行 |

**使用决策树**：
- 结构模式（函数形状、调用、类、控制流） → **ast-grep**
- 文本模式（正则、字符类、文件名） → **rg/grep**
- 语义问题（变量引用、是否抛异常） → **LSP 工具**

**助手脚本**：`scripts/ast_grep_helper.py`，提供 `search`、`replace`、`scan`、`validate` 子命令。**禁止直接 apply 不改 preview。**

#### 19. start-work — 启动工作

| 属性 | 值 |
|------|-----|
| 触发 | 用户说 `$start-work`、`/start-work`、`开始工作` |
| 角色 | **编排器（永不实施）** |

**流程**：选择计划 → 创建或更新 Boulder 状态 → 执行下一个复选框（按难度分配 agent） → 验证并记录证据（5 道门禁） → 标记进度 → 直至完成。

**Boulder 证据系统**：每步产出记录到 `.omo/start-work/ledger.jsonl`，包含事件、任务、命令、工件、对抗性测试类、清理凭证。

#### 20. ulw-plan — 规划顾问（Prometheus）

| 属性 | 值 |
|------|-----|
| 触发 | 模糊/大型需求、`ulw-plan`、`plan this` |
| 角色 | **只规划，不实施** |

**意图路由**：
- CLEAR — 用户知道要什么，只问仓库回答不了的偏好
- UNCLEAR — 目标模糊，研究最大化，采用最佳实践默认值，不问用户

**输出**：决策完整的实施计划至 `.omo/plans/<slug>.md`。

#### 21. review-work — 5 Agent 并行代码审查

| 属性 | 值 |
|------|-----|
| 触发 | 实施完成后、PR 前 |
| 并行 | 5 个 agent 同时运行 |

**五个审查维度**：
| # | 审查员 | Agent 类型 | 审查内容 |
|---|--------|-----------|---------|
| 1 | 目标验证员 | Oracle | 是否构建了要求的内容？ |
| 2 | QA 执行员 | unspecified-high | 实际运行是否正常工作？ |
| 3 | 代码审查员 | Oracle | 代码质量是否良好？ |
| 4 | 安全审计员 | Oracle | 是否存在安全漏洞？ |
| 5 | 上下文挖掘员 | unspecified-high | 是否遗漏了任何上下文？ |

**判定**：全部 PASS → 通过；任一 FAIL → 失败。

#### 22. init-deep — 生成 AGENTS.md 知识库

| 属性 | 值 |
|------|-----|
| 触发 | `/init-deep` |
| 输出 | 层级化 `AGENTS.md` 文件（根 + 子模块） |

**流程**：并行探索发现 → LSP/codegraph 代码映射 → 目录评分决策 → 并行生成 AGENTS.md → 去重审查。

---

### 三、通用工具类

#### 23. writing-skills — 编写 / 编辑 Skill

| 属性 | 值 |
|------|-----|
| 触发 | 创建或编辑技能文件 |
| 用途 | 编写新的 Skill 定义文件 |

#### 24. security-research / security-review — 安全研究

| 属性 | 值 |
|------|-----|
| 触发 | `security-research`、`security review`、`/security-review` |
| 工作流 | 3 个漏洞猎人 + 2 个 PoC 工程师并行审计代码库 |

#### 25. ultimate-browsing — 终极浏览器

| 属性 | 值 |
|------|-----|
| 触发 | 被 WAF/Cloudflare 屏蔽、需要 JS 渲染、中文平台抓取 |
| 三层分级 | T1 指纹浏览器 + T1.5 平台原生读取器 + T2 Chrome 隐身 |

**支持**：小红书、抖音、微博、B 站、V2EX、微信公众号、Twitter、Reddit、LinkedIn、GitHub 等。

#### 26. ulw-research / ultraresearch — 深度研究

| 属性 | 值 |
|------|-----|
| 触发 | `ulw-research`、`$ultraresearch` |
| 能力 | 最大饱和度研究：代码库 + Web + 官方文档 + OSS 代码并行 |

#### 27. visual-qa — 可视化 QA

| 属性 | 值 |
|------|-----|
| 触发 | 构建/修改 UI 后、被问到页面是否正常 |
| 流程 | 截图 → 设计系统审查 → 功能审查 → CJK 审查 → 像素对比 |

#### 28. coding-agent-sessions — 会话历史管理

| 属性 | 值 |
|------|-----|
| 触发 | 查找、读取、搜索代理会话历史 |
| 支持 | Codex、Claude Code/Desktop、OpenCode、Senpi、Aider 等 |

**可用工具**：
- `session_list` — 列出所有会话
- `session_read` — 读取会话消息历史
- `session_search` — 全文搜索会话内容
- `session_info` — 查看会话元数据

#### 29. lsp-setup — LSP 语言服务器配置

| 属性 | 值 |
|------|-----|
| 触发 | 配置 LSP、安装语言服务器、修复 "no LSP server configured" |
| 支持 | TypeScript、Python、Go、Rust、C/C++、Java 等 |

#### 30. lcx-doctor — Codex CLI/LazyCodex 健康检查

| 属性 | 值 |
|------|-----|
| 触发 | Codex CLI 安装异常、更新后行为异常 |

#### 31. lcx-report-bug — 报告 Codex 相关 Bug

| 属性 | 值 |
|------|-----|
| 触发 | 报告、提交 LazyCodex/Codex CLI bug |
| 输出 | 带根因分析和复现步骤的 GitHub Issue |

#### 32. lcx-contribute-bug-fix — 提交 Codex 相关 Bug 修复

| 属性 | 值 |
|------|-----|
| 触发 | 修复 LazyCodex/Codex CLI bug 并提交 PR |

#### 33. customize-opencode — 配置 OpenCode 本身

| 属性 | 值 |
|------|-----|
| 触发 | 编辑 `opencode.json`、`~/.config/opencode/`、创建 agent/skill/plugin |

---

## Slash 命令速查

| 命令 | 来源 | 作用 |
|------|------|------|
| `/start-work` | oh-my-openagent | 按计划启动工作，角色为编排器 |
| `/init-deep` | oh-my-openagent | 生成层级化 AGENTS.md 知识库 |
| `/ulw-loop` | oh-my-openagent | **自循环**，不达 100% 不停止 |
| `/review-work` | oh-my-openagent | 5 Agent 并行代码审查 |
| `/refactor` | oh-my-openagent | 智能重构（指定目标/范围/策略） |
| `/writing-plans` | superpowers | 使用 writing-plans skill |
| `/brainstorming` | superpowers | 使用 brainstorming skill |
| `/subagent-driven-development` | superpowers | 使用子代理驱动开发 |
| `/executing-plans` | superpowers | 使用执行计划 skill |
| `/using-git-worktrees` | superpowers | 使用工作树隔离 skill |
| `/test-driven-development` | superpowers | 使用 TDD skill |
| `/systematic-debugging` | superpowers | 使用系统化调试 skill |
| `/verification-before-completion` | superpowers | 使用完成前验证 skill |
| `/requesting-code-review` | superpowers | 请求代码审查 |
| `/receiving-code-review` | superpowers | 接收审查反馈 |
| `/finishing-a-development-branch` | superpowers | 完成开发分支 |
| `/writing-skills` | superpowers | 编写技能文件 |
| `/brainstorming` | superpowers | 创意设计流程 |
| `/security-research` / `/security-review` | oh-my-openagent | 安全研究审计 |
| `/playwright` | OpenCode | 浏览器自动化 |
| `/stop-continuation` | OpenCode | 停止所有持续机制 |
| `/handoff` | OpenCode | 生成上下文摘要以在新会话继续 |
| `/ralph-loop` | OpenCode | 启动自引用开发循环 |
| `/cancel-ralph` | OpenCode | 取消 Ralph Loop |
| `/hyperplan` | OpenCode | 对抗性多 Agent 规划 |
| `ultrawork` / `ulw` | oh-my-openagent | **一键激活**所有 Agent 并行工作 |

---

## Git 版本控制

本项目使用 Git 进行版本控制，初始提交 `21969ee` 包含 46 个文件（6760 行）。

### 常用命令速查

| 命令 | 作用 | 用法示例 |
|------|------|---------|
| `git log` | 查看提交历史 | `git log --oneline -10`（最近 10 条） |
| `git log --oneline --graph` | 图形化查看分支历史 | `git log --oneline --graph --all` |
| `git status` | 查看当前工作区状态（改了哪些文件） | `git status` |
| `git diff` | 查看工作区与上次提交的差异 | `git diff` |
| `git diff --stat` | 只显示改了哪些文件，不显示具体内容 | `git diff --stat HEAD~1` |
| `git show` | 查看某次提交的详情 | `git show 21969ee` |
| `git blame <文件>` | 查看文件每行是谁最后改的、什么时候 | `git blame src/.../laser_follower.py` |
| `git add <文件>` | 暂存文件（准备提交） | `git add src/.../serial_bridge.py` |
| `git add .` | 暂存所有变更 | `git add .` |
| `git commit -m "消息"` | 提交暂存的文件 | `git commit -m "fix: 统一轮距为 0.180m"` |
| `git commit -am "消息"` | 暂存所有已跟踪文件 + 提交（一步到位） | `git commit -am "refactor: 删除 wheel_controller.py"` |
| `git checkout -- <文件>` | **丢弃某个文件的未提交改动**（后悔药） | `git checkout -- esp32_firmware/esp32_firmware.ino` |
| `git checkout .` | **丢弃所有未提交改动** | `git checkout .` |
| `git revert <提交ID>` | **撤销某次提交**（安全，不会丢历史） | `git revert 21969ee` |
| `git branch` | 列出本地分支 | `git branch -a`（含远程） |
| `git checkout -b <分支名>` | 创建并切换到新分支 | `git checkout -b fix-wheel-base` |
| `git checkout <分支名>` | 切换分支 | `git checkout master` |
| `git merge <分支名>` | 合并分支到当前分支 | `git merge fix-wheel-base` |
| `git stash` | 临时保存当前工作（切分支前用） | `git stash && git checkout master` |
| `git stash pop` | 恢复临时保存的工作 | `git stash pop` |
| `git tag <标签名>` | 打标签（里程碑） | `git tag v1.0-slam-works` |
| `git clean -fd` | 删除未跟踪的文件和目录 | `git clean -fd`（⚠️ 慎用，会删 .gitignore 外的文件） |

### 实际场景速查

**场景 1：改坏了想还原**
```bash
# 还没提交：
git checkout .                    # 所有文件还原到上次提交状态
git checkout -- src/.../file.py   # 只还原某个文件

# 已经提交了：
git revert HEAD                   # 撤销最新一次提交（安全）
git revert <commit-id>            # 撤销某次特定提交
```

**场景 2：想试试一个改动，又怕影响主线**
```bash
git checkout -b test-new-pid      # 开个新分支
# ... 改 PID 参数、跑一圈 ...
git commit -am "test: 尝试新的 PID 参数"
git checkout master                # 回到主线
git branch -D test-new-pid        # 不满意，删掉分支
```

**场景 3：查一段代码是谁写的**
```bash
git blame src/robot_bringup/scripts/s9_lidar_driver.py
# 输出：每行前面有 commit ID + 作者 + 日期
```

**场景 4：比较当前和一周前的区别**
```bash
git log --oneline --since="7 days ago"   # 看一周内的改动
git diff HEAD~5                           # 跟 5 次提交前比
```

### .gitignore 内容

```
build/
devel/
install/
*.pyc / __pycache__/
.vscode/ / .idea/
.claude/ / .claud/ / .omo/ / .waylog/
YDLIDAR/   # 第三方 vendored SDK，自带 git
```

---

## 配置文件位置

| 文件 | 作用 |
|------|------|
| `~/.config/opencode/opencode.jsonc` | OpenCode 主配置（插件 + MCP 注册） |
| `~/.config/opencode/oh-my-openagent.json` | oh-my-openagent Agent 与模型映射 |
| `~/.config/opencode/mcp_ros.py` | ROS MCP 服务器实现 |
| `~/.local/share/opencode/auth.json` | 模型提供商凭据（OpenCode Go + DeepSeek） |
| `~/.claude/CLAUDE.md` | 全局 AI 行为指令（本项目用中文回复） |
