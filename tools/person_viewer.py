#!/usr/bin/env python3
"""person_viewer.py — 显示小车摄像头图像 + YOLO 人体检测结果 (PC 端)

订阅 person_detector 发布的 /person_overlay (带检测框的压缩图),
在 PC 窗口实时显示.

用法 (配合 view_detection.sh 或单独运行):
  python3 ~/ROS/tools/person_viewer.py
"""
import cv2
import numpy as np
import rospy
from sensor_msgs.msg import CompressedImage

WINDOW = "小车摄像头 - YOLO 人体检测"
latest_frame = None  # 回调线程存帧, 主线程显示


def img_cb(msg):
    global latest_frame
    frame = cv2.imdecode(np.frombuffer(msg.data, np.uint8), cv2.IMREAD_COLOR)
    if frame is not None:
        latest_frame = frame


if __name__ == "__main__":
    rospy.init_node("person_viewer")
    cv2.namedWindow(WINDOW, cv2.WINDOW_NORMAL)
    rospy.Subscriber("/person_overlay", CompressedImage, img_cb, queue_size=1)
    rospy.loginfo("[Viewer] 订阅 /person_overlay, 关闭窗口按 q 或 Ctrl-C")

    # 主线程循环显示 (cv2.imshow 必须在主线程, 否则 Qt 后端显示失败)
    # 20Hz 显示足够流畅, 降低渲染负载避免 CPU 争抢导致卡顿
    rate = rospy.Rate(20)
    while not rospy.is_shutdown():
        if latest_frame is not None:
            cv2.imshow(WINDOW, latest_frame)
            cv2.waitKey(1)
        rate.sleep()
    cv2.destroyAllWindows()
