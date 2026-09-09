# 开发环境指南（开发者参考）

本文档记录本仓库的 AI 辅助开发工具链配置，属于**开发者个人环境参考**，与机器人源码无关，仅供维护者使用。

- 项目首页（用户视角）见 [../README.md](../README.md)
- 完整部署手册见 [../SETUP.md](../SETUP.md)

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

### 3. websearch (Exa) — 联网搜索

| 属性 | 值 |
|------|-----|
| 类型 | 内置 |
| 用途 | 联网搜索任何主题，返回清洁文本内容 |
| 用法 | 通过 `websearch_web_search_exa` 工具调用 |

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

### 一、流程规约类（Superpowers 提供）

| Skill | 触发时机 | 作用 |
|-------|----------|------|
| `brainstorming` | 任何创造性工作（建功能、改行为、加特性） | 探索需求 → 澄清问题 → 提方案 → 用户批准 → 写设计文档 → 再实现 |
| `systematic-debugging` | 任何技术问题（bug、测试失败、构建失败） | 根因分析 → 模式分析 → 假设验证 → 单点修复（先写失败测试） |
| `verification-before-completion` | 声称工作完成/修复/通过之前 | 无验证证据，无完成声明 |
| `test-driven-development` | 任何新功能、bug 修复、重构 | 红-绿-重构循环，先写失败测试 |
| `writing-plans` | 有规格说明后、碰代码之前 | 输出 `docs/superpowers/plans/YYYY-MM-DD-<功能名>.md` |
| `executing-plans` | 已有实施计划，当前会话执行 | 加载计划 → 审查 → 逐步执行验证 |
| `subagent-driven-development` | 已有实施计划，任务相对独立 | 每个任务派独立子代理实施、测试、自审 |
| `finishing-a-development-branch` | 实施完成、测试通过后 | 提供合并/推送 PR/保持/丢弃四种选项 |
| `requesting-code-review` | 每个任务完成后、合并前 | 派发专门审查子代理（含 BASE/HEAD SHA） |
| `receiving-code-review` | 收到代码审查反馈时 | 先理解再实施、先验证再行动 |
| `using-git-worktrees` | 需要隔离工作区的功能开发 | 避免污染主工作区 |

### 二、领域实施类（oh-my-openagent 提供）

| Skill | 触发时机 | 作用 |
|-------|----------|------|
| `programming` | 任何 `.py` `.rs` `.ts` `.tsx` `.go` 文件 | 类型即证明、边界解析、TDD、穷尽匹配、250 LOC 上限 |
| `debugging` | 任何运行时调试 | 运行时真相胜过代码阅读，十阶段循环 + Oracle 三重奏 |
| `frontend` | 任何前端/UI/UX/样式/设计工作 | Linear/Stripe 级设计质量，设计系统门禁 |
| `refactor` | `refactor`、重构、清理 | 意图门禁 → 并行探索 → 依赖图 → 计划 → 逐步执行 |
| `remove-ai-slops` | 清理 AI 代码异味 | 先回归测试锁行为 → 再删梯子 → 并行分批清理 |
| `git-master` | 任何 git 操作 | 原子提交、rebase/squash、历史搜索（blame/bisect/log -S） |
| `ast-grep` | 按代码结构（非文本）搜索/重写 | 结构模式用 sg，文本模式用 rg，语义问题用 LSP |
| `start-work` | 用户说 `$start-work` / `开始工作` | 编排器角色，按计划执行 + Boulder 证据系统 |
| `ulw-plan` | 模糊/大型需求 | 只规划不实施，输出决策完整的实施计划 |
| `review-work` | 实施完成后、PR 前 | 5 个 Agent 并行审查（目标/QA/代码/安全/上下文） |
| `init-deep` | `/init-deep` | 生成层级化 AGENTS.md 知识库 |
| `writing-skills` | 创建或编辑技能文件 | 编写新的 Skill 定义文件 |
| `security-research` / `security-review` | 安全研究、漏洞审计 | 3 个漏洞猎人 + 2 个 PoC 工程师并行审计 |
| `ultimate-browsing` | 被 WAF/Cloudflare 屏蔽、需要 JS 渲染 | 三层分级：T1 指纹浏览器 + T1.5 平台原生 + T2 Chrome 隐身 |
| `ulw-research` / `ultraresearch` | 深度研究 | 最大饱和度研究：代码库 + Web + 官方文档 + OSS 并行 |
| `visual-qa` | 构建/修改 UI 后 | 截图 → 设计系统审查 → 功能审查 → CJK 审查 → 像素对比 |
| `coding-agent-sessions` | 查找/读取会话历史 | 支持 Codex、Claude Code、OpenCode 等 |
| `lsp-setup` | 配置 LSP 语言服务器 | 路由到各语言 README（TS/Python/Go/Rust 等） |

### 三、通用工具类

- `lcx-doctor` — Codex CLI/LazyCodex 健康检查
- `lcx-report-bug` — 报告 Codex 相关 Bug
- `lcx-contribute-bug-fix` — 修复 Codex 相关 Bug 并提交 PR
- `customize-opencode` — 配置 OpenCode 本身（agent/skill/plugin/MCP/权限）

---

## Slash 命令速查

| 命令 | 来源 | 作用 |
|------|------|------|
| `/start-work` | oh-my-openagent | 按计划启动工作，角色为编排器 |
| `/init-deep` | oh-my-openagent | 生成层级化 AGENTS.md 知识库 |
| `/ulw-loop` | oh-my-openagent | 自循环，不达 100% 不停止 |
| `/review-work` | oh-my-openagent | 5 Agent 并行代码审查 |
| `/refactor` | oh-my-openagent | 智能重构 |
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
| `/security-research` / `/security-review` | oh-my-openagent | 安全研究审计 |
| `/playwright` | OpenCode | 浏览器自动化 |
| `/stop-continuation` | OpenCode | 停止所有持续机制 |
| `/handoff` | OpenCode | 生成上下文摘要以在新会话继续 |
| `/ralph-loop` | OpenCode | 启动自引用开发循环 |
| `/cancel-ralph` | OpenCode | 取消 Ralph Loop |
| `/hyperplan` | OpenCode | 对抗性多 Agent 规划 |
| `ultrawork` / `ulw` | oh-my-openagent | 一键激活所有 Agent 并行工作 |

---

## Git 版本控制

### 常用命令速查

| 命令 | 作用 |
|------|------|
| `git log --oneline -10` | 查看最近 10 条提交 |
| `git log --oneline --graph --all` | 图形化查看分支历史 |
| `git status` | 查看当前工作区状态 |
| `git diff` | 查看工作区与上次提交的差异 |
| `git show <commit>` | 查看某次提交的详情 |
| `git blame <文件>` | 查看文件每行是谁最后改的、什么时候 |
| `git add <文件>` | 暂存文件（准备提交） |
| `git commit -m "消息"` | 提交暂存的文件 |
| `git checkout -- <文件>` | 丢弃某个文件的未提交改动 |
| `git checkout .` | 丢弃所有未提交改动 |
| `git revert <提交ID>` | 撤销某次提交（安全，不丢历史） |
| `git branch` / `git checkout -b <分支名>` | 分支管理 |
| `git stash` / `git stash pop` | 临时保存/恢复工作 |
| `git tag <标签名>` | 打标签（里程碑） |
| `git clean -fd` | 删除未跟踪的文件和目录（⚠️ 慎用） |

### 实际场景速查

**改坏了想还原：**
```bash
# 还没提交：
git checkout .                    # 所有文件还原到上次提交状态
git checkout -- src/.../file.py   # 只还原某个文件

# 已经提交了：
git revert HEAD                   # 撤销最新一次提交（安全）
git revert <commit-id>            # 撤销某次指定提交
```

**想试试一个改动，又怕影响主线：**
```bash
git checkout -b test-new-pid      # 开个新分支
# ... 改 PID 参数、跑一圈 ...
git commit -am "test: 尝试新的 PID 参数"
git checkout main                 # 回到主线
git branch -D test-new-pid        # 不满意，删掉分支
```

**查一段代码是谁写的：**
```bash
git blame src/robot_bringup/scripts/s9_lidar_driver.py
```

**比较当前和一周前的区别：**
```bash
git log --oneline --since="7 days ago"   # 看一周内的改动
git diff HEAD~5                          # 跟 5 次提交前比
```

### .gitignore 内容

```
build/ devel/ install/ .catkin_workspace
*.pyc __pycache__/ .pytest_cache/
.vscode/ .idea/
.claude/ .claud/ .omo/ .waylog/ .codegraph/
YDLIDAR/              # 第三方 vendored SDK，自带 git
*.docx *.doc *.ppt *.pptx *.pdf   # 二进制文档保留本地
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