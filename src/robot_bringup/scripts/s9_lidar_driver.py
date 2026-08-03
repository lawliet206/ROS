#!/usr/bin/env python3
"""
S9-FSRD-V1.0 RX 激光雷达 ROS 驱动节点 v5
==========================================
协议: 115200 8N1, AA 55 帧头
帧格式: AA 55 [ct:1B] [count:1B] [firstAngle:2B] [lastAngle:2B] [cs:2B] [nodes: count×3B]
每个点: quality(1B) + dist(2B LE, mm)
角度单位: 1/64 度 (firstAngle/64 = 角度°)

跨零检测: firstAngle 从 >260° 跳回 <100° 时触发一圈完整扫描发布
超时兜底: 0.5s 没收到跨零也发布, 保证数据不断流

使用:
  rosrun robot_bringup s9_lidar_driver.py _port:=/dev/ttyUSB0
"""
import rospy
import serial
import math
import time
import struct

from std_msgs.msg import Header
from sensor_msgs.msg import LaserScan

FRAME_HEADER = b'\xAA\x55'
NUM_BINS = 360
ANGLE_64 = 64.0        # 1 degree = 64 units
CROSS_HI = 260          # 跨零高阈值 (度)
CROSS_LO = 100          # 跨零低阈值 (度)


def _extract_frames(buf):
    """从串口字节流提取完整帧 (定长解析, cnt 驱动).

    不依赖搜索下一个 AA55 定帧界, 因此载荷内偶然出现的 AA55 不会截断合法帧.
    帧尾边界验证: 切出帧后检查下一字节为下一帧头 AA55 (或 0x6D 尾标记 + AA55),
    防止丢字节时用下一帧首字节补足损坏帧 (A 缺字节 + B 场景).

    Returns:
        (frames, 剩余buf): frames 为完整帧列表 (bytes)
    """
    frames = []
    for _ in range(200):
        p = buf.find(FRAME_HEADER)
        if p < 0:
            if len(buf) > 3:
                buf = buf[-3:]          # 保留尾部防跨边界
            break
        if p > 0:
            buf = buf[p:]               # 对齐到帧头

        if len(buf) < 12:
            break                        # 头部不足, 等更多数据

        cnt = buf[3]
        if cnt == 0 or cnt > 80:
            buf = buf[2:]                # 非法 cnt: 跳过帧头继续搜索
            continue

        expect = 10 + cnt * 3
        if len(buf) < expect:
            break                        # 帧体不足, 等更多数据

        frame = bytes(buf[:expect])
        # ---- 帧尾边界验证 ----
        # 正常帧尾后 = 下一帧头 AA55, 或 0x6D 尾标记(部分帧) + AA55, 或流末尾.
        # 丢字节场景 (A 缺 1 字节后接 B): 损坏帧 A[:-1]+B[0] 的边界字节是 B[1],
        # 不是 AA55 → 拒绝本帧并重同步, 防止吞掉 B.
        if (len(buf) >= expect + 2 and buf[expect] == 0xAA
                and buf[expect + 1] == 0x55):
            frames.append(frame)
            buf = buf[expect:]
            continue
        if (len(buf) >= expect + 3 and buf[expect] == 0x6D
                and buf[expect + 1] == 0xAA and buf[expect + 2] == 0x55):
            frames.append(frame)
            buf = buf[expect + 1:]       # 跳过 0x6D 尾标记
            continue
        if len(buf) == expect:
            # 帧恰好到流末尾, 无边界字节可验证 → 信任 cnt 输出.
            # 丢字节 + 下一帧恰好只到 1 字节的极罕见组合会误输出 1 字节污染帧,
            # 但串口批量读取 (read 256B) 下整帧同批到达, 该组合几乎不发生.
            frames.append(frame)
            buf = bytearray()
            break
        # 不完整前缀: 帧头/尾标记只到一半 → 保留缓冲区, 等下一次读取再验证,
        # 防止分块边界处误删合法帧 (完整A + 0xAA, 或 A + 0x6D + 0xAA, 0x55 后到).
        if len(buf) == expect + 1 and buf[expect] in (0xAA, 0x6D):
            break                        # 等 0x55 (帧头) 或 AA55 (尾标记后)
        if len(buf) == expect + 2 and buf[expect] == 0x6D and buf[expect + 1] == 0xAA:
            break                        # 等 0x55 (0x6D + 帧头)
        # 帧尾异常: 丢字节/噪声 → 丢弃本帧头重新同步
        buf = buf[2:]
    return frames, buf


def _dist_raw_to_m(dist_raw):
    """S9 距离换算: 实测标定 1 unit = 0.25mm (16bit raw /4 得 mm → /4000 得 m).
    实测: 0.7m 墙 → dist_raw≈2760 → 2760/4000 = 0.69m."""
    return dist_raw / 4000.0


def _radar_angle_to_bin(angle_deg, angle_offset):
    """雷达原始角度(度) → 声明 bin.

    镜像: 实测物理左(逆时针) → 原始角度减小, 数据递增 = 物理顺时针,
          与 LaserScan 逆时针为正相反, 需取反 (360 - angle_deg).
    offset: 使车头方向落到声明 0°. 桌面标定车头≈原始 89.5°,
            镜像后 270.5° + (-90) = 180.5° → 声明 ~0.5°.
    """
    angle_deg = (360.0 - angle_deg) % 360.0
    return int(round(angle_deg + angle_offset)) % NUM_BINS


class S9LidarDriver:
    def __init__(self):
        port = rospy.get_param("~port", "/dev/ttyUSB0")
        self.frame_id = rospy.get_param("~frame_id", "laser_link")
        self.min_range = rospy.get_param("~min_range", 0.03)
        self.max_range = rospy.get_param("~max_range", 8.0)   # S9 实际量程 ~5m, 与 amcl laser_max_range=8.0 一致
        # 角度标定偏移 (度): 默认 -90, 使"车头方向"映射到声明 0°(LaserScan 约定 0°=车头).
        # 实测 (桌面标定): 车头在原始角度 ~89.5°, 镜像后 ~270.5°, 需 -90.5 到 bin 180(声明0°).
        # 实车装好后请重新标定此值 (放一物体于车头, 调 offset 使其出现在 0°).
        self.angle_offset = rospy.get_param("~angle_offset", -90.0)

        self.running = True
        self.frame_count = 0
        self.scan_count = 0
        self.cross_count = 0
        self.last_log = time.time()

        # 360° 缓冲区
        self.ranges = [float('inf')] * NUM_BINS
        self.intensities = [0.0] * NUM_BINS
        self.fill_count = 0

        self.last_first = -1     # 上一帧 firstAngle
        self.last_publish = time.time()

        self.scan_pub = rospy.Publisher("/scan", LaserScan, queue_size=3)

        # 打开串口 (权限由 udev 规则管理, SETUP.md 4.6)
        try:
            self.ser = serial.Serial(port, 115200, timeout=0.02)
            rospy.loginfo("[S9] %s @ 115200 | cross=%ddeg", port, CROSS_HI)
        except serial.SerialException as e:
            rospy.logerr("[S9] 无法打开 %s: %s", port, e)
            raise

        time.sleep(0.3)
        self.ser.reset_input_buffer()

    def publish_scan(self):
        """发布一圈完整的 360° 扫描"""
        if self.fill_count < 30:
            return

        scan = LaserScan()
        scan.header = Header(stamp=rospy.Time.now(), frame_id=self.frame_id)
        # 角度约定: [-π, π). 数据在 parse_frame 中先镜像 (360-原始角度, 使递增方向符合
        # LaserScan 逆时针为正), 再加 angle_offset(-90°) 使车头方向落到声明 0°.
        # 桌面实测标定: 车头 ≈ 原始 89.5°, 镜像后 270.5° + (-90) = 180.5° → 声明 ~0.5°.
        # 注意: 安装姿态 (雷达相对车头偏航) 已并入 angle_offset, URDF laser_joint 保持无旋转.
        scan.angle_min = -math.pi
        # 360 个 1° bin: 最后一个 bin 为 +179° (π - 1°), 而非 π (声明 π 隐含 361 个元素)
        scan.angle_max = -math.pi + (NUM_BINS - 1) * math.radians(1.0)
        scan.angle_increment = math.radians(1.0)
        scan.time_increment = 1e-4
        scan.scan_time = 0.1
        scan.range_min = self.min_range
        scan.range_max = self.max_range
        scan.ranges = list(self.ranges)
        scan.intensities = list(self.intensities)

        self.scan_pub.publish(scan)
        self.scan_count += 1

        # 清空缓冲区
        self.ranges = [float('inf')] * NUM_BINS
        self.intensities = [0.0] * NUM_BINS
        self.fill_count = 0
        self.last_publish = time.time()

    def parse_frame(self, frame):
        """解析 AA 55 帧, 按 firstAngle/lastAngle 逐点插值"""
        if len(frame) < 12:
            return

        cnt = frame[3]
        if cnt == 0 or cnt > 80:
            return
        # 帧长范围校验: 帧 = 10B 头 (AA55 ct cnt firstAngle lastAngle cs) + cnt×3B 节点.
        # 实测部分帧 (ct=0x28 类) 末尾额外带 1 字节帧尾标记 0x6D, 故允许 10+cnt*3 或 11+cnt*3.
        # 注: _extract_frames 已按 cnt 定长切分 (载荷内 AA55 不会截断), 此处校验为防御性兜底;
        #     S9 cs 字段为专有校验算法, 黑盒实测 (累加/XOR/CRC/组合) 均无法匹配, 待协议文档补全.
        if len(frame) < 10 + cnt * 3 or len(frame) > 11 + cnt * 3:
            return

        # 读取 firstAngle / lastAngle (1/64 度)
        fr = struct.unpack_from("<H", frame, 4)[0]
        lr = struct.unpack_from("<H", frame, 6)[0]
        first = fr >> 1    # bit0 校验位, 右移 1 得到真实 15-bit 角度
        last = lr >> 1

        self.frame_count += 1

        # 跨零检测
        cross = (self.last_first >= 0 and
                 self.last_first > CROSS_HI * ANGLE_64 and
                 first < CROSS_LO * ANGLE_64)
        self.last_first = first

        if cross:
            self.cross_count += 1
            self.publish_scan()

        # 角度区间 (处理跨零)
        if last < first and first > 17280 and last < 5760:
            span = 23040 + last - first   # 跨零
        elif last >= first:
            span = last - first
        else:
            span = 1

        step = span / (cnt - 1) if cnt > 1 else 0
        # 只取 cnt×3 节点字节, 忽略可能存在的帧尾标记 (0x6D)
        payload = frame[10:10 + cnt * 3]

        for i in range(cnt):
            off = i * 3
            if off + 3 > len(payload):
                break

            quality = payload[off]
            d0 = payload[off + 1]
            d1 = payload[off + 2]
            dist_raw = d0 | (d1 << 8)

            if dist_raw == 0 or dist_raw >= 0xFFF0:
                continue

            # 距离单位: 实测标定 1 unit = 0.25mm (见 _dist_raw_to_m).
            # 原实现 /1000 会偏大 4 倍, 导致 SLAM/导航/跟随距离全错.
            dist_m = _dist_raw_to_m(dist_raw)
            if dist_m < self.min_range or dist_m > self.max_range:
                continue

            angle_deg = (first + step * i) / ANGLE_64 % 360
            # 镜像 + 车头偏移 → 声明 bin (见 _radar_angle_to_bin)
            bin_idx = _radar_angle_to_bin(angle_deg, self.angle_offset)

            if self.ranges[bin_idx] == float('inf'):
                self.fill_count += 1
            if dist_m < self.ranges[bin_idx]:
                self.ranges[bin_idx] = dist_m
            self.intensities[bin_idx] = max(self.intensities[bin_idx], quality)

    def run(self):
        buf = bytearray()

        while self.running and not rospy.is_shutdown():
            try:
                raw = self.ser.read(256)
                if not raw:
                    continue
                buf.extend(raw)

                # 从缓冲区提取完整帧 — 定长解析 (cnt 驱动), 见 _extract_frames
                frames, buf = _extract_frames(buf)
                for frame in frames:
                    self.parse_frame(frame)

                # 超时兜底: 0.5s 没跨零也发布
                now = time.time()
                if now - self.last_publish > 0.5 and self.fill_count >= 30:
                    self.publish_scan()

                # 日志
                if now - self.last_log > 3.0:
                    pts = sum(1 for r in self.ranges if r < self.max_range)
                    rospy.loginfo("[S9] %dfps | scan#%d cross=%d | %dpts/360",
                                  round(self.frame_count / 3), self.scan_count,
                                  self.cross_count, pts)
                    self.frame_count = 0
                    self.last_log = now

            except Exception as e:
                rospy.logwarn_throttle(5, "[S9] %s", e)
                time.sleep(0.1)

        self.ser.close()

    def stop(self):
        self.running = False
        if hasattr(self, 'ser') and self.ser and self.ser.is_open:
            self.ser.close()


if __name__ == "__main__":
    rospy.init_node("s9_lidar_driver")
    try:
        driver = S9LidarDriver()
        driver.run()
    except rospy.ROSInterruptException:
        pass
