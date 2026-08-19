import sys
import os

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "src", "robot_bringup", "scripts"))

from send_goals import _goals_to_tuples


# ============ 纯函数: _goals_to_tuples ============
def test_goals_xy_without_yaw():
    # [x, y] → yaw 补 0.0
    assert _goals_to_tuples([[1, 2], [3, 4]]) == [(1.0, 2.0, 0.0), (3.0, 4.0, 0.0)]


def test_goals_with_yaw():
    assert _goals_to_tuples([[1, 2, 1.57], [3, 4, -1.57]]) == [(1.0, 2.0, 1.57), (3.0, 4.0, -1.57)]


def test_goals_skips_malformed_entries():
    # 畸形条目被跳过而不是抛异常: 字符串/长度不足/数值非法/None/字典/嵌套列表
    raw = [["bad"], [5], [6, 7, "x"], [8, "y"], None, {"a": 1}, [[1, 2], 3], [1, 2]]
    assert _goals_to_tuples(raw) == [(1.0, 2.0, 0.0)]


def test_goals_empty():
    assert _goals_to_tuples([]) == []


def test_goals_string_numbers():
    # YAML 文件可能以字符串形式写数值
    assert _goals_to_tuples([["1.5", "2.5", "3"]]) == [(1.5, 2.5, 3.0)]


def test_goals_int_yaw():
    assert _goals_to_tuples([[0, 0, 3]]) == [(0.0, 0.0, 3.0)]