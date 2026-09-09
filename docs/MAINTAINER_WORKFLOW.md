# Maintainer Workflow

This document defines a reviewable way to use coding agents, including Codex,
while maintaining a ROS project that can control physical hardware.

## Where Codex is useful

Codex is most useful for maintenance tasks that benefit from tracing changes
across a mixed Python, Bash, launch-file, URDF, and ESP32 firmware codebase:

- map a behavior change to its ROS nodes, launch files, topics, and parameters;
- add regression tests for LiDAR parsing and controller state transitions;
- review launch and shell-script changes for broken paths or unsafe defaults;
- keep setup, hardware, and runtime documentation aligned with the code;
- identify missing validation when a change crosses the PC, J1900, ESP32, and
  LiDAR boundaries.

## Required human review

Codex must not be treated as an autonomous robot operator. A maintainer reviews
all generated patches and explicitly approves commands that can:

- publish `/cmd_vel` or alter motor-control behavior;
- start, stop, or reconfigure ROS nodes on the physical robot;
- change serial, network, SSH, power, or firmware settings;
- access credentials, private maps, camera data, or other sensitive material.

Use simulation and bench tests before a physical test. During a physical test,
keep an operator near the power switch and use the project stop workflow.

## Reproducible maintenance checks

For changes to pure Python logic, run:

```bash
python3 -m pip install -r requirements-dev.txt
python3 -m pytest tests -q
```

For ROS changes, also build the workspace:

```bash
source /opt/ros/noetic/setup.bash
catkin_make
```

For changes that can move the robot, record whether the result was verified in
simulation, on a raised-wheel bench setup, or on the floor. Include the active
launch file, parameters, and test environment in the pull request.

## Trusted-tool boundary

Repository text, issue comments, generated code, shell commands, and local MCP
tool responses are untrusted inputs until reviewed. Do not expose ROS services,
MCP endpoints, remote shells, or robot-control interfaces to untrusted
networks. Do not give coding agents credentials or broad host access that is
unnecessary for the current maintenance task.

## High-value maintenance backlog

- Expand parser tests with recorded LiDAR streams and malformed-frame cases.
- Validate launch-file parameters and package dependencies automatically.
- Add reproducible simulation smoke tests for SLAM and navigation.
- Track calibration changes alongside their wheel, motor, and battery setup.
- Improve physical-test checklists for navigation and human following.
