#!/usr/bin/env python3
"""
laser_follower.py — 纯激光跟随 (跟踪最近物体)
================================================
算法:
  1. 聚类激光点，找到最近的有效簇
  2. 先旋转对准目标，再前后调整距离
  3. Ctrl-C 自动停车
"""
import rospy
import math
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist


class LaserFollower:

    def __init__(self):
        self.target_dist  = rospy.get_param("~target_dist",  1.0)
        self.max_linear   = rospy.get_param("~max_linear",   0.5)
        self.max_angular  = rospy.get_param("~max_angular",  0.6)
        self.min_dist     = rospy.get_param("~min_dist",     0.30)
        self.max_dist     = rospy.get_param("~max_dist",     5.0)
        self.cluster_tol  = rospy.get_param("~cluster_tol",  0.15)
        self.min_points   = rospy.get_param("~min_points",   5)

        self.kp_linear  = 0.4
        self.kp_angular = 0.5

        self.target_angle = 0.0
        self.target_dist_ema = 0.0
        self.locked = False

        cmd_topic = rospy.get_param("~cmd_vel_topic", "/cmd_vel")
        self.cmd_pub = rospy.Publisher(cmd_topic, Twist, queue_size=10)
        rospy.Subscriber("/scan", LaserScan, self.scan_callback)

        rospy.on_shutdown(self.stop)
        rospy.loginfo("[Follow] 已启动 (dist=%.1fm, vmax=%.1f, wmax=%.1f)",
                       self.target_dist, self.max_linear, self.max_angular)

    def stop(self):
        self.cmd_pub.publish(Twist())
        rospy.loginfo("[Follow] 已停止")

    def scan_callback(self, scan):
        # 收集有效点 (只看前方 ±90°)
        points = []
        for i, r in enumerate(scan.ranges):
            if self.min_dist < r < self.max_dist and math.isfinite(r):
                a = scan.angle_min + i * scan.angle_increment
                if -1.57 < a < 1.57:   # 只看前方, 忽略身后
                    points.append((r * math.cos(a), r * math.sin(a)))

        if len(points) < self.min_points:
            if not self.locked:
                self.cmd_pub.publish(Twist())
            return

        # 聚类：相邻点距离 < cluster_tol 的归为一簇
        clusters = []
        cur = [points[0]]
        for i in range(1, len(points)):
            d = math.hypot(points[i][0] - cur[-1][0],
                           points[i][1] - cur[-1][1])
            if d < self.cluster_tol:
                cur.append(points[i])
            else:
                if len(cur) >= self.min_points:
                    clusters.append(cur)
                cur = [points[i]]
        if len(cur) >= self.min_points:
            clusters.append(cur)

        if not clusters:
            if not self.locked:
                self.cmd_pub.publish(Twist())
            return

        # 取最近（距离最小）的簇
        best = min(clusters, key=lambda c: math.hypot(
            sum(p[0] for p in c) / len(c),
            sum(p[1] for p in c) / len(c)))

        cx = sum(p[0] for p in best) / len(best)
        cy = sum(p[1] for p in best) / len(best)
        dist = math.hypot(cx, cy)
        angle = math.atan2(cy, cx)

        # EMA 滤波
        if self.locked:
            self.target_dist_ema = 0.5 * dist + 0.5 * self.target_dist_ema
        else:
            self.target_dist_ema = dist
            self.locked = True

        # 控制
        self.control(angle, self.target_dist_ema)

    def control(self, angle, dist):
        cmd = Twist()
        abs_angle = abs(angle)

        # 角速度：对准目标
        if abs_angle > 0.08:
            cmd.angular.z = self.kp_angular * angle
            cmd.angular.z = max(-self.max_angular, min(self.max_angular, cmd.angular.z))

        # 线速度：只有对准了才前后移动
        if abs_angle < 0.3:
            err = dist - self.target_dist
            if abs(err) > 0.15:
                cmd.linear.x = self.kp_linear * err
                cmd.linear.x = max(-self.max_linear, min(self.max_linear, cmd.linear.x))

        self.cmd_pub.publish(cmd)

        rospy.loginfo_throttle(1,
            "[Follow] dist=%.2fm angle=%.0f° | v=%.2f w=%.2f",
            dist, math.degrees(angle), cmd.linear.x, cmd.angular.z)


if __name__ == "__main__":
    rospy.init_node("laser_follower")
    try:
        LaserFollower()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
