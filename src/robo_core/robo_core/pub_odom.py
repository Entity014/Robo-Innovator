import rclpy
import numpy as np
import tf2_ros


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
        self.odom_msg.child_frame_id = "base_footprint"

        self.odom_msg.pose.pose.position.x = msg.linear.x
        self.odom_msg.pose.pose.position.y = msg.linear.y
        self.odom_msg.pose.pose.position.z = 0.0
        self.odom_msg.pose.pose.orientation.x = msg.linear.z
        self.odom_msg.pose.pose.orientation.y = msg.angular.x
        self.odom_msg.pose.pose.orientation.z = msg.angular.y
        self.odom_msg.pose.pose.orientation.w = msg.angular.x

    def sent_odom_callback(self):
        self.sent_odom.publish(self.odom_msg)

        t = TransformStamped()
        t.header.stamp = self.odom_msg.header.stamp
        t.header.frame_id = self.odom_msg.header.frame_id
        t.child_frame_id = self.odom_msg.child_frame_id
        t.transform.translation.x = self.odom_msg.pose.pose.position.x
        t.transform.translation.y = self.odom_msg.pose.pose.position.y
        t.transform.translation.z = 0.0
        t.transform.rotation.w = self.odom_msg.pose.pose.orientation.w
        t.transform.rotation.x = self.odom_msg.pose.pose.orientation.x
        t.transform.rotation.y = self.odom_msg.pose.pose.orientation.y
        t.transform.rotation.z = self.odom_msg.pose.pose.orientation.z
        self.tf_broadcaster.sendTransform(t)


def main():
    rclpy.init()

    sub = PubOdomRobo()
    rclpy.spin(sub)

    rclpy.shutdown()


if __name__ == "__main__":
    main()
