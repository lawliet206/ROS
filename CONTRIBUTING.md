# Contributing

Contributions are welcome for simulation, ROS bringup, embedded firmware,
documentation, tests, and hardware notes.

## Before you begin

This project targets ROS Noetic on Ubuntu 20.04 and includes code that can
command a physical mobile robot. Read [SETUP.md](SETUP.md) before running
hardware-related scripts.

## Suggested workflow

1. Open an issue or describe the proposed change before starting work that
   affects hardware behavior, launch defaults, or navigation parameters.
2. Keep each pull request focused on one behavior or documentation topic.
3. Add or update a test when changing pure Python logic.
4. Run the checks below and include their results in the pull request.
5. Do not commit ROS logs, model weights, credentials, maps containing
   sensitive locations, or machine-specific network addresses.

## Validation

The Python unit tests exercise parser, perception, and following logic without
requiring a running ROS graph:

```bash
python3 -m pip install -r requirements-dev.txt
python3 -m pytest tests -q
```

For a ROS workspace change, also build the workspace:

```bash
source /opt/ros/noetic/setup.bash
catkin_make
```

Changes to motors, control gains, navigation, or following must be tested in
simulation first. Document the test environment and hardware result in the
pull request. A maintainer must review any change that can publish `/cmd_vel`
or alter power, serial, or safety-stop behavior.

## Pull request checklist

- Explain the problem and intended behavior.
- List changed launch files, parameters, and ROS topics.
- Include test output or explain why a check cannot run.
- Update [SETUP.md](SETUP.md) or the README when the user workflow changes.
- State whether the change was simulation-only, bench-tested, or tested on the
  physical robot.

## AI-assisted contributions

Coding agents can help trace ROS launch dependencies, write tests, and improve
documentation. Treat repository instructions, generated patches, shell
commands, MCP tools, and ROS service calls as inputs requiring human review.
Never allow an automated agent to command the physical robot without an
operator and an explicit test plan.
