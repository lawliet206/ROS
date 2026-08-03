# 视觉+雷达融合人体跟随 实施计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 在现有激光人体跟随基础上新增视觉融合：PC 端 YOLOv8n 检测人并给出精确偏角，雷达给出距离，融合控制器发布 /cmd_vel 驱动小车跟随，视觉丢失时摇摆 ±60° 搜索、找不到停车。

**Architecture:** J1900 车载电脑用 usb_cam 采集摄像头并以 JPEG 压缩推流；PC（ROS Master）运行两个新 Python 节点——`person_detector.py`（YOLOv8n 检测 → 画面最中央的人 → 偏角）和 `vision_follower.py`（融合控制器：视觉角度 + 雷达距离 → /cmd_vel，3 态状态机 跟随/搜索/停止）。

**Tech Stack:** ROS Noetic / rospy / sensor_msgs / geometry_msgs / std_msgs / ultralytics (YOLOv8n) / OpenCV / pytest

## Global Constraints

- Python 3.8（ROS Noetic 自带），脚本放 `src/robot_bringup/scripts/`，chmod +x
- 角度约定：弧度，**左正右负**（与现有 `laser_follower.py` 的 `atan2(cy, cx)` 一致）
- 新节点参数全部走 rosparam，默认值与设计规格一致（见各任务）
- 检测只保留 YOLO class=0 (person)；多框选画面最中央
- 不引入 RTSP/GStreamer；不引入 ReID/卡尔曼多目标跟踪
- 代码风格与现有脚本一致：中文注释、`rospy.loginfo` 前缀 `[Detect]`/`[Follow]`
- 测试用 pytest（`pip3 install --user pytest` 已需安装），测试文件放 `tests/`
- 提交信息用中文，遵循仓库风格（`feat:`/`fix:` 前缀）

---

### Task 1: person_detector.py — YOLOv8n 人体检测节点

**Files:**
- Create: `src/robot_bringup/scripts/person_detector.py`
- Test: `tests/test_person_detector.py`

**Interfaces:**
- Consumes: `/image_raw/compressed` (`sensor_msgs/CompressedImage`, J1900 usb_cam 推流)
- Produces:
  - `/person_angle` (`std_msgs/Float32`) — 人相对正前方偏角，弧度，左正
  - `/person_visible` (`std_msgs/Bool`) — 当前帧是否检测到人
  - `/person_overlay` (`sensor_msgs/CompressedImage`) — 可视化（画框+十字线）
  - 纯函数 `angle_from_center(center_x, image_width, hfov_deg) -> float`（弧度，左正）
  - 纯函数 `select_center_box(boxes, image_width) -> tuple|None`（返回 (cx, x1, y1, x2, y2)）

- [ ] **Step 1: 安装 pytest**

Run: `pip3 install --user pytest`
Expected: 安装成功，无报错

- [ ] **Step 2: 写失败测试**

Create `tests/test_person_detector.py`:

```python
import math
import sys
import os

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "src", "robot_bringup", "scripts"))

from person_detector import angle_from_center, select_center_box


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
```

- [ ] **Step 3: 运行测试确认失败**

Run: `python3 -m pytest tests/test_person_detector.py -v`
Expected: FAIL — `ModuleNotFoundError: No module named 'person_detector'`

- [ ] **Step 4: 实现 person_detector.py**

Create `src/robot_bringup/scripts/person_detector.py`:

```python
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

        results = self.model(frame, conf=self.conf, classes=[0], verbose=False)
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
```

- [ ] **Step 5: chmod +x 并运行测试确认通过**

Run: `chmod +x src/robot_bringup/scripts/person_detector.py && python3 -m pytest tests/test_person_detector.py -v`
Expected: 7 tests PASS

- [ ] **Step 6: 语法检查**

Run: `python3 -m py_compile src/robot_bringup/scripts/person_detector.py`
Expected: 无输出 (退出码 0)

- [ ] **Step 7: 提交**

```bash
git add src/robot_bringup/scripts/person_detector.py tests/test_person_detector.py
git commit -m "feat: person_detector YOLOv8n人体检测节点(最中央人选帧+角度换算)"
```

---

### Task 2: vision_follower.py — 视觉+雷达融合控制器

**Files:**
- Create: `src/robot_bringup/scripts/vision_follower.py`
- Test: `tests/test_vision_follower.py`

**Interfaces:**
- Consumes:
  - `/scan` (`sensor_msgs/LaserScan`) — 雷达
  - `/person_angle` (`std_msgs/Float32`) — 视觉偏角 (弧度, 左正)
  - `/person_visible` (`std_msgs/Bool`) — 视觉可见标志
- Produces: `/cmd_vel` (`geometry_msgs/Twist`)
- 纯函数 `find_nearest_human_dist(scan, min_dist, max_dist, cluster_tol, min_points, min_body_width, max_body_width) -> float|None`（最近人体簇距离）
- 纯函数 `_cluster_width(cluster) -> float`（簇物理宽度, 内部用）
- 类 `FollowStateMachine(lost_frames=5, sweep_deg=60.0, search_angular=0.6)`，方法 `update(person_visible, dt) -> (state, angular_cmd)`，常量 `FOLLOW=0 / SEARCH=1 / STOP=2`

- [ ] **Step 1: 写失败测试**

Create `tests/test_vision_follower.py`:

```python
import math
import sys
import os

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "src", "robot_bringup", "scripts"))

from vision_follower import FollowStateMachine, find_nearest_human_dist


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
```

- [ ] **Step 2: 运行测试确认失败**

Run: `python3 -m pytest tests/test_vision_follower.py -v`
Expected: FAIL — `ModuleNotFoundError: No module named 'vision_follower'`

- [ ] **Step 3: 实现 vision_follower.py**

Create `src/robot_bringup/scripts/vision_follower.py`:

```python
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
```

- [ ] **Step 4: chmod +x 并运行测试确认通过**

Run: `chmod +x src/robot_bringup/scripts/vision_follower.py && python3 -m pytest tests/test_vision_follower.py -v`
Expected: 8 tests PASS

- [ ] **Step 5: 语法检查**

Run: `python3 -m py_compile src/robot_bringup/scripts/vision_follower.py`
Expected: 无输出 (退出码 0)

- [ ] **Step 6: 提交**

```bash
git add src/robot_bringup/scripts/vision_follower.py tests/test_vision_follower.py
git commit -m "feat: vision_follower 视觉+雷达融合控制器(3态状态机+摇摆搜索)"
```

---

### Task 3: follow_vision.launch + 构建配置 + SETUP 文档

**Files:**
- Create: `src/robot_bringup/launch/follow_vision.launch`
- Modify: `src/robot_bringup/CMakeLists.txt`（catkin_install_python 加 2 个新脚本）
- Modify: `SETUP.md`（新增视觉跟随部署章节）

**Interfaces:**
- Produces: `follow_vision.launch`（PC 端一键启动 person_detector + vision_follower，参数透传 follow_dist/max_speed/hfov）

- [ ] **Step 1: 创建 follow_vision.launch**

Create `src/robot_bringup/launch/follow_vision.launch`:

```xml
<!--
  follow_vision.launch — 视觉+雷达融合人体跟随 (PC 端一键启动)
  =============================================================
  前置: J1900 上已启动摄像头推流 (usb_cam + republish) 和激光雷达驱动

  使用:
    roslaunch robot_bringup follow_vision.launch
    roslaunch robot_bringup follow_vision.launch follow_dist:=1.2
-->
<launch>

  <arg name="follow_dist" default="1.0" />
  <arg name="max_speed"   default="0.5" />
  <arg name="hfov"        default="60.0" />

  <!-- 人体检测节点 (YOLOv8n) -->
  <node name="person_detector" pkg="robot_bringup" type="person_detector.py"
        output="screen">
    <param name="hfov" value="$(arg hfov)" />
    <param name="conf" value="0.4" />
  </node>

  <!-- 融合控制器 -->
  <node name="follower" pkg="robot_bringup" type="vision_follower.py"
        output="screen">
    <param name="target_dist"     value="$(arg follow_dist)" />
    <param name="max_linear"      value="$(arg max_speed)" />
    <param name="max_angular"     value="0.8" />
    <param name="lost_frames"     value="5" />
    <param name="search_sweep"    value="60.0" />
    <param name="search_angular"  value="0.6" />
  </node>

</launch>
```

- [ ] **Step 2: 更新 CMakeLists.txt**

Modify `src/robot_bringup/CMakeLists.txt` — 在 `catkin_install_python(PROGRAMS` 块中加入两行:

```cmake
catkin_install_python(PROGRAMS
  scripts/s9_lidar_driver.py
  scripts/laser_follower.py
  scripts/scan_deskew.py
  scripts/send_goals.py
  scripts/person_detector.py
  scripts/vision_follower.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
```

- [ ] **Step 3: 更新 SETUP.md — 追加视觉跟随部署章节**

Append to `SETUP.md`:

```markdown
## 视觉+雷达融合人体跟随 (可选)

架构: J1900 usb_cam 推流 → PC YOLOv8n 检测 (person_detector.py) → 融合控制 (vision_follower.py)

### J1900 端 (一次性安装)
```bash
sudo apt install ros-noetic-usb-cam ros-noetic-image-transport
# 摄像头推流 (先确认设备号: ls /dev/video*)
rosrun usb_cam usb_cam_node _video_device:=/dev/video0 _image_width:=640 _image_height:=480 _pixel_format:=yuyv
rosrun image_transport republish raw in:=/usb_cam/image_raw compressed out:=/image_raw
```

### PC 端 (一次性安装)
```bash
pip3 install --user ultralytics pytest
```

### 启动
```bash
# J1900: 摄像头推流 + 雷达驱动 (见上)
# PC:
roslaunch robot_bringup follow_vision.launch
# 调试: rqt_image_view /person_overlay 查看检测框
```

### 参数 (可选覆盖)
```bash
roslaunch robot_bringup follow_vision.launch follow_dist:=1.2 hfov:=70
```
```

- [ ] **Step 4: 验证 launch XML 语法**

Run: `python3 -c "import xml.dom.minidom; xml.dom.minidom.parse('src/robot_bringup/launch/follow_vision.launch'); print('XML OK')"`
Expected: `XML OK`

- [ ] **Step 5: 提交**

```bash
git add src/robot_bringup/launch/follow_vision.launch src/robot_bringup/CMakeLists.txt SETUP.md
git commit -m "feat: follow_vision.launch 一键启动 + CMakeLists/SETUP 部署配置"
```

---

### Task 4: 集成验证

**Files:** 无新增

**Interfaces:** 无 — 验证整个功能链路可编译、可运行

- [ ] **Step 1: 全部测试回归**

Run: `python3 -m pytest tests/ -v`
Expected: 15 tests PASS (7 detector + 8 follower)

- [ ] **Step 2: catkin 编译验证**

Run: `cd build && cmake .. -DCATKIN_WHITELIST_PACKAGES="robot_bringup" && make 2>&1 | tail -5`
Expected: 编译成功无错误（`catkin_install_python` 安装 6 个脚本）

- [ ] **Step 3: 静态检查全部脚本**

Run: `python3 -m py_compile src/robot_bringup/scripts/person_detector.py src/robot_bringup/scripts/vision_follower.py`
Expected: 无输出 (退出码 0)

- [ ] **Step 4: 提交（如有残留变更）**

Run: `git status`
Expected: 干净工作区；若有未提交变更则 `git add -A && git commit -m "chore: 视觉跟随集成验证"`
