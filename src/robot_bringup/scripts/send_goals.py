#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
send_goals.py — move_base 多点导航 (巡点)
===========================================
运行位置: PC (你的笔记本)
前提:   move_base 已启动, 地图已加载, 已定位 (AMCL)

功能:
  依次发送多个导航目标点给 move_base, 支持等待到达或超时跳过

使用方式:
  # 示例: 发送两个目标点
  rosrun robot_bringup send_goals.py _goals:="[(1.0,2.0,0.0), (3.0,4.0,1.57)]"

  或从 YAML 文件读取目标点:
  rosrun robot_bringup send_goals.py _goal_file:=/path/to/goals.yaml

YAML 文件格式 (goals.yaml):
  goals:
    - [1.0, 2.0, 0.0]       # x, y, yaw(rad)
    - [3.0, 4.0, 1.57]
    - [5.0, 1.0, 3.14]

参数:
  _goals         点列表, JSON格式字符串
  _goal_file     YAML 文件路径 (优先级高于 _goals)
  _timeout       每个目标点的超时时间 (秒, 默认 60.0)
  _max_retries   失败/超时后的重试次数 (默认 2, 0=不重试)
  _loop          是否循环 (默认 false)
  _frame         目标坐标系 (默认 "map")
"""

import rospy
import actionlib
import math
import json
import yaml
import os

from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from geometry_msgs.msg import PoseStamped
from tf.transformations import quaternion_from_euler


class GoalSender:
    """多点导航目标发送器"""

    def __init__(self):
        # ===== 参数 =====
        self.timeout     = rospy.get_param("~timeout", 60.0)        # 每个目标超时 (秒)
        self.max_retries = rospy.get_param("~max_retries", 2)    # 失败重试次数 (0=不重试)
        self.loop        = rospy.get_param("~loop",    False)       # 是否循环
        self.frame_id    = rospy.get_param("~frame",   "map")       # 坐标系

        # 解析目标列表
        self.goals = self.parse_goals()

        if not self.goals:
            rospy.logerr("[Nav] 没有有效的导航目标点!")
            rospy.signal_shutdown("无有效目标点")
            return

        rospy.loginfo("[Nav] 已加载 %d 个导航目标点", len(self.goals))
        for i, (x, y, yaw) in enumerate(self.goals):
            rospy.loginfo("[Nav]   目标%d: x=%.2f y=%.2f yaw=%.2f°", i + 1, x, y, math.degrees(yaw))

        # ===== Action 客户端 =====
        self.client = actionlib.SimpleActionClient("move_base", MoveBaseAction)
        rospy.loginfo("[Nav] 等待 move_base action 服务器...")
        if not self.client.wait_for_server(rospy.Duration(10.0)):
            rospy.logerr("[Nav] move_base 未启动! 请先启动导航")
            rospy.signal_shutdown("move_base 未启动")
            return

        rospy.loginfo("[Nav] move_base 已连接")
        rospy.on_shutdown(self._on_shutdown)

    # ================================================================
    # 解析目标点
    # ================================================================
    def parse_goals(self):
        """从 ROS 参数或 YAML 文件加载目标点列表"""
        goal_file = rospy.get_param("~goal_file", "")

        if goal_file and os.path.exists(goal_file):
            # 从 YAML 文件加载
            with open(goal_file, 'r') as f:
                data = yaml.safe_load(f)
            raw = data.get("goals", [])
            rospy.loginfo("[Nav] 从文件加载: %s", goal_file)
        else:
            # 从参数加载 (JSON 格式)
            goals_str = rospy.get_param("~goals", "[]")
            try:
                raw = json.loads(goals_str)
            except json.JSONDecodeError:
                rospy.logerr("[Nav] 无法解析 goals 参数: %s", goals_str)
                return []

        # 转换为 (x, y, yaw) 元组
        goals = []
        for g in raw:
            if len(g) >= 2:
                x, y = float(g[0]), float(g[1])
                yaw = float(g[2]) if len(g) >= 3 else 0.0
                goals.append((x, y, yaw))
        return goals

    # ================================================================
    # 创建 MoveBaseGoal
    # ================================================================
    def make_goal(self, x, y, yaw):
        """构建 move_base 目标"""
        goal = MoveBaseGoal()
        goal.target_pose.header.frame_id = self.frame_id
        goal.target_pose.header.stamp = rospy.Time.now()

        goal.target_pose.pose.position.x = x
        goal.target_pose.pose.position.y = y
        goal.target_pose.pose.position.z = 0

        q = quaternion_from_euler(0, 0, yaw)
        goal.target_pose.pose.orientation.x = q[0]
        goal.target_pose.pose.orientation.y = q[1]
        goal.target_pose.pose.orientation.z = q[2]
        goal.target_pose.pose.orientation.w = q[3]

        return goal

    # ================================================================
    # 节点关闭回调: 取消当前 goal
    # ================================================================
    def _on_shutdown(self):
        self.client.cancel_goal()
        rospy.loginfo("[Nav] 已取消当前目标")

    # ================================================================
    # 执行导航序列
    # ================================================================
    def run(self):
        """依次发送所有目标点"""
        if not self.goals:
            rospy.signal_shutdown("无有效目标点")
            return

        idx = 0
        while not rospy.is_shutdown():
            x, y, yaw = self.goals[idx]
            rospy.loginfo("[Nav] → 前往目标%d (%.2f, %.2f, %.2f°)",
                          idx + 1, x, y, math.degrees(yaw))

            # 带重试的导航循环
            success = False
            for attempt in range(self.max_retries + 1):
                if rospy.is_shutdown():
                    return

                if attempt > 0:
                    rospy.loginfo("[Nav] 重试目标%d (第%d次)", idx + 1, attempt)

                goal = self.make_goal(x, y, yaw)
                self.client.send_goal(goal)
                finished = self.client.wait_for_result(rospy.Duration(self.timeout))

                if finished:
                    state = self.client.get_state()
                    if state == actionlib.GoalStatus.SUCCEEDED:
                        rospy.loginfo("[Nav] ✓ 目标%d 已到达", idx + 1)
                        success = True
                        break
                    else:
                        rospy.logwarn("[Nav] ✗ 目标%d 失败 (状态码: %d)", idx + 1, state)
                else:
                    rospy.logwarn("[Nav] ⏱ 目标%d 超时 (%.1fs)", idx + 1, self.timeout)
                    self.client.cancel_goal()
                    # 等待取消完成
                    self.client.wait_for_result(rospy.Duration(2.0))

            if not success:
                rospy.logerr("[Nav] 目标%d 经%d次尝试后仍失败, 跳过", idx + 1, self.max_retries + 1)

            # 下一个目标
            idx += 1
            if idx >= len(self.goals):
                if self.loop:
                    rospy.loginfo("[Nav] 巡点完成, 重新开始...")
                    idx = 0
                else:
                    rospy.loginfo("[Nav] 全部目标巡点完成!")
                    break

            # 目标间短暂停顿
            rospy.sleep(1.0)


# ================================================================
# 入口
# ================================================================
if __name__ == "__main__":
    rospy.init_node("send_goals")
    try:
        sender = GoalSender()
        sender.run()
    except rospy.ROSInterruptException:
        pass
