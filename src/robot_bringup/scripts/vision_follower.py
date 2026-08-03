#!/usr/bin/env python3
"""
vision_follower.py — 视觉+雷达融合人体跟随控制器 (PC 端)
==========================================================
角度来自视觉 (/person_angle), 距离来自雷达 (人体宽度约束聚类),
直接发布 /cmd_vel 驱动小车跟随.

3 态状态机:
  FOLLOW: 视觉角度 + 雷达距离 → 跟随控制
  SEARCH: 视觉连续丢失 → 左右摇摆 ±60° 搜索, 找到即回 FOLLOW
  STOP:   摇摆一整轮未找到 → 原地停止

用法:
  rosrun robot_bringup vision_follower.py
"""
import math
import rospy
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist
from std_msgs.msg import Float32, Bool


def _cluster_width(cluster):
    """簇的物理宽度 (m)"""
    if len(cluster) < 2:
        return 0.0
    p0, p1 = cluster[0], cluster[-1]
    return math.hypot(p1[0] - p0[0], p1[1] - p0[1])


def find_nearest_human_dist(scan, min_dist, max_dist, cluster_tol, min_points,
                            min_body_width, max_body_width):
    """从 LaserScan 找最近人体簇的距离

    聚类 + 人体宽度约束 (与 laser_follower.py 逻辑一致).

    Args:
        scan: 含 .ranges/.angle_min/.angle_increment 的对象 (LaserScan)
    Returns:
        最近人体簇距离 (m); 无则 None
    """
    points = []
    for i, r in enumerate(scan.ranges):
        if min_dist < r < max_dist and math.isfinite(r):
            a = scan.angle_min + i * scan.angle_increment
            if -1.57 < a < 1.57:
                points.append((r * math.cos(a), r * math.sin(a)))
    if len(points) < min_points:
        return None

    clusters = []
    cur = [points[0]]
    for i in range(1, len(points)):
        d = math.hypot(points[i][0] - cur[-1][0], points[i][1] - cur[-1][1])
        if d < cluster_tol:
            cur.append(points[i])
        else:
            if len(cur) >= min_points:
                clusters.append(cur)
            cur = [points[i]]
    if len(cur) >= min_points:
        clusters.append(cur)

    human = [c for c in clusters
             if min_body_width <= _cluster_width(c) <= max_body_width]
    if not human:
        return None

    best = min(human, key=lambda c: math.hypot(
        sum(p[0] for p in c) / len(c),
        sum(p[1] for p in c) / len(c)))
    return math.hypot(sum(p[0] for p in best) / len(best),
                      sum(p[1] for p in best) / len(best))


class FollowStateMachine:
    """融合跟随状态机: FOLLOW → SEARCH → STOP

    用命令积分近似实际旋转角度 (无里程计依赖), 到达 ±sweep_deg 反向.
    """
    FOLLOW = 0
    SEARCH = 1
    STOP = 2

    def __init__(self, lost_frames=5, sweep_deg=60.0, search_angular=0.6):
        self.lost_frames = lost_frames
        self.sweep_deg = sweep_deg
        self.search_angular = search_angular
        self.state = self.FOLLOW
        self.lost_count = 0
        self.sweep_angle = 0.0
        self.sweep_dir = 1.0

    def update(self, person_visible, dt):
        """状态机一步

        Args:
            person_visible: 视觉当前是否检测到人
            dt: 距上一步的时间 (s)
        Returns:
            (state, angular_cmd): 新状态 + 角速度指令 (非搜索态为 0)
        """
        if person_visible:
            if self.state != self.FOLLOW:
                rospy.loginfo("[Follow] 视觉恢复 → 跟随")
            self.state = self.FOLLOW
            self.lost_count = 0
            self.sweep_angle = 0.0
            self.sweep_dir = 1.0
            return self.state, 0.0

        if self.state == self.FOLLOW:
            self.lost_count += 1
            if self.lost_count >= self.lost_frames:
                self.state = self.SEARCH
                rospy.loginfo("[Follow] 视觉丢失 %d 帧 → 搜索", self.lost_count)
                self.sweep_angle = 0.0
                self.sweep_dir = 1.0
            return self.state, 0.0

        if self.state == self.SEARCH:
            self.sweep_angle += self.sweep_dir * self.search_angular * dt
            if self.sweep_angle >= math.radians(self.sweep_deg):
                self.sweep_dir = -1.0
            elif self.sweep_angle <= -math.radians(self.sweep_deg):
                self.state = self.STOP
                rospy.logwarn("[Follow] 摇摆 ±60° 一轮未找到 → 停止")
                return self.state, 0.0
            return self.state, self.sweep_dir * self.search_angular

        # STOP: 保持停止
        return self.state, 0.0


class VisionFollower:
    def __init__(self):
        self.target_dist   = rospy.get_param("~target_dist",   1.0)
        self.max_linear    = rospy.get_param("~max_linear",    0.5)
        self.max_angular   = rospy.get_param("~max_angular",   0.8)
        self.min_dist      = rospy.get_param("~min_dist",      0.30)
        self.max_dist      = rospy.get_param("~max_dist",      5.0)
        self.cluster_tol   = rospy.get_param("~cluster_tol",   0.15)
        self.min_points    = rospy.get_param("~min_points",    5)
        self.min_body_width = rospy.get_param("~min_body_width", 0.1)
        self.max_body_width = rospy.get_param("~max_body_width", 0.55)
        self.min_lock_frames = rospy.get_param("~min_lock_frames", 3)
        self.lost_frames   = rospy.get_param("~lost_frames",   5)
        self.search_sweep  = rospy.get_param("~search_sweep",  60.0)
        self.search_angular = rospy.get_param("~search_angular", 0.6)
        self.rate          = rospy.get_param("~rate", 10.0)

        self.kp_linear  = 0.4
        self.kp_angular = 0.5

        self.angle = 0.0
        self.person_visible = False
        self.last_vision_time = rospy.Time.now()
        self.latest_dist = None
        self.locked = False
        self.lock_counter = 0
        self.target_angle_ema = 0.0
        self.target_dist_ema = 0.0

        self.sm = FollowStateMachine(
            lost_frames=self.lost_frames,
            sweep_deg=self.search_sweep,
            search_angular=self.search_angular,
        )

        self.cmd_pub = rospy.Publisher("/cmd_vel", Twist, queue_size=10)
        rospy.Subscriber("/scan", LaserScan, self.scan_callback, queue_size=1)
        rospy.Subscriber("/person_angle", Float32, self.angle_callback, queue_size=1)
        rospy.Subscriber("/person_visible", Bool, self.visible_callback, queue_size=1)

        rospy.Timer(rospy.Duration(1.0 / self.rate), self.control_tick)
        rospy.on_shutdown(self.stop)
        rospy.loginfo("[Follow] vision+laser 融合 | dist=%.1fm vmax=%.1f | lost=%dframes sweep=%.0f°",
                      self.target_dist, self.max_linear, self.lost_frames, self.search_sweep)

    def stop(self):
        self.cmd_pub.publish(Twist())
        rospy.loginfo("[Follow] 已停止")

    def scan_callback(self, scan):
        self.latest_dist = find_nearest_human_dist(
            scan, self.min_dist, self.max_dist, self.cluster_tol,
            self.min_points, self.min_body_width, self.max_body_width)

    def angle_callback(self, msg):
        self.angle = msg.data
        self.last_vision_time = rospy.Time.now()

    def visible_callback(self, msg):
        self.person_visible = msg.data
        self.last_vision_time = rospy.Time.now()

    def control_tick(self, event):
        now = rospy.Time.now()
        # 视觉消息超时 0.5s 视为丢失 (检测节点挂掉/图像断流兜底)
        visible = self.person_visible and (now - self.last_vision_time).to_sec() < 0.5
        if event.last_real:
            dt = (event.current_real - event.last_real).to_sec()
        else:
            dt = 1.0 / self.rate

        state, angular = self.sm.update(visible, dt)
        cmd = Twist()

        if state == FollowStateMachine.FOLLOW:
            if not visible:
                self._decay_lock()
                self.cmd_pub.publish(cmd)
                return
            self.lock_counter = min(self.lock_counter + 1, self.min_lock_frames * 2)
            if not self.locked and self.lock_counter >= self.min_lock_frames:
                self.locked = True
                self.target_angle_ema = self.angle
                self.target_dist_ema = self.latest_dist if self.latest_dist is not None else self.target_dist
                rospy.loginfo("[Follow] 锁定目标")
            if self.locked:
                self.target_angle_ema = 0.4 * self.angle + 0.6 * self.target_angle_ema
                if self.latest_dist is not None:
                    self.target_dist_ema = 0.4 * self.latest_dist + 0.6 * self.target_dist_ema
                cmd = self._control(self.target_angle_ema, self.target_dist_ema)
            self.cmd_pub.publish(cmd)

        elif state == FollowStateMachine.SEARCH:
            self._reset_lock()  # 搜索后回跟随需重新初始化 EMA, 避免旧目标残留
            cmd.angular.z = angular
            self.cmd_pub.publish(cmd)
            rospy.loginfo_throttle(1, "[Follow] 搜索中 w=%.2f 已扫=%.0f°",
                                   angular, math.degrees(self.sm.sweep_angle))

        else:  # STOP
            self._reset_lock()
            self.cmd_pub.publish(cmd)

    def _decay_lock(self):
        self.lock_counter = max(0, self.lock_counter - 1)
        if self.lock_counter == 0:
            self.locked = False

    def _reset_lock(self):
        self.locked = False
        self.lock_counter = 0

    def _control(self, angle, dist):
        cmd = Twist()
        abs_angle = abs(angle)
        if abs_angle > 0.08:
            cmd.angular.z = max(-self.max_angular, min(self.max_angular,
                                                       self.kp_angular * angle))
        if abs_angle < 0.3:
            err = dist - self.target_dist
            if abs(err) > 0.15:
                cmd.linear.x = max(-self.max_linear, min(self.max_linear,
                                                         self.kp_linear * err))
        rospy.loginfo_throttle(1, "[Follow] dist=%.2fm angle=%.0f° | v=%.2f w=%.2f",
                               dist, math.degrees(angle), cmd.linear.x, cmd.angular.z)
        return cmd


if __name__ == "__main__":
    rospy.init_node("vision_follower")
    try:
        VisionFollower()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
