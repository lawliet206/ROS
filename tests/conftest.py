"""Test-only stand-ins for ROS and optional runtime dependencies.

The project runs on ROS Noetic, but its parser and controller unit tests should
also be executable by contributors and CI without a ROS installation.
"""

import importlib
import sys
import types


def _module_available(name):
    try:
        importlib.import_module(name)
    except ModuleNotFoundError:
        return False
    return True


def _install_message_module(package, names):
    if _module_available(package + ".msg"):
        return

    package_module = sys.modules.setdefault(package, types.ModuleType(package))
    message_module = types.ModuleType(package + ".msg")

    class Message:
        def __init__(self, *args, **kwargs):
            for key, value in kwargs.items():
                setattr(self, key, value)

    for name in names:
        setattr(message_module, name, type(name, (Message,), {}))

    sys.modules[package + ".msg"] = message_module
    package_module.msg = message_module


if not _module_available("rospy"):
    rospy = types.ModuleType("rospy")
    rospy.get_param = lambda _name, default=None: default
    rospy.loginfo = lambda *_args, **_kwargs: None
    rospy.logwarn = lambda *_args, **_kwargs: None
    rospy.logerr = lambda *_args, **_kwargs: None
    rospy.init_node = lambda *_args, **_kwargs: None
    rospy.is_shutdown = lambda: False
    sys.modules["rospy"] = rospy

if not _module_available("serial"):
    serial = types.ModuleType("serial")

    class Serial:
        pass

    serial.Serial = Serial
    serial.SerialException = Exception
    sys.modules["serial"] = serial

_install_message_module("std_msgs", ("Bool", "Float32", "Header"))
_install_message_module("sensor_msgs", ("CompressedImage", "Imu", "LaserScan"))
_install_message_module("nav_msgs", ("Odometry",))

if not _module_available("geometry_msgs.msg"):
    geometry_msgs = sys.modules.setdefault(
        "geometry_msgs", types.ModuleType("geometry_msgs")
    )
    geometry_msgs_msg = types.ModuleType("geometry_msgs.msg")

    class Vector3:
        def __init__(self):
            self.x = 0.0
            self.y = 0.0
            self.z = 0.0

    class Twist:
        def __init__(self):
            self.linear = Vector3()
            self.angular = Vector3()

    class PoseStamped:
        def __init__(self, *args, **kwargs):
            self.header = None
            self.pose = None
            for key, value in kwargs.items():
                setattr(self, key, value)

    geometry_msgs_msg.Twist = Twist
    geometry_msgs_msg.Vector3 = Vector3
    geometry_msgs_msg.PoseStamped = PoseStamped
    sys.modules["geometry_msgs.msg"] = geometry_msgs_msg
    geometry_msgs.msg = geometry_msgs_msg

if not _module_available("cv2"):
    cv2 = types.ModuleType("cv2")
    cv2.ROTATE_180 = 1

    def rotate(frame, code):
        if code != cv2.ROTATE_180:
            raise ValueError("Only 180-degree rotation is supported in tests")
        return frame[::-1, ::-1]

    cv2.rotate = rotate
    sys.modules["cv2"] = cv2

if not _module_available("ultralytics"):
    ultralytics = types.ModuleType("ultralytics")

    class YOLO:
        def __init__(self, *_args, **_kwargs):
            pass

    ultralytics.YOLO = YOLO
    sys.modules["ultralytics"] = ultralytics

if not _module_available("actionlib"):
    actionlib = types.ModuleType("actionlib")

    class SimpleActionClient:
        def __init__(self, *args, **kwargs):
            pass

    class GoalStatus:
        SUCCEEDED = 3
        ABORTED = 4

    actionlib.SimpleActionClient = SimpleActionClient
    actionlib.GoalStatus = GoalStatus
    sys.modules["actionlib"] = actionlib

_install_message_module("move_base_msgs", ("MoveBaseAction", "MoveBaseGoal"))

if not _module_available("tf.transformations"):
    tf = sys.modules.setdefault("tf", types.ModuleType("tf"))
    tf_transformations = types.ModuleType("tf.transformations")
    tf_transformations.quaternion_from_euler = lambda roll, pitch, yaw: [0.0, 0.0, yaw, 1.0]
    sys.modules["tf.transformations"] = tf_transformations
    tf.transformations = tf_transformations
