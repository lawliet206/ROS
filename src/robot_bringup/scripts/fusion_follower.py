#!/usr/bin/env python3
"""
fusion_follower.py — 激光+毫米波融合人体跟随
===============================================
策略:
  毫米波雷达(LD2402) → 先确认是否是人体
  激光雷达(S9)       → 确认后, 激光寻找最近目标并跟踪
  融合: 雷达做人体验证, 激光做实际跟踪(角度+距离)

实物启动:
  roslaunch robot_bringup follow.launch sensor:=fusion

仿真启动:
  终端1: bash ~/ROS/src/robot_sim/scripts/sim_follow_radar.sh
  终端2: rosrun robot_bringup fusion_follower.py
"""
import rospy
import math
from std_msgs.msg import Bool
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
        self.angle_thresh  = rospy.get_param("~angle_thresh", 0.1)
        self.cluster_tol   = rospy.get_param("~cluster_tol", 0.15)
        self.min_points    = rospy.get_param("~min_points", 5)
        self.presence_timeout = rospy.get_param("~presence_timeout", 2.0)
        self.search_angle    = rospy.get_param("~search_angle", 0.3)     # 搜索角速度 (rad/s)
        self.search_duration = rospy.get_param("~search_duration", 2.5)  # 每侧搜索时长 (s)

        # === 激光数据 ===
        self.target_angle = 0.0
        self.target_dist_laser = -1.0
        self.laser_valid = False
        self.laser_stale = 0.0

        # === 雷达数据 (人体确认门控) ===
        self.human_present = False
        self.presence_stale = 0.0

        # === 搜索状态 ===
        self.search_start = 0.0    # 本轮搜索开始时间
        self.search_done  = False  # 本轮搜索是否已完成

        # === 订阅 ===
        rospy.Subscriber("/scan", LaserScan, self._laser_cb)
        rospy.Subscriber("/radar_presence", Bool, self._presence_cb)

        # === 发布 ===
        self.cmd_pub = rospy.Publisher("/cmd_vel", Twist, queue_size=10)
        rospy.on_shutdown(self._stop)

        rospy.loginfo("[Fusion] 已启动 | 雷达验证+激光跟踪")

    def _stop(self):
        self.cmd_pub.publish(Twist())

    # ============================================================
    # 雷达回调 — 人体确认
    # ============================================================
    def _presence_cb(self, msg):
        self.presence_stale = rospy.Time.now().to_sec()
        self.human_present = msg.data

    # ============================================================
    # 激光回调 — 找最近目标
    # ============================================================
    def _laser_cb(self, scan):
        points = []
        for i, r in enumerate(scan.ranges):
            if 0.3 < r < 8.0 and math.isfinite(r):
                a = scan.angle_min + i * scan.angle_increment
                if -1.57 < a < 1.57:
                    points.append((r, a))

        if len(points) < self.min_points:
            self.laser_valid = False
            return

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
            self.laser_valid = False
            return

        # 取最近簇
        best = min(clusters, key=lambda c: min(p[0] for p in c))
        self.target_angle = sum(p[1] for p in best) / len(best)
        self.target_dist_laser = min(p[0] for p in best)
        self.laser_valid = True
        self.laser_stale = rospy.Time.now().to_sec()

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
        now = rospy.Time.now().to_sec()

        # 检查雷达人体确认是否过期
        presence_fresh = (now - self.presence_stale) < self.presence_timeout
        human = self.human_present and presence_fresh

        # 检查激光数据是否过期 (>1s)
        laser_fresh = self.laser_valid and (now - self.laser_stale) < 1.0

        # ---- 门控: 雷达没确认人体 ----
        if not human:
            # 首次进入搜索 → 记录开始时间
            if self.search_start == 0.0:
                self.search_start = now

            elapsed = now - self.search_start
            if not self.search_done:
                if elapsed < self.search_duration:
                    cmd.angular.z = -self.search_angle   # 阶段1: 向左
                elif elapsed < self.search_duration * 3:
                    cmd.angular.z = self.search_angle    # 阶段2: 向右 (2x, 跨中心)
                else:
                    self.search_done = True

            self.cmd_pub.publish(cmd)
            rospy.loginfo_throttle(2.0,
                "[Fusion] %s %.1fs",
                "搜索中..." if not self.search_done else "搜索完成, 停止",
                elapsed)
            return

        # ---- 有人确认, 重置搜索 ----
        self.search_start = 0.0
        self.search_done = False

        # ---- 激光没数据, 不动 ----
        if not laser_fresh:
            self.cmd_pub.publish(cmd)
            return

        # ---- 角速度: 对准目标 ----
        if abs(self.target_angle) > self.angle_thresh:
            cmd.angular.z = self.kp_angular * self.target_angle
            cmd.angular.z = max(-self.max_angular,
                                min(self.max_angular, cmd.angular.z))

        # ---- 线速度: 对准后才前后移动 ----
        aligned = abs(self.target_angle) < (self.angle_thresh * 3)
        if aligned and self.target_dist_laser > 0:
            err = self.target_dist_laser - self.target_dist
            if abs(err) > self.deadzone:
                cmd.linear.x = self.kp_linear * err
                cmd.linear.x = max(-self.max_linear,
                                   min(self.max_linear, cmd.linear.x))

        self.cmd_pub.publish(cmd)

        rospy.loginfo_throttle(1.0,
            "[Fusion] human=%s angle=%.1f° dist=%.2fm v=%.2f w=%.2f",
            human, math.degrees(self.target_angle),
            self.target_dist_laser,
            cmd.linear.x, cmd.angular.z)


if __name__ == "__main__":
    rospy.init_node("fusion_follower")
    try:
        FusionFollower().spin()
    except rospy.ROSInterruptException:
        pass
