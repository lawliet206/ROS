import math
import sys
import os

import numpy as np

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "src", "robot_bringup", "scripts"))

from person_detector import angle_from_center, orient_frame, select_center_box


def test_angle_center_zero():
    # 框中心在图像正中央 → 0°
    assert angle_from_center(320, 640, 60.0) == 0.0


def test_angle_right_negative():
    # 框中心偏右 80px → 负角 (人偏右)
    a = angle_from_center(400, 640, 60.0)
    expected = 80 / 640 * math.radians(60.0)
    assert abs(a - (-expected)) < 1e-9


def test_angle_left_positive():
    # 框中心偏左 80px → 正角 (人偏左)
    a = angle_from_center(240, 640, 60.0)
    expected = 80 / 640 * math.radians(60.0)
    assert abs(a - expected) < 1e-9


def test_angle_full_fov_edge():
    # 图像最右边缘 = 人偏右 → -半FOV = -30°
    a = angle_from_center(640, 640, 60.0)
    assert abs(a - (-math.radians(30.0))) < 1e-9


def test_select_center_box_empty():
    assert select_center_box([], 640) is None


def test_select_center_box_picks_center():
    boxes = [(10, 0, 20, 100, 0.9), (300, 0, 340, 100, 0.8), (600, 0, 620, 100, 0.95)]
    sel = select_center_box(boxes, 640)
    assert sel is not None
    assert sel[0] == 320.0  # 中间框中心 x=320


def test_select_center_box_tie_break():
    # 两个等距框 → 取先出现的
    boxes = [(310, 0, 330, 100, 0.9), (330, 0, 350, 100, 0.8)]
    sel = select_center_box(boxes, 640)
    assert sel is not None
    assert sel[0] == 320.0


def test_orient_frame_rotates_180():
    frame = np.array([[1, 2, 3], [4, 5, 6]], dtype=np.uint8)
    assert orient_frame(frame, True).tolist() == [[6, 5, 4], [3, 2, 1]]


def test_orient_frame_can_be_disabled():
    frame = np.array([[1, 2], [3, 4]], dtype=np.uint8)
    assert orient_frame(frame, False) is frame
