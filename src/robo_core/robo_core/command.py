import rclpy
import numpy as np
import math
import time

from rclpy.node import Node
from std_msgs.msg import Bool
from geometry_msgs.msg import Twist, Vector3
from robo_interfaces.msg import Dict
from rclpy import qos


class CommandRobo(Node):
    def __init__(self):
        super().__init__("command_node")
        self.dat = self.create_subscription(
            Dict,
            "joystick_topic",
            self.sub_callback,
            qos_profile=qos.qos_profile_sensor_data,
        )
        self.dat

        self.encoder = self.create_subscription(
            Twist,
            "genaral_topic",
            self.sub_encode_callback,
            qos_profile=qos.qos_profile_sensor_data,
        )
        self.encoder

        self.sent_command = self.create_publisher(
            Twist, "command_topic", qos_profile=qos.qos_profile_system_default
        )
        self.sent_command_timer = self.create_timer(0.05, self.sent_command_callback)

        self.declare_parameters(
            "",
            [
                ("box_pos_1", None),
                ("box_pos_2", None),
                ("box_pos_3", None),
                ("box_pos_4", None),
                ("box_pos_5", None),
            ],
        )
        self.encode_box = [
            self.get_parameter("box_pos_1").get_parameter_value().double_value,
            self.get_parameter("box_pos_2").get_parameter_value().double_value,
            self.get_parameter("box_pos_3").get_parameter_value().double_value,
            self.get_parameter("box_pos_4").get_parameter_value().double_value,
            self.get_parameter("box_pos_5").get_parameter_value().double_value,
        ]

        self.grab_mode = 0
        self.pre_grab = -1
        self.encode_value = 0

        self.spin_mode = 0
        self.pre_spin = -1

    def sub_callback(self, msg):
        self.axes = dict(zip(msg.key_axes, msg.value_axes))
        self.buttons = dict(zip(msg.key_buttons, msg.value_buttons))

    def sub_encode_callback(self, msg):
        self.encode_value = msg.linear.x

    def sent_command_callback(self):  # publisher drive topic
        msg = Twist()

        msg.linear.x = 0.0
        msg.linear.y = 0.0
        msg.linear.z = 0.0
        msg.angular.x = 0.0
        msg.angular.y = 0.0
        msg.angular.z = 0.0

        try:
            if self.buttons["X"] == 1:
                msg.linear.x = 100.0
            elif self.buttons["B"] == 1:
                msg.linear.x = -100.0

            if self.pre_spin != self.buttons["R2"]:
                self.pre_spin = self.buttons["R2"]
                if self.pre_spin == 1:
                    self.spin_mode += 1

            if self.spin_mode == 0:
                msg.linear.x = 0.0
            elif self.spin_mode == 1:
                for i in self.encode_box:
                    if self.encode_box[i] != self.encode_value:
                        msg.linear.x = 100.0
                    else:
                        msg.linear.x = 0.0
            else:
                self.spin_mode = 0

            if self.pre_grab != self.buttons["A"]:
                self.pre_grab = self.buttons["A"]
                if self.pre_grab == 1:
                    self.grab_mode += 1

            if self.grab_mode == 0:
                msg.angular.x = 43.0
                msg.angular.y = 160.0
                msg.angular.z = 20.0
            elif self.grab_mode == 1:
                msg.angular.x = 43.0
                msg.angular.y = 80.0
                msg.angular.z = 100.0
            elif self.grab_mode == 2:
                msg.angular.x = 160.0
                msg.angular.y = 80.0
                msg.angular.z = 100.0
            elif self.grab_mode == 3:
                msg.angular.x = 160.0
                msg.angular.y = 160.0
                msg.angular.z = 20.0
            else:
                self.grab_mode = 0

        except KeyError:
            pass
        except AttributeError:
            pass

        self.get_logger().info(f"{self.spin_mode}")
        self.sent_command.publish(msg)


def main():
    rclpy.init()

    sub = CommandRobo()
    rclpy.spin(sub)

    rclpy.shutdown()


if __name__ == "__main__":
    main()
