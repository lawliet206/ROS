#!/usr/bin/env python3
"""
laser_follower.py — 纯激光跟随 (带人体宽度约束)
==================================================
算法:
  1. 聚类激光点
  2. 筛选符合人体宽度的簇 (0.2~0.5m, 墙/柱子自动排除)
  3. 取最近簇 → 对准角度 → 调整距离
"""
import rospy
import math
from collections import deque
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist


class LaserFollower:

    def __init__(self):
        self.target_dist   = rospy.get_param("~target_dist",    1.0)
        self.max_linear    = rospy.get_param("~max_linear",     0.5)
        self.max_angular   = rospy.get_param("~max_angular",    0.6)
        self.min_dist      = rospy.get_param("~min_dist",       0.30)
        self.max_dist      = rospy.get_param("~max_dist",       5.0)
        self.cluster_tol   = rospy.get_param("~cluster_tol",    0.15)
        self.min_points    = rospy.get_param("~min_points",     5)
        self.min_body_width  = rospy.get_param("~min_body_width",  0.1)
        self.max_body_width  = rospy.get_param("~max_body_width",  0.55)
        self.min_lock_frames = rospy.get_param("~min_lock_frames", 3)

        self.kp_linear  = 0.4
        self.kp_angular = 0.5

        self.target_angle_ema = 0.0
        self.target_dist_ema = 0.0
        self.locked = False
        self.lock_counter = 0
        self.history = deque(maxlen=10)

        cmd_topic = rospy.get_param("~cmd_vel_topic", "/cmd_vel")
        self.cmd_pub = rospy.Publisher(cmd_topic, Twist, queue_size=10)
        rospy.Subscriber("/scan", LaserScan, self.scan_callback)

        rospy.on_shutdown(self.stop)
        rospy.loginfo("[Follow] width=%.2f~%.2fm lock=%dframes | dist=%.1fm vmax=%.1f",
                       self.min_body_width, self.max_body_width, self.min_lock_frames,
                       self.target_dist, self.max_linear)

    def stop(self):
        self.cmd_pub.publish(Twist())
        rospy.loginfo("[Follow] 已停止")

    def _cluster_width(self, cluster):
        """计算簇的物理宽度 (m)"""
        if len(cluster) < 2:
            return 0.0
        p0, p1 = cluster[0], cluster[-1]
        return math.hypot(p1[0] - p0[0], p1[1] - p0[1])

    def scan_callback(self, scan):
        points = []
        for i, r in enumerate(scan.ranges):
            if self.min_dist < r < self.max_dist and math.isfinite(r):
                a = scan.angle_min + i * scan.angle_increment
                if -1.57 < a < 1.57:
                    points.append((r * math.cos(a), r * math.sin(a)))

        if len(points) < self.min_points:
            self._lost()
            return

        # 基于欧氏距离的聚类
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
            self._lost()
            return

        # ---- 人体宽度约束 ----
        human_clusters = [c for c in clusters
                          if self.min_body_width <= self._cluster_width(c) <= self.max_body_width]

        rospy.logdebug("[Follow] clusters=%d human=%d",
                       len(clusters), len(human_clusters))

        if not human_clusters:
            self._lost()
            return

        # 从符合人体宽度的簇中取最近
        best = min(human_clusters, key=lambda c: math.hypot(
            sum(p[0] for p in c) / len(c),
            sum(p[1] for p in c) / len(c)))

        cx = sum(p[0] for p in best) / len(best)
        cy = sum(p[1] for p in best) / len(best)
        dist = math.hypot(cx, cy)
        angle = math.atan2(cy, cx)

        # ---- 连续锁定检测 ----
        self.history.append((angle, dist))
        self.lock_counter = min(self.lock_counter + 1, self.min_lock_frames * 2)

        if not self.locked and self.lock_counter >= self.min_lock_frames:
            self.locked = True
            # 刚锁定时用当前值初始化 EMA，避免冷启动滞后
            self.target_dist_ema  = dist
            self.target_angle_ema = angle
            rospy.loginfo("[Follow] 锁定目标")

        if self.locked:
            self.target_dist_ema  = 0.4 * dist  + 0.6 * self.target_dist_ema
            self.target_angle_ema = 0.4 * angle + 0.6 * self.target_angle_ema
        else:
            self.target_dist_ema  = dist
            self.target_angle_ema = angle

        self.control(self.target_angle_ema, self.target_dist_ema)

    def _lost(self):
        self.lock_counter = max(0, self.lock_counter - 1)
        self.history.clear()
        if self.locked and self.lock_counter == 0:
            self.locked = False
            rospy.loginfo("[Follow] 目标丢失")
        self.cmd_pub.publish(Twist())

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
