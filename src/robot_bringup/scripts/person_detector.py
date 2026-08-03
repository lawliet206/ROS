#!/usr/bin/env python3
"""
person_detector.py — YOLOv8n 人体检测节点 (PC 端)
====================================================
订阅 J1900 推流的压缩图像, 检测人体, 选画面最中央的人,
发布相对机器人正前方的偏角 /person_angle 和可见标志 /person_visible.

用法:
  rosrun robot_bringup person_detector.py
"""
import math
import cv2
import numpy as np
import rospy
from sensor_msgs.msg import CompressedImage
from std_msgs.msg import Float32, Bool
from ultralytics import YOLO


def angle_from_center(center_x, image_width, hfov_deg):
    """图像中心偏移 → 偏角 (弧度, 左正右负)

    Args:
        center_x: 框中心 x 像素坐标
        image_width: 图像宽度 (像素)
        hfov_deg: 摄像头水平视场角 (度)
    Returns:
        弧度; 正=人偏左, 负=人偏右
    """
    hfov_rad = math.radians(hfov_deg)
    offset = (image_width / 2.0 - center_x) / image_width
    return offset * hfov_rad


def select_center_box(boxes, image_width):
    """从检测框列表选画面最中央的人

    Args:
        boxes: [(x1, y1, x2, y2, conf), ...] (像素坐标, 可含多框)
        image_width: 图像宽度 (像素)
    Returns:
        (cx, x1, y1, x2, y2) 或 None (无框时)
    """
    if not boxes:
        return None
    center = image_width / 2.0
    best = min(boxes, key=lambda b: abs((b[0] + b[2]) / 2.0 - center))
    cx = (best[0] + best[2]) / 2.0
    return (cx, best[0], best[1], best[2], best[3])


class PersonDetector:
    def __init__(self):
        self.conf = rospy.get_param("~conf", 0.4)
        self.hfov = rospy.get_param("~hfov", 60.0)
        self.image_topic = rospy.get_param("~image_topic", "/image_raw/compressed")
        self.publish_overlay = rospy.get_param("~publish_overlay", True)
        # 显式 CPU 推理: 本机 CUDA 版 PyTorch 在无可用 GPU 时推理会直接崩溃
        self.device = rospy.get_param("~device", "cpu")

        rospy.loginfo("[Detect] YOLOv8n 加载中 (首次运行自动下载权重)...")
        self.model = YOLO("yolov8n.pt")

        self.angle_pub = rospy.Publisher("/person_angle", Float32, queue_size=1)
        self.visible_pub = rospy.Publisher("/person_visible", Bool, queue_size=1)
        if self.publish_overlay:
            self.overlay_pub = rospy.Publisher("/person_overlay", CompressedImage, queue_size=1)

        rospy.Subscriber(self.image_topic, CompressedImage, self.img_callback, queue_size=1)
        rospy.loginfo("[Detect] hfov=%.1f° conf=%.2f 订阅 %s", self.hfov, self.conf, self.image_topic)

    def img_callback(self, msg):
        try:
            frame = cv2.imdecode(np.frombuffer(msg.data, np.uint8), cv2.IMREAD_COLOR)
        except Exception:
            rospy.logwarn_throttle(5, "[Detect] 图像解码失败")
            return
        if frame is None:
            return
        h, w = frame.shape[:2]

        results = self.model(frame, conf=self.conf, classes=[0], verbose=False, device=self.device)
        boxes = []
        if results and results[0].boxes is not None:
            xyxy = results[0].boxes.xyxy.cpu().tolist()
            confs = results[0].boxes.conf.cpu().tolist()
            for (x1, y1, x2, y2), c in zip(xyxy, confs):
                boxes.append((x1, y1, x2, y2, c))

        sel = select_center_box(boxes, w)
        if sel is not None:
            cx, x1, y1, x2, y2 = sel
            angle = angle_from_center(cx, w, self.hfov)
            self.angle_pub.publish(Float32(data=angle))
            self.visible_pub.publish(Bool(data=True))
            rospy.logdebug("[Detect] person cx=%.0f angle=%.1f°", cx, math.degrees(angle))
            if self.publish_overlay:
                self._publish_overlay(frame, boxes, sel)
        else:
            self.visible_pub.publish(Bool(data=False))
            if self.publish_overlay:
                self._publish_overlay(frame, boxes, None)

    def _publish_overlay(self, frame, boxes, sel):
        for (x1, y1, x2, y2, _c) in boxes:
            cv2.rectangle(frame, (int(x1), int(y1)), (int(x2), int(y2)), (0, 255, 0), 2)
        if sel is not None:
            cx = int(sel[0])
            cv2.line(frame, (cx, 0), (cx, frame.shape[0]), (0, 0, 255), 2)
        # 图像中心十字线 (蓝色)
        cv2.line(frame, (frame.shape[1] // 2, 0), (frame.shape[1] // 2, frame.shape[0]), (255, 0, 0), 1)
        ok, enc = cv2.imencode(".jpg", frame, [cv2.IMWRITE_JPEG_QUALITY, 80])
        if ok:
            self.overlay_pub.publish(CompressedImage(format="jpeg", data=enc.tobytes()))


if __name__ == "__main__":
    rospy.init_node("person_detector")
    try:
        PersonDetector()
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
