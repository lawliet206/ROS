<!-- 感谢提交 PR！请填写以下内容。 -->

## 变更摘要

<!-- 改了什么、为什么改 -->

## 测试与验证

- [ ] `python3 -m pytest tests -q` 通过（或说明 CI 结果）
- [ ] CI 静态检查通过（compileall / bash -n / XML / YAML）
- [ ] 仿真验证（如适用）
- [ ] 实机验证（如适用，**底盘相关改动必须**）
- [ ] 未改动文档中已同步更新（README / SETUP / AGENTS）

## 安全自查

- [ ] 不影响启动/重启后静止状态
- [ ] 不削弱任何 `stop` 零速度保障
- [ ] 未修改 EKF 坐标系约定 / URDF 轮距 / ros_lib vendored 库

## 相关 Issue

<!-- Fixes #123 / Closes #456 -->