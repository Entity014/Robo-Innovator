import rclpy
from rclpy.node import Node
import numpy as np
import math

from sensor_msgs.msg import Imu
from geometry_msgs.msg import TransformStamped, Vector3
import tf2_ros

import tf2_py
from rclpy import qos


class FixedFrameBroadcaster(Node):
    def __init__(self):
        super().__init__("fixed_frame_tf2_broadcaster")
        self.tf_broadcaster = tf2_ros.TransformBroadcaster(self)
        self.timer = self.create_timer(0.1, self.broadcast_timer_callback)
        self.imu = self.create_subscription(
            Imu,
            "imu/data_raw",
            self.sub_imu_callback,
            qos_profile=qos.qos_profile_sensor_data,
        )
        self.imu

        self.frame_id = ""
        self.stamp = self.get_clock().now().to_msg()
        self.accel = Vector3()
        self.gyro = Vector3()

    def sub_imu_callback(self, msg):
        self.frame_id = msg.header.frame_id
        self.stamp = msg.header.stamp
        self.accel = msg.linear_acceleration
        self.gyro = msg.angular_velocity

    def broadcast_timer_callback(self):
        tf = TransformStamped()

        tf.header.stamp = self.stamp
        tf.header.frame_id = "base_link"
        tf.child_frame_id = self.frame_id
        tf.transform.translation.x = 0.0
        tf.transform.translation.y = 2.0
        tf.transform.translation.z = 0.0
        tf.transform.rotation.x = 0.0
        tf.transform.rotation.y = 0.0
        tf.transform.rotation.z = 0.0
        tf.transform.rotation.w = 1.0

        self.tf_broadcaster.sendTransform(tf)


def main():
    rclpy.init()
    node = FixedFrameBroadcaster()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    rclpy.shutdown()
