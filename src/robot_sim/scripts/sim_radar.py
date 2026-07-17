#!/usr/bin/env python3
"""
sim_radar.py — LD2402 毫米波雷达仿真
=====================================
用 Gazebo 激光数据模拟 LD2402 行为：
  - 只检测前方 ±60° 扇区
  - 输出最近物体的距离（米）
  - 无目标时输出 -1

用法:
  rosrun robot_sim sim_radar.py
  或通过 sim_follow_radar.sh 一键启动
"""
import rospy
import math
from std_msgs.msg import Float64, Bool
from sensor_msgs.msg import LaserScan


class SimRadar:
    def __init__(self):
        self.fov_deg = rospy.get_param("~fov", 120)       # 探测扇区 (±60°)
        self.max_range = rospy.get_param("~max_range", 8.0)  # 最远探测距离
        self.min_range = rospy.get_param("~min_range", 0.3)   # 最近探测距离

        self.presence = False
        self.distance = -1.0

        self.dist_pub = rospy.Publisher("/radar_distance", Float64, queue_size=10)
        self.presence_pub = rospy.Publisher("/radar_presence", Bool, queue_size=10)

        rospy.Subscriber("/scan", LaserScan, self.scan_callback)
        rospy.loginfo("[SimRadar] 已启动 (FOV=%d°, range=%.1fm)", self.fov_deg, self.max_range)

    def scan_callback(self, scan):
        half_fov = math.radians(self.fov_deg / 2.0)
        min_dist = float('inf')

        for i, r in enumerate(scan.ranges):
            if not math.isfinite(r):
                continue
            if r < self.min_range or r > self.max_range:
                continue

            angle = scan.angle_min + i * scan.angle_increment
            if abs(angle) <= half_fov:
                if r < min_dist:
                    min_dist = r

        if math.isfinite(min_dist):
            self.distance = min_dist
            self.presence = True
        else:
            self.distance = -1.0
            self.presence = False

        self.dist_pub.publish(Float64(self.distance))
        self.presence_pub.publish(Bool(self.presence))

        rospy.loginfo_throttle(1.0,
            "[SimRadar] dist=%.2fm presence=%s", self.distance, self.presence)


if __name__ == "__main__":
    rospy.init_node("sim_radar")
    try:
        SimRadar()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
