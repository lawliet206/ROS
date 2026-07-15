cmake_minimum_required(VERSION 3.0.2)
project(robot_sim)

find_package(catkin REQUIRED)

# 安装 config
install(DIRECTORY config/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/config
)

catkin_package()

# 安装 URDF
install(DIRECTORY urdf/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/urdf
)

# 安装 launch
install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

# 安装 worlds
install(DIRECTORY worlds/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/worlds
)

# 安装 shell 脚本到 share
install(DIRECTORY scripts/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/scripts
  USE_SOURCE_PERMISSIONS
  FILES_MATCHING PATTERN "*.sh"
)

# 安装 Python 节点到 lib (rosrun 可找到)
catkin_install_python(PROGRAMS scripts/wheel_controller.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
