#!/usr/bin/env python3
"""
radar_follower.py — LD2402 毫米波雷达人体跟随
=================================================
实物: LD2402 TX → USB-UART RX (插入 J1900 USB), 串口模式
仿真: rosrun ... radar_follower.py _sim_mode:=true, 话题模式

工作逻辑:
  跟随模式 — 读到人体距离, 维持设定距离
  搜索模式 — 丢失目标后停车等待, 找到自动恢复
"""
import rospy
import serial
import struct
from geometry_msgs.msg import Twist
from std_msgs.msg import Float64, Bool


RADAR_FRAME_HEADER = b'\xF4\xF3\xF2\xF1'
RADAR_FRAME_TYPE_DISTANCE = 0x83

ST_IDLE, ST_HEADER, ST_FRAME = 0, 1, 2
MODE_FOLLOW, MODE_SEARCH = 0, 1


class RadarFollower:

    def __init__(self):
        self.sim_mode = rospy.get_param("~sim_mode", False)

        port = rospy.get_param("~port", "/dev/ttyUSB2")
        baud = rospy.get_param("~baud", 115200)
        self.target_dist = rospy.get_param("~target_dist", 1.0)
        self.max_linear = rospy.get_param("~max_linear", 0.5)
        self.kp_linear = rospy.get_param("~kp_linear", 0.4)
        self.follow_deadzone = rospy.get_param("~deadzone", 0.2)
        self.timeout = rospy.get_param("~timeout", 2.0)
        self.dist_filter_alpha = rospy.get_param("~dist_filter_alpha", 0.3)

        self.distance = -1.0
        self.distance_filtered = -1.0
        self.presence = False
        self.last_detect_time = 0.0
        self.mode = MODE_SEARCH

        self.cmd_pub = rospy.Publisher("/cmd_vel", Twist, queue_size=10)
        rospy.on_shutdown(self.stop)

        if self.sim_mode:
            rospy.loginfo("[Radar] 仿真模式 — 订阅 /radar_distance")
            self.ser = None
            rospy.Subscriber("/radar_distance", Float64, self._sim_dist_cb)
            rospy.Subscriber("/radar_presence", Bool, self._sim_presence_cb)
        else:
            try:
                self.ser = serial.Serial(port, baud, timeout=0.01)
                rospy.loginfo("[Radar] 已打开 %s @ %d", port, baud)
            except serial.SerialException as e:
                rospy.logfatal("[Radar] 无法打开 %s: %s", port, e)
                raise

            self._buf = b""
            self._state = ST_IDLE
            self._frame = bytearray()

    def stop(self):
        self.cmd_pub.publish(Twist())
        if self.sim_mode:
            return
        if hasattr(self, 'ser') and self.ser and self.ser.is_open:
            self.ser.close()
        rospy.loginfo("[Radar] 已停止")

    def read_serial(self):
        try:
            raw = self.ser.read(256)
        except serial.SerialException:
            raw = b""
        if not raw:
            return
        self._buf += raw

        while self._buf:
            if self._state == ST_IDLE:
                idx = self._buf.find(RADAR_FRAME_HEADER)
                if idx < 0:
                    if len(self._buf) > 256:
                        self._buf = self._buf[-4:]
                    break
                if idx > 0:
                    self._parse_ascii(self._buf[:idx])
                self._buf = self._buf[idx + 4:]
                self._frame = bytearray(RADAR_FRAME_HEADER)
                self._state = ST_HEADER

            elif self._state == ST_HEADER:
                if len(self._buf) < 1:
                    break
                frame_type = self._buf[0]
                self._frame.append(frame_type)
                self._buf = self._buf[1:]
                self._state = ST_FRAME if frame_type == RADAR_FRAME_TYPE_DISTANCE else ST_IDLE

            elif self._state == ST_FRAME:
                self._frame.append(self._buf[0])
                self._buf = self._buf[1:]
                if len(self._frame) >= 24:
                    self._parse_distance_frame(bytes(self._frame))
                    self._state = ST_IDLE

        if self._buf and len(self._buf) > 512:
            self._buf = self._buf[-128:]

    def _parse_ascii(self, text):
        try:
            s = text.decode("ascii", errors="ignore").strip()
        except Exception:
            return
        if "distance:" in s:
            try:
                val = float(s.split("distance:")[-1].split()[0])
                self._update_target(val)
            except (ValueError, IndexError):
                pass
        elif s == "OFF":
            self.presence = False

    def _parse_distance_frame(self, frame):
        if len(frame) < 14:
            return
        status = frame[8]
        vals = []
        for i in range(12, len(frame) - 3, 4):
            raw = struct.unpack_from("<I", frame, i)[0]
            if raw > 0:
                vals.append(raw * 0.1)
        if vals:
            self._update_target(min(vals))
        elif status == 0:
            self.presence = False

    def _update_target(self, dist_cm):
        dist_m = dist_cm / 100.0
        self.distance = dist_m
        if self.distance_filtered < 0:
            self.distance_filtered = dist_m
        else:
            self.distance_filtered = (self.dist_filter_alpha * dist_m +
                                      (1.0 - self.dist_filter_alpha) * self.distance_filtered)
        self.presence = True
        self.last_detect_time = rospy.Time.now().to_sec()

    def _sim_dist_cb(self, msg):
        """仿真模式: /radar_distance → 距离更新 (m → cm)"""
        if msg.data > 0:
            self._update_target(msg.data * 100.0)

    def _sim_presence_cb(self, msg):
        """仿真模式: /radar_presence → 目标状态"""
        self.presence = msg.data

    def follow(self):
        if not self.sim_mode:
            self.read_serial()
        now = rospy.Time.now().to_sec()
        has_target = self.presence and (now - self.last_detect_time) <= self.timeout

        if has_target:
            if self.mode == MODE_SEARCH:
                rospy.loginfo("[Radar] 重新找到目标, 切换到跟随模式")
                self.mode = MODE_FOLLOW

            cmd = Twist()
            err = self.distance_filtered - self.target_dist
            if abs(err) > self.follow_deadzone:
                cmd.linear.x = self.kp_linear * err
                cmd.linear.x = max(-self.max_linear,
                                   min(self.max_linear, cmd.linear.x))
            self.cmd_pub.publish(cmd)
            rospy.loginfo_throttle(
                1, "[Radar] 跟 dist=%.2fm err=%.2f v=%.2f",
                self.distance_filtered, err, cmd.linear.x)
        else:
            if self.mode == MODE_FOLLOW:
                rospy.loginfo("[Radar] 丢失目标, 停车等待")
                self.mode = MODE_SEARCH

            self.cmd_pub.publish(Twist())


if __name__ == "__main__":
    rospy.init_node("radar_follower")
    try:
        node = RadarFollower()
        rate = rospy.Rate(20)
        while not rospy.is_shutdown():
            node.follow()
            rate.sleep()
    except rospy.ROSInterruptException:
        pass
    except serial.SerialException as e:
        rospy.logfatal("[Radar] 串口错误: %s", e)
