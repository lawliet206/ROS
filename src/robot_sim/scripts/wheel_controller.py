#!/usr/bin/env python3
"""
Diff-drive wheel controller using apply_joint_effort.
P feedback + small feedforward, velocity estimated from joint positions.
"""
import rospy
from geometry_msgs.msg import Twist
from gazebo_msgs.srv import ApplyJointEffort
from sensor_msgs.msg import JointState


class WheelController:
    def __init__(self):
        rospy.init_node("wheel_controller")

        self.wheel_radius = rospy.get_param("~wheel_radius", 0.075)
        self.wheel_separation = rospy.get_param("~wheel_separation", 0.30)
        self.max_effort = rospy.get_param("~max_effort", 15.0)
        self.kp = rospy.get_param("~kp", 1.5)
        self.k_ff = rospy.get_param("~k_ff", 0.8)

        # State
        self.target_v = 0.0
        self.target_w = 0.0
        self.last_cmd_time = rospy.Time.now()
        self.last_update = rospy.Time.now()

        # Velocity estimation
        self.last_left_pos = None
        self.last_right_pos = None
        self.last_js_time = None
        self.left_vel = 0.0
        self.right_vel = 0.0

        # Debug
        self.loop_count = 0

        rospy.loginfo("Waiting for /gazebo/apply_joint_effort ...")
        rospy.wait_for_service("/gazebo/apply_joint_effort")
        self.apply_effort = rospy.ServiceProxy(
            "/gazebo/apply_joint_effort", ApplyJointEffort
        )
        rospy.loginfo("Service connected.")

        rospy.Subscriber("/cmd_vel", Twist, self.cmd_vel_cb)
        rospy.Subscriber("/joint_states", JointState, self.joint_state_cb)
        rospy.loginfo("Wheel controller ready (kp=%.1f, ff=%.1f, max=%.1f Nm)",
                       self.kp, self.k_ff, self.max_effort)

    def cmd_vel_cb(self, msg):
        self.target_v = msg.linear.x
        self.target_w = msg.angular.z
        self.last_cmd_time = rospy.Time.now()

    def joint_state_cb(self, msg):
        try:
            idx_l = msg.name.index("left_wheel_joint")
            idx_r = msg.name.index("right_wheel_joint")
            now = rospy.Time.now()
            pos_l = msg.position[idx_l]
            pos_r = msg.position[idx_r]
            if self.last_left_pos is not None and self.last_js_time is not None:
                dt = (now - self.last_js_time).to_sec()
                if dt > 0.0001:
                    raw_l = (pos_l - self.last_left_pos) / dt
                    raw_r = (pos_r - self.last_right_pos) / dt
                    self.left_vel = 0.6 * self.left_vel + 0.4 * raw_l
                    self.right_vel = 0.6 * self.right_vel + 0.4 * raw_r
            self.last_left_pos = pos_l
            self.last_right_pos = pos_r
            self.last_js_time = now
        except ValueError:
            pass

    def spin(self):
        rate = rospy.Rate(50)
        duration = rospy.Duration(0.03)
        zero_time = rospy.Time(0)

        while not rospy.is_shutdown():
            now = rospy.Time.now()
            dt = (now - self.last_update).to_sec()
            self.last_update = now

            if (now - self.last_cmd_time).to_sec() > 0.5:
                self.target_v = 0.0
                self.target_w = 0.0

            half_sep = self.wheel_separation / 2.0
            target_left = (self.target_v - self.target_w * half_sep) / self.wheel_radius
            target_right = (self.target_v + self.target_w * half_sep) / self.wheel_radius

            # P + feedforward
            effort_left = self.k_ff * target_left + self.kp * (target_left - self.left_vel)
            effort_right = self.k_ff * target_right + self.kp * (target_right - self.right_vel)

            effort_left = max(-self.max_effort, min(self.max_effort, effort_left))
            effort_right = max(-self.max_effort, min(self.max_effort, effort_right))

            try:
                self.apply_effort("left_wheel_joint", effort_left, zero_time, duration)
                self.apply_effort("right_wheel_joint", effort_right, zero_time, duration)
            except rospy.ServiceException as e:
                rospy.logwarn_throttle(5, "apply_effort failed: %s", str(e))

            self.loop_count += 1
            if self.loop_count % 50 == 0:  # once per second
                rospy.loginfo("cmd=(%.2f,%.2f) tgt=(%.1f,%.1f) vel=(%.1f,%.1f) eff=(%.1f,%.1f)",
                               self.target_v, self.target_w,
                               target_left, target_right,
                               self.left_vel, self.right_vel,
                               effort_left, effort_right)

            rate.sleep()


if __name__ == "__main__":
    try:
        WheelController().spin()
    except rospy.ROSInterruptException:
        pass
