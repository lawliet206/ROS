#!/usr/bin/env python3
"""
ld2402_publisher.py — LD2402 毫米波雷达数据发布
================================================
从串口读取 LD2402 原始数据, 解析后发布距离和存在状态。
供 fusion_follower.py 等节点使用。

话题:
  /radar_distance   Float64  最近人体距离 (m), 无目标时为 -1
  /radar_presence   Bool     是否检测到人体

用法:
  rosrun robot_bringup ld2402_publisher.py _port:=/dev/ttyUSB2
"""
import rospy
import serial
from std_msgs.msg import Float64, Bool


class LD2402Publisher:
    def __init__(self):
        port = rospy.get_param("~port", "/dev/ttyUSB2")
        baud = rospy.get_param("~baud", 115200)
        self.dist_filter_alpha = rospy.get_param("~dist_filter_alpha", 0.3)
        self.presence_timeout = rospy.get_param("~presence_timeout", 2.0)

        self.distance_filtered = -1.0
        self.presence = False
        self.last_detect_time = 0.0

        self.dist_pub = rospy.Publisher("/radar_distance", Float64, queue_size=10)
        self.presence_pub = rospy.Publisher("/radar_presence", Bool, queue_size=10)

        try:
            self.ser = serial.Serial(port, baud, timeout=0.01)
            rospy.loginfo("[LD2402] %s @ %d", port, baud)
        except serial.SerialException as e:
            rospy.logfatal("[LD2402] 无法打开 %s: %s", port, e)
            raise

        self._buf = b""

        rospy.on_shutdown(self._stop)

    def _stop(self):
        if self.ser and self.ser.is_open:
            self.ser.close()

    def _update_target(self, dist_cm):
        dist_m = dist_cm / 100.0
        if self.distance_filtered < 0:
            self.distance_filtered = dist_m
        else:
            self.distance_filtered = (self.dist_filter_alpha * dist_m +
                                      (1.0 - self.dist_filter_alpha) * self.distance_filtered)
        self.presence = True
        self.last_detect_time = rospy.Time.now().to_sec()

    def read_serial(self):
        try:
            raw = self.ser.read(256)
        except serial.SerialException:
            return
        if not raw:
            return
        self._buf += raw

        # 按行解析 ASCII 数据
        while b"\n" in self._buf:
            line, self._buf = self._buf.split(b"\n", 1)
            try:
                text = line.decode("ascii", errors="ignore").strip()
                if text.startswith("distance:"):
                    try:
                        val = float(text.split("distance:")[-1].strip())
                        self._update_target(val)
                    except (ValueError, IndexError):
                        pass
                elif text == "OFF":
                    self.presence = False
            except Exception:
                pass

        if len(self._buf) > 512:
            self._buf = self._buf[-128:]

    def spin(self):
        rate = rospy.Rate(20)
        while not rospy.is_shutdown():
            self.read_serial()

            # 超时清除 presence
            now = rospy.Time.now().to_sec()
            if self.presence and (now - self.last_detect_time) > self.presence_timeout:
                self.presence = False
                self.distance_filtered = -1.0

            dist = self.distance_filtered if self.presence else -1.0
            self.dist_pub.publish(Float64(dist))
            self.presence_pub.publish(Bool(self.presence))

            rospy.loginfo_throttle(1.0,
                "[LD2402] dist=%.2fm presence=%s", dist, self.presence)
            rate.sleep()


if __name__ == "__main__":
    rospy.init_node("ld2402_publisher")
    try:
        LD2402Publisher().spin()
    except rospy.ROSInterruptException:
        pass
