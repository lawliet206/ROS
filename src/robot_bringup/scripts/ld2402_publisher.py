#!/usr/bin/env python3
"""
ld2402_publisher.py — LD2402 毫米波雷达人体存在发布
=====================================================
只发布人体存在状态 (Bool), 不发布距离 (不可靠).

话题:
  /radar_presence   Bool  是否检测到人体

用法:
  rosrun robot_bringup ld2402_publisher.py _port:=/dev/ttyUSB2
"""
import rospy
import serial
from std_msgs.msg import Bool


class LD2402Publisher:
    def __init__(self):
        port = rospy.get_param("~port", "/dev/ttyUSB2")
        baud = rospy.get_param("~baud", 115200)
        self.presence_timeout = rospy.get_param("~presence_timeout", 2.0)

        self.presence = False
        self.last_detect_time = 0.0

        self.pub = rospy.Publisher("/radar_presence", Bool, queue_size=10)

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

    def read_serial(self):
        try:
            raw = self.ser.read(256)
        except serial.SerialException:
            return
        if not raw:
            return
        self._buf += raw

        while b"\n" in self._buf:
            line, self._buf = self._buf.split(b"\n", 1)
            try:
                text = line.decode("ascii", errors="ignore").strip()
                if text.startswith("distance:"):
                    self.presence = True
                    self.last_detect_time = rospy.Time.now().to_sec()
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

            now = rospy.Time.now().to_sec()
            if self.presence and (now - self.last_detect_time) > self.presence_timeout:
                self.presence = False

            self.pub.publish(Bool(self.presence))
            rate.sleep()


if __name__ == "__main__":
    rospy.init_node("ld2402_publisher")
    try:
        LD2402Publisher().spin()
    except rospy.ROSInterruptException:
        pass
