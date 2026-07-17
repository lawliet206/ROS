#!/usr/bin/env python3
"""
fusion_follower.py — 激光+毫米波融合人体跟随
===============================================
策略:
  激光雷达 → 确定目标角度 (360° 扫描, 聚类找最近物体)
  毫米波雷达 → 提供精确距离 (LD2402 人体检测 / sim_radar 仿真)
  融合: 转向对准目标 (激光角度), 前后保持距离 (雷达距离)

实物启动:
  roslaunch robot_bringup follow.launch sensor:=fusion

仿真启动:
  终端1: bash ~/ROS/src/robot_sim/scripts/sim_follow_radar.sh
  终端2: rosrun robot_bringup fusion_follower.py
"""
import rospy
import math
from std_msgs.msg import Float64, Bool
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist


class FusionFollower:
    def __init__(self):
        # === 参数 ===
        self.target_dist   = rospy.get_param("~target_dist", 1.0)
        self.max_linear    = rospy.get_param("~max_linear", 0.5)
        self.max_angular   = rospy.get_param("~max_angular", 0.6)
        self.kp_linear     = rospy.get_param("~kp_linear", 0.4)
        self.kp_angular    = rospy.get_param("~kp_angular", 0.5)
        self.deadzone      = rospy.get_param("~deadzone", 0.2)
        self.angle_thresh  = rospy.get_param("~angle_thresh", 0.1)   # 角对准阈值 (rad)
        self.cluster_tol   = rospy.get_param("~cluster_tol", 0.15)    # 聚类容差 (m)
        self.min_points    = rospy.get_param("~min_points", 5)
        self.radar_timeout = rospy.get_param("~radar_timeout", 2.0)

        # === 激光数据 ===
        self.target_angle = 0.0     # 目标角度 (rad, 0=正前方)
        self.angle_valid = False
        self.angle_stale = 0.0      # 最近更新时间

        # === 雷达数据 ===
        self.radar_dist = -1.0
        self.radar_valid = False
        self.radar_stale = 0.0      # 最近更新时间

        # === 订阅 ===
        rospy.Subscriber("/scan", LaserScan, self._laser_cb)
        rospy.Subscriber("/radar_distance", Float64, self._radar_dist_cb)
        rospy.Subscriber("/radar_presence", Bool, self._radar_presence_cb)

        # === 发布 ===
        self.cmd_pub = rospy.Publisher("/cmd_vel", Twist, queue_size=10)
        rospy.on_shutdown(self._stop)

        rospy.loginfo("[Fusion] 已启动 | 激光角度 + 雷达距离")

    def _stop(self):
        self.cmd_pub.publish(Twist())

    # ============================================================
    # 雷达回调
    # ============================================================
    def _radar_dist_cb(self, msg):
        self.radar_stale = rospy.Time.now().to_sec()
        if msg.data > 0:
            self.radar_dist = msg.data
            self.radar_valid = True

    def _radar_presence_cb(self, msg):
        self.radar_stale = rospy.Time.now().to_sec()
        if not msg.data:
            self.radar_valid = False
            self.radar_dist = -1.0

    # ============================================================
    # 激光回调 — 聚类找最近目标的中心角度
    # ============================================================
    def _laser_cb(self, scan):
        # 只取前方 ±90°, 有效距离内的点
        points = []
        for i, r in enumerate(scan.ranges):
            if 0.3 < r < 8.0 and math.isfinite(r):
                a = scan.angle_min + i * scan.angle_increment
                if -1.57 < a < 1.57:
                    points.append((r, a))

        if len(points) < self.min_points:
            self.angle_valid = False
            return

        # 按相邻点距离聚类
        clusters = []
        cur = [points[0]]
        for i in range(1, len(points)):
            d = abs(points[i][0] - cur[-1][0])
            if d < self.cluster_tol:
                cur.append(points[i])
            else:
                if len(cur) >= self.min_points:
                    clusters.append(cur)
                cur = [points[i]]
        if len(cur) >= self.min_points:
            clusters.append(cur)

        if not clusters:
            self.angle_valid = False
            return

        # 取最近簇的质心角度
        best = min(clusters, key=lambda c: min(p[0] for p in c))
        self.target_angle = sum(p[1] for p in best) / len(best)
        self.angle_valid = True
        self.angle_stale = rospy.Time.now().to_sec()

    # ============================================================
    # 控制循环 (20Hz)
    # ============================================================
    def spin(self):
        rate = rospy.Rate(20)
        while not rospy.is_shutdown():
            self._control()
            rate.sleep()

    def _control(self):
        cmd = Twist()

        # 检查角度数据是否过期 (>1s)
        now = rospy.Time.now().to_sec()
        angle_fresh = self.angle_valid and (now - self.angle_stale) < 1.0

        # ---- 角速度: 对准目标 ----
        if angle_fresh and abs(self.target_angle) > self.angle_thresh:
            cmd.angular.z = self.kp_angular * self.target_angle
            cmd.angular.z = max(-self.max_angular,
                                min(self.max_angular, cmd.angular.z))

        # ---- 线速度: 对准后才前后移动 ----
        aligned = angle_fresh and abs(self.target_angle) < (self.angle_thresh * 3)
        radar_fresh = self.radar_valid and (now - self.radar_stale) < self.radar_timeout
        if aligned and radar_fresh:
            err = self.radar_dist - self.target_dist
            if abs(err) > self.deadzone:
                cmd.linear.x = self.kp_linear * err
                cmd.linear.x = max(-self.max_linear,
                                   min(self.max_linear, cmd.linear.x))

        self.cmd_pub.publish(cmd)

        rospy.loginfo_throttle(1.0,
            "[Fusion] laser_angle=%.1f° radar=%.2fm | v=%.2f w=%.2f",
            math.degrees(self.target_angle) if angle_fresh else -999,
            self.radar_dist if self.radar_valid else -1,
            cmd.linear.x, cmd.angular.z)


if __name__ == "__main__":
    rospy.init_node("fusion_follower")
    try:
        FusionFollower().spin()
    except rospy.ROSInterruptException:
        pass
