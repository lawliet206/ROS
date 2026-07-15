cmake_minimum_required(VERSION 3.0.2)
project(robot_bringup)

find_package(catkin REQUIRED)

catkin_package()

# 安装脚本（注：serial_bridge.py 已被 rosserial 取代，不再安装）
catkin_install_python(PROGRAMS
  scripts/s9_lidar_driver.py
  scripts/laser_follower.py
  scripts/send_goals.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# 安装 launch 文件
install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

# 安装 shell 脚本
install(PROGRAMS
  scripts/slam_start.sh
  scripts/nav_start.sh
  scripts/save_map.sh
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# 安装 urdf
install(DIRECTORY urdf/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/urdf
)

# 安装 config
install(DIRECTORY config/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/config
)
