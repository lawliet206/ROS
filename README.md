# ROS Differential-Drive Robot

A ROS Noetic mobile robot project that connects simulation, embedded motor control, LiDAR, odometry, sensor fusion, SLAM, navigation, and human following on a self-built differential-drive platform.

The repository is organized around one practical workflow:

```text
Gazebo simulation -> ROS validation -> physical robot bringup -> real-world tuning
```

> Project status: actively developed. The physical platform is running, while real-world navigation, patrol, and human-following behavior still require environment-specific tuning.

<p align="center">
  <img src="assets/robot.jpg" width="780" alt="Self-built differential-drive robot">
</p>

<p align="center">
  <strong>Self-built differential-drive robot running on a real test field</strong>
</p>

## Demo

### Physical robot

The video is a short real-world run of the physical robot. It demonstrates the assembled platform moving on a test field; it is intentionally described as a physical robot demo rather than a claim of fully autonomous navigation.

<p align="center">
  <a href="assets/demo.mp4"><img src="assets/robot.jpg" width="780" alt="Open the physical robot demo video"></a>
</p>

<p align="center">
  <a href="assets/demo.mp4">Open the physical robot demo (MP4)</a>
</p>

### LiDAR SLAM mapping

The physical robot publishes LiDAR and odometry data to ROS. The resulting 2D occupancy grid can be inspected in RViz and saved for later localization and navigation.

<p align="center">
  <img src="assets/mapping.jpg" width="900" alt="RViz LiDAR SLAM map">
</p>

## Highlights

- ROS Noetic packages for both simulation and physical bringup.
- Gazebo differential-drive simulation with LiDAR, SLAM, navigation, and following experiments.
- ESP32 low-level control for motor PWM, encoder feedback, PID, IMU acquisition, and rosserial communication.
- S9 LiDAR driver and scan processing for mapping, obstacle information, and following.
- EKF fusion of wheel odometry and MPU6050 IMU data through `robot_localization`.
- AMCL localization, `move_base`, TEB local planning, and multi-goal patrol workflows.
- LiDAR-only and vision-plus-LiDAR human-following nodes.
- Python tests for the perception and LiDAR-related nodes.
- Startup and stop scripts for coordinating the ROS PC, J1900 onboard computer, ESP32, LiDAR, and camera.

## System architecture

The default physical setup uses a ROS PC as the master and a J1900 as the onboard computer. The two computers communicate over Wi-Fi. The J1900 connects to the ESP32 and LiDAR over USB, while the ESP32 drives the motors and reads the encoders and IMU.

```text
                          ROS PC
            RViz / SLAM / AMCL / move_base / TEB
                              |
                         Wi-Fi network
                              |
                         J1900 onboard PC
                    rosserial / LiDAR / camera
                         |              |
                        USB            USB
                         |              |
                       ESP32          S9 LiDAR
                 PWM / encoders       LaserScan
                 PID / MPU6050            |
                         |                |
                    TB6612FNG <-----------+
                      |     |
                Left motor  Right motor
```

The main data paths are:

```text
S9 LiDAR -> LaserScan -> scan processing -> gmapping -> /map
Encoders -> /odom
MPU6050 -> /imu
/odom + /imu -> robot_localization EKF -> /odometry/filtered
/map + odometry + LaserScan -> AMCL / move_base -> /cmd_vel
/cmd_vel -> rosserial -> ESP32 -> motor PID -> motors
```

## Software stack

| Component | Technology |
| --- | --- |
| Operating system | Ubuntu 20.04 |
| Middleware | ROS Noetic |
| Simulation | Gazebo |
| Visualization | RViz |
| SLAM | `gmapping` |
| Localization | `amcl` |
| Navigation | `move_base` |
| Local planner | TEB local planner |
| Sensor fusion | `robot_localization` EKF |
| Embedded communication | `rosserial` |
| Robot model | URDF |
| Main languages | Python, Bash, C++, Arduino |

## Hardware

The current documented hardware configuration is:

| Component | Hardware |
| --- | --- |
| ROS onboard computer | J1900 x86_64 computer |
| Microcontroller | ESP32-WROOM-32 |
| Motor driver | TB6612FNG dual H-bridge |
| Motors | JGB37-520 geared motors with encoders |
| LiDAR | S9-FSRD-V1.0 |
| IMU | MPU6050 |
| Drive type | Differential drive |
| Wheel diameter | 85 mm |
| Battery | 3S LiPo |

See [SETUP.md](SETUP.md) for the two-computer network, USB serial, GPIO, power, calibration, and first-power-on procedures.

## Repository structure

```text
ROS/
|-- src/
|   |-- robot_bringup/
|   |   |-- config/
|   |   |-- launch/
|   |   |-- scripts/
|   |   `-- urdf/
|   `-- robot_sim/
|       |-- config/
|       |-- launch/
|       |-- rviz/
|       |-- scripts/
|       |-- urdf/
|       `-- worlds/
|-- esp32_firmware/
|-- tests/
|-- tools/
|-- docs/
|-- assets/
|   |-- robot.jpg
|   |-- mapping.jpg
|   `-- demo.mp4
|-- SETUP.md
`-- README.md
```

## Quick start

The commands below assume Ubuntu 20.04 with ROS Noetic installed.

```bash
git clone https://github.com/lawliet206/ROS.git
cd ROS

source /opt/ros/noetic/setup.bash
catkin_make
source devel/setup.bash
```

Install the complete dependency list and configure the PC/J1900 network by following [SETUP.md](SETUP.md). Every new shell that runs ROS commands must source both the ROS installation and this workspace.

## Simulation

Simulation commands are intended to run on the ROS PC without the physical robot or J1900.

### Gazebo SLAM

```bash
bash src/robot_sim/scripts/sim_slam.sh
```

Drive the robot with `teleop_twist_keyboard` in another terminal, then save a map:

```bash
rosrun teleop_twist_keyboard teleop_twist_keyboard.py
rosrun map_server map_saver -f ~/maps/sim_map
```

### Simulated navigation

```bash
bash src/robot_sim/scripts/sim_navigation.sh ~/maps/sim_map.yaml
```

The simulated navigation launch includes AMCL, `move_base`, costmaps, and the TEB local planner.

### Simulated LiDAR following

```bash
bash src/robot_sim/scripts/sim_follow.sh
```

## Physical robot

The physical startup script runs on the ROS PC. It starts and stops the PC-side ROS nodes and uses SSH to coordinate the J1900 hardware services.

Before using the script:

1. Configure ROS networking between the PC and J1900.
2. Confirm passwordless SSH from the PC to the J1900.
3. Deploy `src/robot_bringup` to the J1900 as described in [SETUP.md](SETUP.md).
4. Confirm the robot is lifted or otherwise mechanically safe for the first motor test.

Check the current state:

```bash
bash src/robot_bringup/scripts/robot_start.sh status
```

### Real-robot SLAM

```bash
bash src/robot_bringup/scripts/robot_start.sh slam
```

The script starts the ESP32 and LiDAR side, waits for real ROS topics, launches `gmapping`, and opens RViz when a graphical session is available. Save a map with:

```bash
bash src/robot_bringup/scripts/save_map.sh lab_map
```

### Multi-goal patrol

Edit the patrol points for the map you saved. The checked-in file is a template and should not be treated as safe physical coordinates without validation.

```bash
nano src/robot_bringup/config/patrol_goals.yaml
bash src/robot_bringup/scripts/robot_start.sh patrol \
  ~/maps/lab_map.yaml \
  src/robot_bringup/config/patrol_goals.yaml
```

### Human following

The physical following workflow combines the camera detector with LiDAR-based range and direction information. It requires the YOLOv8n weights on the ROS workspace and is considered experimental outside the tested environment.

```bash
bash src/robot_bringup/scripts/robot_start.sh follow
```

The lower-level LiDAR-only follower is also available through `follow.launch` for isolated testing.

### Stop safely

Always stop the active physical mode through the project script. It publishes a zero velocity before stopping the PC and J1900 nodes.

```bash
bash src/robot_bringup/scripts/robot_start.sh stop
```

## ESP32 firmware

Open [esp32_firmware/esp32_firmware.ino](esp32_firmware/esp32_firmware.ino) in Arduino IDE and flash an ESP32 Dev Module. The firmware handles:

- dual motor PWM and direction control;
- encoder pulse counting and wheel-speed estimation;
- wheel PID control;
- MPU6050 communication;
- odometry and IMU message publication;
- `/cmd_vel` subscription through rosserial;
- watchdog and stop behavior.

The documented rosserial baud rate is 115200. Follow the wiring and serial-device checks in [SETUP.md](SETUP.md) before connecting the motors or starting the J1900 bridge.

## Sensor fusion

The shared launch file [odom_ekf.launch](src/robot_bringup/launch/odom_ekf.launch) configures the `robot_localization` EKF for encoder odometry and MPU6050 data. The SLAM, navigation, and standalone EKF launch files include this configuration where needed.

```text
/odom + /imu
     |
     v
robot_localization EKF
     |
     v
/odometry/filtered and odom -> base_footprint TF
```

Do not start multiple copies of the EKF node at the same time. Duplicate publishers can interrupt TF and destabilize localization.

## AI-assisted development and security

The repository contains instruction files for local coding-agent workflows, including [AGENTS.md](AGENTS.md), [CLAUDE.md](CLAUDE.md), and [CURSOR.md](CURSOR.md). These files are development aids, not a replacement for reviewing commands before they are run.

Any local agent, MCP integration, shell runner, ROS service caller, or hardware-control tool used with this project should be restricted to a trusted development environment. Review repository instructions, shell commands, dependencies, credentials, and ROS service calls before granting an automated tool access to the workspace or robot.

The physical robot can move and interact with its environment. Test changes with the wheels lifted or at low speed first, keep an operator near the power switch, and never expose robot-control interfaces directly to an untrusted network.

## Project status

| Area | Status |
| --- | --- |
| Differential-drive hardware | Running on the physical prototype |
| ESP32 motor control | Implemented and tested |
| Encoder and MPU6050 data | Implemented |
| S9 LiDAR driver | Implemented |
| Gazebo simulation | Available |
| Physical SLAM | Implemented and validated in the documented setup |
| EKF sensor fusion | Implemented and under hardware testing |
| AMCL and `move_base` | Implemented; real-world tuning continues |
| TEB local planning | Implemented; parameter tuning continues |
| Multi-goal patrol | Available in code; physical route validation remains |
| Human following | Experimental; requires further field tuning |
| Automated tests | Present under `tests/` |

## Known limitations

- Navigation parameters are environment- and floor-dependent.
- The checked-in patrol goal file is intentionally conservative and must be edited for a validated map.
- Physical SLAM and navigation depend on correct ROS networking, TF, serial-device detection, and power delivery.
- LiDAR and camera placement affect following performance.
- Motor, wheel, battery, and encoder changes require recalibration.
- Simulation results do not guarantee the same behavior on the physical platform.

## Roadmap

- [ ] Improve real-world navigation stability.
- [ ] Tune TEB and costmap parameters for additional environments.
- [ ] Improve SLAM consistency and scan processing.
- [ ] Complete physical multi-goal patrol validation.
- [ ] Improve human-following robustness and recovery behavior.
- [ ] Expand automated tests and reproducible launch checks.
- [ ] Improve simulation-to-real consistency.
- [ ] Document hardware assembly and calibration with more photographs.
- [ ] Harden local agent and ROS tool execution boundaries.

## Contributing

Issues, fixes, hardware notes, and documentation improvements are welcome. For a useful bug report, include:

```text
ROS version:
Ubuntu version:
Hardware:
Relevant launch file or script:
Relevant configuration:
Error message:
ROS logs:
Steps to reproduce:
```

For changes that can move the robot, include the test environment and the safety procedure used.

## Documentation

- [SETUP.md](SETUP.md): installation, networking, wiring, first power-on, simulation, and physical operation.
- Hardware and mechanical notes are documented in the repository-root hardware notes file.
- [Robot bringup launch files](src/robot_bringup/launch/): real-robot SLAM, navigation, EKF, and following.
- [Simulation launch files](src/robot_sim/launch/): Gazebo, simulated SLAM, and navigation.
- [Tests](tests/): unit tests for LiDAR and following-related Python nodes.

## License

The ROS package manifests currently declare MIT. This checkout does not contain a top-level `LICENSE` file, so add one before distributing the repository under a formal license.

## About

This project is an ongoing exploration of self-built mobile robotics with ROS, embedded control, SLAM, navigation, perception, and careful simulation-to-real validation.

Repository: https://github.com/lawliet206/ROS
