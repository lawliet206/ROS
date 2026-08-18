# ROS Differential-Drive Robot 🤖

A complete ROS Noetic differential-drive mobile robot project covering **simulation, embedded control, LiDAR, odometry, sensor fusion, SLAM, navigation, and human following**.

The project is designed to provide a practical path from **Gazebo simulation to a physical robot**, with the ROS navigation stack running on a real differential-drive platform.

> 🚧 This project is actively developed. Some real-world navigation and following functions are still being validated and tuned.

<p align="center">
  <img src="assets/robot.jpg" width="700">
</p>

<p align="center">
  <b>Physical differential-drive robot</b>
</p>

---

## 🎥 Demo

### Physical Robot

The following video shows the physical robot running on a real test field.

[▶️ **Watch the physical robot demo**](assets/demo.mp4)

### SLAM Mapping

The robot uses LiDAR-based SLAM to build a 2D map of the environment.

<p align="center">
  <img src="assets/mapping.jpg" width="800">
</p>

---

## ✨ Features

* Differential-drive mobile robot
* ROS Noetic
* Gazebo simulation
* ESP32-based motor controller
* Encoder-based odometry
* MPU6050 IMU
* S9 LiDAR
* LiDAR driver
* EKF sensor fusion
* SLAM mapping
* AMCL localization
* ROS Navigation Stack
* TEB local planner
* Multi-goal navigation
* LiDAR-based human following
* URDF robot model
* Physical robot deployment
* AI-assisted development workflow
* Local ROS MCP tools for development and debugging

---

## 🧩 System Architecture

```text
                         ┌──────────────────────────┐
                         │       ROS Noetic PC      │
                         │                          │
                         │  RViz / SLAM / AMCL      │
                         │  Navigation / TEB / EKF  │
                         └────────────┬─────────────┘
                                      │
                                   Network
                                      │
                         ┌────────────▼─────────────┐
                         │          J1900           │
                         │        ROS Computer      │
                         │                          │
                         │     LiDAR / ROS Nodes    │
                         └───────┬──────────┬────────┘
                                 │          │
                                USB        USB
                                 │          │
                    ┌────────────▼───┐   ┌──▼────────────┐
                    │     ESP32      │   │   S9 LiDAR    │
                    │                │   │               │
                    │ Motor Control  │   │  Laser Scan   │
                    │ Encoder PCNT   │   │    Driver     │
                    │ PID            │   └───────────────┘
                    │ MPU6050        │
                    │ rosserial      │
                    └───────┬────────┘
                            │
                       TB6612FNG
                            │
                  ┌─────────┴─────────┐
                  │                   │
             Left Motor          Right Motor
```

---

## 🖥️ Software Stack

| Component           | Technology               |
| ------------------- | ------------------------ |
| Operating System    | Ubuntu                   |
| Robot Middleware    | ROS Noetic               |
| Simulation          | Gazebo                   |
| Visualization       | RViz                     |
| SLAM                | GMapping                 |
| Localization        | AMCL                     |
| Global Planning     | Navfn                    |
| Local Planning      | TEB                      |
| Sensor Fusion       | robot_localization / EKF |
| Embedded Controller | ESP32                    |
| Communication       | rosserial                |
| LiDAR               | S9                       |
| Robot Model         | URDF                     |

---

## 🤖 Hardware

The current physical robot uses the following main components:

| Component       | Hardware           |
| --------------- | ------------------ |
| Main Computer   | J1900              |
| Microcontroller | ESP32-WROOM-32     |
| Motor Driver    | TB6612FNG          |
| Motors          | JGB37-520          |
| LiDAR           | S9                 |
| IMU             | MPU6050            |
| Drive Type      | Differential Drive |
| Wheel Diameter  | 85 mm              |
| Battery         | 3S LiPo            |

The hardware configuration and GPIO assignments are documented in the repository.

---

## 📁 Repository Structure

```text
ROS/
├── robot_bringup/
│   ├── launch/
│   ├── config/
│   ├── urdf/
│   ├── scripts/
│   └── ...
│
├── robot_sim/
│   ├── launch/
│   ├── worlds/
│   ├── urdf/
│   └── ...
│
├── esp32_firmware/
│   └── ...
│
├── S9/
│   └── ...
│
├── tools/
│   └── ...
│
├── assets/
│   ├── robot.jpg
│   ├── mapping.jpg
│   └── demo.mp4
│
└── README.md
```

---

# 🚀 Quick Start

## 1. Clone the repository

```bash
git clone https://github.com/lawliet206/ROS.git
cd ROS
```

## 2. Source ROS Noetic

```bash
source /opt/ros/noetic/setup.bash
```

## 3. Build the workspace

```bash
catkin_make
```

Then source the workspace:

```bash
source devel/setup.bash
```

> The exact launch file depends on whether you are running the simulation or the physical robot.

---

# 🧪 Simulation

The repository includes a Gazebo simulation environment for developing and testing the robot without physical hardware.

The simulation environment is intended to support:

* Differential-drive control
* LiDAR simulation
* SLAM
* Navigation
* Localization
* Human-following experiments

Typical workflow:

```text
Gazebo
  ↓
Robot Model
  ↓
LiDAR / Odometry
  ↓
SLAM
  ↓
Map
  ↓
AMCL
  ↓
Navigation
  ↓
TEB
```

---

# 🗺️ SLAM

The robot uses a 2D LiDAR-based SLAM workflow to build an occupancy grid map.

The physical robot publishes LiDAR and odometry information to ROS, which can then be used by the SLAM system.

Main data flow:

```text
S9 LiDAR
   ↓
LaserScan
   ↓
SLAM
   +
Odometry
   ↓
Occupancy Grid Map
```

The resulting map can be visualized in RViz and saved for later navigation.

---

# 🧭 Navigation

The navigation stack combines:

* Global planning
* Local planning
* AMCL localization
* Costmaps
* TF
* Odometry
* LiDAR obstacle information
* TEB local planner

The intended navigation pipeline is:

```text
Map
 ↓
AMCL
 ↓
Move Base
 ├── Global Planner
 └── TEB Local Planner
       ↓
    cmd_vel
       ↓
     ESP32
       ↓
     Motors
```

---

# 👤 Human Following

The repository also contains a LiDAR-based human-following workflow.

The basic concept is:

```text
LiDAR
 ↓
Laser Scan Processing
 ↓
Human Detection
 ↓
Target Position
 ↓
Velocity Command
 ↓
Robot
```

This function is currently considered experimental and requires further real-world tuning.

---

# 🔧 ESP32 Firmware

The ESP32 acts as the low-level controller of the robot.

Its main responsibilities include:

* Motor PWM control
* Encoder acquisition
* Wheel speed measurement
* PID control
* MPU6050 communication
* Odometry-related data
* ROS communication through rosserial

The architecture separates high-level robotics algorithms from low-level motor control:

```text
ROS
 │
 │ cmd_vel
 ▼
ESP32
 │
 ├── PID
 ├── PWM
 ├── Encoder
 └── Motor Driver
```

This allows the ROS computer to focus on localization, mapping, planning, and perception while the ESP32 handles real-time motor control.

---

# 📡 Sensor Fusion

The robot combines wheel odometry and IMU information through an EKF-based sensor-fusion workflow.

```text
Wheel Encoder
      │
      ▼
  Odometry ─────┐
                │
                ▼
              EKF
                ▲
                │
             MPU6050
                │
                ▼
          Filtered State
```

This is used to improve the stability of the robot's estimated motion state.

---

# 🧠 AI-Assisted Development

This project also experiments with **AI-assisted robotics development**.

AI coding agents are used as development assistants for:

* ROS code analysis
* Debugging
* Configuration analysis
* Build troubleshooting
* Documentation
* Repository navigation
* ROS development workflows

The repository includes a local ROS MCP development tool that allows an AI agent to interact with the ROS development environment.

---

# 🔌 ROS MCP

The project contains a local MCP server for AI-assisted ROS development.

The current tooling includes operations for tasks such as:

* Building the ROS workspace
* Launching ROS nodes
* Listing ROS nodes
* Listing ROS topics
* Inspecting topic messages
* Calling ROS services
* Inspecting launch files

Conceptually:

```text
             AI Agent
                 │
                 ▼
              MCP Tool
                 │
        ┌────────┴────────┐
        │                 │
      Build             ROS
        │             Operations
        ▼                 │
   catkin_make        Nodes / Topics
                          │
                          ▼
                       Robot
```

These tools are intended for trusted development environments.

Because some MCP operations can interact with the local ROS environment or execute development commands, they should **not be exposed to untrusted agents or untrusted network clients**.

---

# 🔐 Security Considerations

Because this repository combines robotics software with AI-assisted development tools, security is considered at several boundaries.

Potential areas include:

* Repository-level prompt injection
* Malicious AI-agent instructions
* Unsafe shell command execution
* Unsafe subprocess usage
* MCP trust boundaries
* Third-party MCP dependencies
* Credential and environment-variable exposure
* Malicious or compromised dependencies
* Unsafe ROS service invocation
* Untrusted repository contributions

In particular, files such as agent instructions and development configuration may influence the behavior of AI coding agents.

The project therefore treats AI-agent instructions, MCP tools, shell commands, and ROS execution interfaces as separate trust boundaries.

> AI-assisted development tools should only be used in trusted development environments and should be reviewed by a human before executing potentially destructive or hardware-affecting operations.

---

# 📊 Project Status

| Component                 | Status                  |
| ------------------------- | ----------------------- |
| Differential-drive robot  | ✅ Working               |
| ESP32 motor control       | ✅ Working               |
| Encoder feedback          | ✅ Implemented           |
| MPU6050                   | ✅ Implemented           |
| S9 LiDAR                  | ✅ Implemented           |
| Gazebo simulation         | ✅ Available             |
| SLAM                      | ✅ Implemented / Testing |
| AMCL                      | ✅ Implemented           |
| Navigation                | ⚠️ Active tuning        |
| TEB                       | ⚠️ Active tuning        |
| EKF sensor fusion         | ⚠️ Testing              |
| Human following           | ⚠️ Experimental         |
| Physical robot validation | 🚧 Active development   |

---

# 🐛 Known Issues

Current development focuses on improving real-world robustness.

Known areas for improvement include:

* The global planner can occasionally generate inefficient paths.
* TEB behavior near the goal still requires parameter tuning.
* Some Gazebo mapping configurations may produce minor mapping artifacts.
* Physical-world navigation requires additional testing across different environments.
* Human-following performance still requires further real-world validation.

These issues are documented intentionally so that the project remains transparent about its current development state.

---

# 🛣️ Roadmap

* [ ] Improve real-world navigation stability
* [ ] Further tune TEB parameters
* [ ] Improve SLAM consistency
* [ ] Complete multi-goal navigation validation
* [ ] Improve human-following robustness
* [ ] Expand automated testing
* [ ] Improve simulation-to-real consistency
* [ ] Harden MCP and AI-agent execution boundaries
* [ ] Improve project documentation
* [ ] Add more reproducible development workflows

---

# 🤝 Contributing

Contributions, bug reports, improvements, and discussions are welcome.

If you find a problem with:

* ROS nodes
* Navigation
* SLAM
* Sensor fusion
* ESP32 firmware
* Simulation
* Documentation
* AI-assisted development tooling

please open an issue with enough information to reproduce the problem.

For robotics-related issues, including the following information when possible:

```text
ROS version:
Ubuntu version:
Hardware:
Relevant launch file:
Relevant configuration:
Error message:
ROS logs:
Steps to reproduce:
```

---

# ⚠️ Development Notes

This project is primarily intended for **education, research, experimentation, and robotics development**.

The physical robot can interact with its environment. Always test new control, navigation, and AI-assisted changes in a controlled environment before deploying them to hardware.

Never expose ROS services, MCP tools, shell execution interfaces, or robot-control interfaces directly to untrusted networks.

---

# 📚 Documentation

More detailed documentation will be added progressively for:

* Hardware assembly
* ESP32 firmware
* ROS bringup
* Gazebo simulation
* SLAM
* Navigation
* Sensor fusion
* Human following
* AI-assisted development
* ROS MCP tools

---

# 📄 License

See the repository license file for the current licensing terms.

---

## ⭐ About

This project is an ongoing exploration of **ROS-based mobile robotics, embedded control, SLAM, autonomous navigation, and AI-assisted robotics development**.

The long-term goal is to build a reproducible and extensible robotics platform that connects:

```text
Embedded Control
       +
ROS
       +
Simulation
       +
SLAM / Navigation
       +
AI-Assisted Development
```

If you find the project useful, consider giving it a ⭐ on GitHub.

**Repository:**
[https://github.com/lawliet206/ROS](https://github.com/lawliet206/ROS)
