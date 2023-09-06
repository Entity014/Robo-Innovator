import rclpy
import numpy as np
import tf2_ros
import math
import tf_transformations

from rclpy.node import Node
from geometry_msgs.msg import Twist, TransformStamped
from nav_msgs.msg import Odometry
from rclpy import qos


class PubOdomRobo(Node):
    def __init__(self):
        super().__init__("pub_odom_node")

        self.odom = self.create_subscription(
            Twist,
            "motor_topic",
            self.sub_odom_callback,
            qos_profile=qos.qos_profile_sensor_data,
        )
        self.odom

        self.sent_odom = self.create_publisher(
            Odometry, "odom/data_raw", qos_profile=qos.qos_profile_system_default
        )
        self.sent_odom_timer = self.create_timer(0.05, self.sent_odom_callback)
        self.odom_msg = Odometry()
        self.tf_broadcaster = tf2_ros.TransformBroadcaster(self)

    def sub_odom_callback(self, msg):
        self.odom_msg.header.stamp = self.get_clock().now().to_msg()
        self.odom_msg.header.frame_id = "odom"
        self.odom_msg.child_frame_id = "base_link"

        quat = tf_transformations.quaternion_from_euler(0.0, 0.0, msg.linear.z)
        self.odom_msg.pose.pose.position.x = msg.linear.x
        self.odom_msg.pose.pose.position.y = msg.linear.y
        self.odom_msg.pose.pose.position.z = 0.0
        self.odom_msg.pose.pose.orientation.x = quat[0]
        self.odom_msg.pose.pose.orientation.y = quat[1]
        self.odom_msg.pose.pose.orientation.z = quat[2]
        self.odom_msg.pose.pose.orientation.w = quat[3]
        # self.get_logger().info(f"{quat}")

    def sent_odom_callback(self):
        self.sent_odom.publish(self.odom_msg)

        t = TransformStamped()
        t.header.stamp = self.odom_msg.header.stamp
        t.header.frame_id = self.odom_msg.header.frame_id
        t.child_frame_id = self.odom_msg.child_frame_id
        t.transform.translation.x = self.odom_msg.pose.pose.position.x
        t.transform.translation.y = self.odom_msg.pose.pose.position.y
        t.transform.translation.z = 0.0
        t.transform.rotation.x = self.odom_msg.pose.pose.orientation.x
        t.transform.rotation.y = self.odom_msg.pose.pose.orientation.y
        t.transform.rotation.z = self.odom_msg.pose.pose.orientation.z
        t.transform.rotation.w = self.odom_msg.pose.pose.orientation.w
        # quaternion = (
        #     self.odom_msg.pose.pose.orientation.x,
        #     self.odom_msg.pose.pose.orientation.y,
        #     self.odom_msg.pose.pose.orientation.z,
        #     self.odom_msg.pose.pose.orientation.w,
        # )
        # quaternion_length = (
        #     math.sqrt(sum(q**2 for q in quaternion))
        #     if math.sqrt(sum(q**2 for q in quaternion)) != 0
        #     else 1
        # )
        # normalized_quaternion = [q / quaternion_length for q in quaternion]
        # t.transform.rotation.w = normalized_quaternion[3]
        # t.transform.rotation.x = normalized_quaternion[0]
        # t.transform.rotation.y = normalized_quaternion[1]
        # t.transform.rotation.z = normalized_quaternion[2]
        self.tf_broadcaster.sendTransform(t)


def main():
    rclpy.init()

    sub = PubOdomRobo()
    rclpy.spin(sub)

    rclpy.shutdown()


if __name__ == "__main__":
    main()
