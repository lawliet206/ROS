import math
import sys
import os

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "src", "robot_bringup", "scripts"))

from vision_follower import FollowStateMachine, find_nearest_human_dist, compute_cmd


def test_compute_cmd_radar_lost_no_forward():
    # 雷达丢失 (dist=None): 只转不前进 (规格 §6.1)
    cmd = compute_cmd(0.2, None, 1.0, 0.4, 0.5, 0.5, 0.8)
    assert cmd.linear.x == 0.0
    assert cmd.angular.z > 0.0


def test_compute_cmd_far_target_forward():
    # 距离远: 前进
    cmd = compute_cmd(0.0, 3.0, 1.0, 0.4, 0.5, 0.5, 0.8)
    assert cmd.linear.x > 0.0
    assert cmd.angular.z == 0.0


def test_compute_cmd_angle_deadzone():
    # 角度小于死区: 不转, 距离误差大则前进
    cmd = compute_cmd(0.05, 3.0, 1.0, 0.4, 0.5, 0.5, 0.8)
    assert cmd.angular.z == 0.0
    assert cmd.linear.x > 0.0


def test_compute_cmd_angle_priority():
    # 角度大: 只转不前进 (先对准再靠近)
    cmd = compute_cmd(0.5, 3.0, 1.0, 0.4, 0.5, 0.5, 0.8)
    assert cmd.angular.z > 0.0
    assert cmd.linear.x == 0.0


class FakeScan:
    def __init__(self, ranges, angle_min=-1.57, angle_increment=0.01):
        self.ranges = ranges
        self.angle_min = angle_min
        self.angle_increment = angle_increment


def _make_scan():
    """默认: 前方远处墙(6m, 超出max_dist), 角度跨度 -1.57..1.57"""
    n = int((1.57 - (-1.57)) / 0.01) + 1
    ranges = [6.0] * n
    return FakeScan(ranges), n


def test_find_nearest_human():
    scan, n = _make_scan()
    # 人在正前方 1.0m, 占 ±0.15 rad (宽度约 0.30m)
    for k in range(31):
        a = -0.15 + 0.01 * k
        idx = int((a - scan.angle_min) / scan.angle_increment)
        scan.ranges[idx] = 1.0
    dist = find_nearest_human_dist(scan, 0.3, 5.0, 0.15, 5, 0.1, 0.55)
    assert dist is not None
    assert abs(dist - 1.0) < 0.05


def test_wide_cluster_rejected():
    # 一堵宽墙在 1m 处 (±0.6 rad → 宽 ~1.13m) → 超人体宽度 → None
    scan, n = _make_scan()
    for k in range(121):
        a = -0.6 + 0.01 * k
        idx = int((a - scan.angle_min) / scan.angle_increment)
        scan.ranges[idx] = 1.0
    dist = find_nearest_human_dist(scan, 0.3, 5.0, 0.15, 5, 0.1, 0.55)
    assert dist is None


def test_no_human_in_range():
    scan, n = _make_scan()
    # 全是远处的墙 → None
    dist = find_nearest_human_dist(scan, 0.3, 5.0, 0.15, 5, 0.1, 0.55)
    assert dist is None


def test_sm_follow_with_visible():
    sm = FollowStateMachine(lost_frames=5)
    state, ang = sm.update(True, 0.1)
    assert state == FollowStateMachine.FOLLOW
    assert ang == 0.0


def test_sm_search_after_lost_frames():
    sm = FollowStateMachine(lost_frames=3)
    for _ in range(2):
        sm.update(False, 0.1)
    assert sm.state == FollowStateMachine.FOLLOW
    state, _ = sm.update(False, 0.1)
    assert state == FollowStateMachine.SEARCH
    assert sm.sweep_angle == 0.0
    assert sm.sweep_dir == 1.0


def test_sm_search_sweeps_and_reverses():
    sm = FollowStateMachine(lost_frames=1, sweep_deg=60.0, search_angular=1.0)
    sm.update(False, 0.1)  # 丢失 1 帧 → SEARCH
    # 向左扫: 1 rad/s × 1.1s = 1.1 rad ≥ 60°(1.047) → 反向
    state, ang = sm.update(False, 1.1)
    assert state == FollowStateMachine.SEARCH
    assert sm.sweep_dir == -1.0
    # 向右回扫 1.1 rad → 回到 0
    state, ang = sm.update(False, 1.1)
    assert state == FollowStateMachine.SEARCH
    assert sm.sweep_angle == 0.0
    # 再向右 1.1 rad → 达 -60° → STOP
    state, ang = sm.update(False, 1.1)
    assert state == FollowStateMachine.STOP
    assert ang == 0.0


def test_sm_recovers_to_follow():
    sm = FollowStateMachine(lost_frames=1, sweep_deg=60.0, search_angular=0.6)
    sm.update(False, 0.1)  # → SEARCH
    sm.update(False, 0.5)  # 扫了一会儿
    state, ang = sm.update(True, 0.1)  # 发现人 → 回 FOLLOW
    assert state == FollowStateMachine.FOLLOW
    assert ang == 0.0
    assert sm.sweep_angle == 0.0


def test_sm_stop_stays_stopped():
    sm = FollowStateMachine(lost_frames=1, sweep_deg=60.0, search_angular=1.0)
    sm.update(False, 0.1)  # → SEARCH
    sm.update(False, 1.1)  # → 反向
    sm.update(False, 1.1)  # → 回到 0
    sm.update(False, 1.1)  # → 达 -60° → STOP
    assert sm.state == FollowStateMachine.STOP
    state, ang = sm.update(False, 0.1)
    assert state == FollowStateMachine.STOP
    assert ang == 0.0
