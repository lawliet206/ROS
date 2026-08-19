import math
import sys
import os
import threading
from collections import deque

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "src", "robot_bringup", "scripts"))

from scan_deskew import _deskew_ranges, ScanDeskew


# ============ 纯函数: _deskew_ranges ============
# 约定扫描参数: angle_min=-π, angle_increment=π/2, N=5 → bins: -π, -π/2, 0, π/2, π
ANGLE_MIN = -math.pi
ANGLE_INC = math.pi / 2
N = 5
RANGE_MIN, RANGE_MAX = 0.1, 10.0


def _call(ranges, intensities, total_yaw):
    return _deskew_ranges(ranges, intensities,
                          ANGLE_MIN, ANGLE_MIN + (N - 1) * ANGLE_INC, ANGLE_INC,
                          RANGE_MIN, RANGE_MAX, total_yaw)


def test_deskew_zero_yaw_identity():
    # 无旋转 → 每个点原地不动, 距离/强度保持
    ranges = [1.0, 2.0, 3.0, 4.0, 5.0]
    intensities = [10, 20, 30, 40, 50]
    new_ranges, new_intensities = _call(ranges, intensities, 0.0)
    assert new_ranges == ranges
    assert new_intensities == intensities


def test_deskew_rotation_shifts_and_wraps():
    # 正旋转 (total_yaw=0.3): 末点 (i=4, frac=1) 从 π 处被补偿到 -π 附近 → 环绕到 bin 0
    ranges = [float('inf'), float('inf'), 2.0, float('inf'), 3.0]
    intensities = [0, 0, 200, 0, 100]
    new_ranges, new_intensities = _call(ranges, intensities, 0.3)
    assert new_ranges[0] == 3.0 and new_intensities[0] == 100   # 环绕点
    assert new_ranges[2] == 2.0 and new_intensities[2] == 200   # 中点小幅偏移仍在原 bin
    assert new_ranges[1] == float('inf')
    assert new_ranges[3] == float('inf')
    assert new_ranges[4] == float('inf')


def test_deskew_collision_min_wins():
    # 两点落入同一 bin → 较近的距离胜出 (强度跟随)
    ranges = [5.0, float('inf'), float('inf'), float('inf'), 3.0]
    intensities = [50, 0, 0, 0, 100]
    new_ranges, new_intensities = _call(ranges, intensities, 0.3)
    assert new_ranges[0] == 3.0
    assert new_intensities[0] == 100


def test_deskew_invalid_ranges_filtered():
    # inf / 低于 range_min / 高于 range_max / NaN → 全部丢弃
    ranges = [float('inf'), 0.05, 2.0, 12.0, float('nan')]
    new_ranges, _ = _call(ranges, [0] * N, 0.0)
    assert new_ranges[2] == 2.0
    assert all(r == float('inf') for r in new_ranges[:2] + new_ranges[3:])


def test_deskew_intensities_missing():
    # 无强度输入 → 输出强度全 0, 距离仍正常重排
    ranges = [float('inf'), float('inf'), 2.0, float('inf'), 3.0]
    new_ranges, new_intensities = _call(ranges, [], 0.3)
    assert new_ranges[0] == 3.0
    assert new_intensities[0] == 0.0
    assert new_ranges[2] == 2.0


def test_deskew_intensities_shorter_than_ranges():
    # 强度数组短于距离数组 → 缺失处不写强度
    ranges = [float('inf'), float('inf'), 2.0, float('inf'), 3.0]
    intensities = [1, 2, 3]
    new_ranges, new_intensities = _call(ranges, intensities, 0.3)
    assert new_ranges[2] == 2.0 and new_intensities[2] == 3.0
    assert new_ranges[0] == 3.0 and new_intensities[0] == 0.0


def test_deskew_empty_scan():
    assert _deskew_ranges([], [], ANGLE_MIN, math.pi, ANGLE_INC,
                          RANGE_MIN, RANGE_MAX, 0.0) == ([], [])


# ============ _get_yaw_rate (绕过 __init__, 不触碰 ROS) ============
def _make_deskew():
    d = object.__new__(ScanDeskew)
    d.imu_buf = deque()
    d._lock = threading.Lock()
    return d


def test_yaw_rate_empty_buffer():
    assert _make_deskew()._get_yaw_rate(1.0) == 0.0


def test_yaw_rate_window_mean():
    d = _make_deskew()
    t = 100.0
    d.imu_buf.extend([(t - 0.02, 1.0), (t, 2.0), (t + 0.02, 3.0)])
    assert d._get_yaw_rate(t) == 2.0


def test_yaw_rate_nearest_fallback():
    # 窗口内不足 2 条 → 取最近值 (0.1s 内)
    d = _make_deskew()
    d.imu_buf.append((100.0 + 0.07, 5.0))
    assert d._get_yaw_rate(100.0) == 5.0


def test_yaw_rate_too_far_returns_zero():
    # 最近值也超过 0.1s → 视为无数据
    d = _make_deskew()
    d.imu_buf.append((100.0 + 0.2, 5.0))
    assert d._get_yaw_rate(100.0) == 0.0