import struct
import sys
import os

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "src", "robot_bringup", "scripts"))

from s9_lidar_driver import (_extract_frames, _dist_raw_to_m,
                             _radar_angle_to_bin, NUM_BINS)


# ============ 帧构造辅助 ============
def make_frame(cnt, inject_aa55=False):
    """构造一帧: AA55 ct cnt firstAngle lastAngle cs + cnt×3B 节点"""
    head = (b'\xAA\x55' + bytes([0x3e, cnt]) +
            struct.pack('<H', 100 << 1) + struct.pack('<H', 200 << 1) +
            b'\x12\x34')
    nodes = bytes([0x00, 0x01, 0x02]) * cnt  # cnt×3 字节
    if inject_aa55 and len(nodes) > 6:
        nodes = nodes[:6] + b'\xAA\x55' + nodes[6:]  # 载荷内插 AA55
        nodes = nodes[:cnt * 3]
    return head + nodes


# ============ 定长帧提取 ============
def test_extract_normal_stream():
    stream = make_frame(40) + make_frame(13) + make_frame(1)
    frames, rest = _extract_frames(bytearray(stream))
    assert len(frames) == 3
    assert [len(f) for f in frames] == [130, 49, 13]
    assert len(rest) == 0


def test_extract_aa55_in_payload():
    # 载荷内出现 AA55: 定长解析不受污染 (旧按下一帧头切分会截断)
    stream = make_frame(40, inject_aa55=True) + make_frame(13)
    frames, rest = _extract_frames(bytearray(stream))
    assert len(frames) == 2
    assert len(frames[0]) == 130
    assert len(frames[1]) == 49


def test_extract_chunked():
    # 串口分块到达
    stream = make_frame(5) + make_frame(5)
    buf = bytearray()
    frames = []
    for chunk in (stream[:17], stream[17:]):
        buf.extend(chunk)
        fs, buf = _extract_frames(buf)
        frames.extend(fs)
    fs, buf = _extract_frames(buf)
    frames.extend(fs)
    assert len(frames) == 2
    assert len(buf) == 0


def test_extract_frame_tail_marker():
    # 帧尾 0x6D 标记被跳过
    stream = make_frame(12) + b'\x6D' + make_frame(12)
    frames, rest = _extract_frames(bytearray(stream))
    assert len(frames) == 2
    assert len(rest) == 0


def test_extract_noise_prefix():
    stream = b'\x00\x11' + make_frame(3)
    frames, _ = _extract_frames(bytearray(stream))
    assert len(frames) == 1
    assert len(frames[0]) == 19


def test_extract_invalid_cnt_recovery():
    # cnt=255 非法 → 跳过帧头继续搜索, 恢复同步
    bad = b'\xAA\x55\x3e\xFF' + b'\x00' * 50
    stream = bad + make_frame(3)
    frames, _ = _extract_frames(bytearray(stream))
    assert len(frames) == 1


def test_extract_lost_byte_resync():
    # 丢 1 字节 (A[:-1] + B + C): 损坏的 A 应被拒绝 (帧尾边界非 AA55),
    # B 不应被吞掉, C 正常. 边界验证防"用下一帧首字节补足损坏帧".
    a, b, c = make_frame(40), make_frame(13), make_frame(1)
    stream = a[:-1] + b + c
    frames, _ = _extract_frames(bytearray(stream))
    assert len(frames) == 2
    assert len(frames[0]) == len(b)
    assert len(frames[1]) == len(c)


def test_extract_blocked_at_frame_header():
    # 分块边界: 完整 A + 0xAA (下一帧头第一字节), 0x55 后到 → A 不应被误删
    a, b = make_frame(5), make_frame(5)
    stream = a + b
    buf = bytearray(stream[:len(a) + 1])      # A + 0xAA
    frames1, buf1 = _extract_frames(buf)
    assert len(frames1) == 0                  # 帧头不完整, 保留等待
    assert len(buf1) == len(a) + 1
    buf1.extend(stream[len(a) + 1:])          # 0x55 + 剩余
    frames2, _ = _extract_frames(buf1)
    assert len(frames2) == 2                  # A 和 B 都提取


def test_extract_blocked_after_tail_marker():
    # 分块边界: A + 0x6D + 0xAA (尾标记+帧头第一字节), 0x55 后到 → A 不误删
    a, b = make_frame(5), make_frame(5)
    stream = a + b'\x6D' + b
    buf = bytearray(stream[:len(a) + 2])      # A + 0x6D + 0xAA
    frames1, buf1 = _extract_frames(buf)
    assert len(frames1) == 0                  # 尾标记后帧头不完整, 保留等待
    assert len(buf1) == len(a) + 2
    buf1.extend(stream[len(a) + 2:])          # 0x55 + 剩余
    frames2, _ = _extract_frames(buf1)
    assert len(frames2) == 2                  # A 和 B 都提取


def test_extract_blocked_at_tail_marker():
    # 分块边界: 完整 A + 0x6D (尾标记, 后续帧头未到) → A 保留, 不误删
    a, b = make_frame(5), make_frame(5)
    stream = a + b'\x6D' + b
    buf = bytearray(stream[:len(a) + 1])      # A + 0x6D
    frames1, buf1 = _extract_frames(buf)
    assert len(frames1) == 0
    buf1.extend(stream[len(a) + 1:])          # 0xAA 0x55 + 剩余
    frames2, _ = _extract_frames(buf1)
    assert len(frames2) == 2


def test_extract_all_split_positions():
    # 穷举验证: 合法数据流在任意两段切分位置 (串口分块到达) 下,
    # 经两轮提取都必须得到全部帧且无残留, 防止分块边界回归.
    def verify(stream, expect_count):
        for cut in range(1, len(stream)):
            buf = bytearray()
            frames = []
            buf.extend(stream[:cut])
            fs, buf = _extract_frames(buf)
            frames.extend(fs)
            buf.extend(stream[cut:])
            fs, buf = _extract_frames(buf)
            frames.extend(fs)
            assert len(frames) == expect_count, \
                f'cut={cut}: 提取 {len(frames)} 帧, 期望 {expect_count}'
            assert len(buf) == 0, f'cut={cut}: 残留 {len(buf)} 字节'

    # 普通帧流 (3 帧, 含 0x6D 尾标记)
    a, b, c = make_frame(40), make_frame(13), make_frame(1)
    verify(a + b + c, 3)
    verify(a + b'\x6D' + b + b'\x6D' + c, 3)
    # 帧体内部切分 (之前的手工分块用例)
    verify(make_frame(5) + make_frame(5), 2)


# ============ 距离换算 ============
def test_dist_raw_to_m_calibration():
    # 实测标定: 0.7m 墙 → dist_raw≈2760
    assert abs(_dist_raw_to_m(2760) - 0.69) < 0.01
    assert abs(_dist_raw_to_m(4000) - 1.0) < 1e-6
    assert abs(_dist_raw_to_m(1000) - 0.25) < 1e-6


# ============ 角度换算 (镜像 + 偏移) ============
def test_angle_bin_mirror():
    # 车头 ≈ 原始 89.5°: 镜像后 270.5° + (-90) = 180.5 → bin 180 (声明 0°)
    bin_idx = _radar_angle_to_bin(89.5, -90.0)
    assert bin_idx == 180


def test_angle_bin_offset_effect():
    # 相同原始角度, 不同 offset 产生不同 bin
    assert _radar_angle_to_bin(0.0, 0.0) == 0          # 0° → bin 0
    assert _radar_angle_to_bin(0.0, -90.0) == 270      # 0° → bin 270
    assert _radar_angle_to_bin(180.0, -90.0) == 90     # 180° → bin 90


def test_angle_bin_wraparound():
    # bin 取模 360
    assert _radar_angle_to_bin(0.0, -1.0) == 359
    # 原始 359.9° → 镜像 0.1° → +(-90) = -89.9 → round(-90) → %360 = 270
    assert _radar_angle_to_bin(359.9, -90.0) == 270
    assert 0 <= _radar_angle_to_bin(359.9, -90.0) < NUM_BINS
