import rclpy
import numpy as np
import math

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

        self.grab_mode = 0
        self.pre_grab = -1
        self.encode_value = 0

        self.spin_mode = 0
        self.pre_spin = -1
        self.lastTime = self.get_clock().now().to_msg().sec
        self.Integral = 0
        self.lastError = 0

        self.box_pos = [0, 400, 800, 1200, 1600]
        self.select_Box = 0
        self.next_select_Box = 0
        self.setpoint = 0
        self.setpoint_adder = 0

        self.set_new_setpoint = False
        self.setPID = False

    def PID(self, Kp, Ki, Kd, min_power, setpoint, current):
        currentTime = self.get_clock().now().to_msg().sec
        if (currentTime - self.lastTime) == 0:
            deltha_time = 1e-2
        else:
            deltha_time = currentTime - self.lastTime
        error_value = setpoint - current
        self.Integral += error_value * deltha_time
        if abs(self.Integral) >= 255:
            self.Integral = 255 * (self.Integral / abs(self.Integral))
        Derivative = (error_value - self.lastError) / deltha_time
        self.lastError = error_value
        # self.get_logger().info(f"{self.Integral}, {setpoint} {current}")
        return (
            (error_value * Kp)
            + (self.Integral * Ki)
            + (Derivative * Kd)
            + (min_power * error_value / abs(error_value))
        )

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
            current = self.encode_value
            if self.select_Box < self.next_select_Box:
                if (
                    self.next_select_Box - self.select_Box
                    < len(self.box_pos) - self.next_select_Box + self.select_Box
                ):
                    self.setpoint_adder = self.box_pos[
                        self.next_select_Box - self.select_Box
                    ]
                else:
                    self.setpoint_adder = (
                        -1
                        * self.box_pos[
                            len(self.box_pos) - self.next_select_Box + self.select_Box
                        ]
                    )
            elif self.select_Box == self.next_select_Box:
                self.set_new_setpoint = False
                self.setpoint_adder = self.box_pos[
                    self.next_select_Box - self.select_Box
                ]
            else:
                if (
                    self.select_Box - self.next_select_Box
                    > self.next_select_Box - self.select_Box + len(self.box_pos)
                ):
                    self.setpoint_adder = self.box_pos[
                        self.next_select_Box - self.select_Box + len(self.box_pos)
                    ]
                else:
                    self.setpoint_adder = (
                        -1 * self.box_pos[self.select_Box - self.next_select_Box]
                    )

            if self.select_Box != self.next_select_Box:
                self.set_new_setpoint = True
                self.setPID = False
            if self.set_new_setpoint:
                self.setpoint = self.setpoint_adder + current

                # self.get_logger().info(
                #     f"{self.select_Box} {current} {self.setpoint} {self.setpoint_adder} {self.next_select_Box}"
                # )
            self.get_logger().info(
                f"{self.setpoint} {current} {self.set_new_setpoint} {self.select_Box} {self.next_select_Box}"
            )
            self.select_Box = self.next_select_Box

            if self.buttons["X"] == 1:
                self.next_select_Box = 1
            if self.buttons["B"] == 1:
                self.next_select_Box = 4
            if self.buttons["Y"] == 1:
                self.next_select_Box = 0

            if (
                not (self.setpoint - 5 < current and self.setpoint + 5 > current)
                and self.setPID
            ):
                self.Integral = 0
                self.lastError = 0
                value = self.PID(
                    0.14,  # 0.13
                    0.14,  # 0.13
                    0,
                    25,
                    self.setpoint,
                    current,
                )
                if abs(value) >= 255:
                    value = 255.0 * value / abs(value)

                msg.linear.x = float(-1 * value)
            else:
                msg.linear.x = 0.0
                self.setPID = False
            self.lastTime = self.get_clock().now().to_msg().sec

            if self.pre_grab != self.buttons["A"]:
                self.pre_grab = self.buttons["A"]
                if self.pre_grab == 1:
                    self.grab_mode += 1

            if self.grab_mode == 0:
                msg.angular.x = 29.0
                msg.angular.y = 160.0
                msg.angular.z = 20.0
            elif self.grab_mode == 1:
                msg.angular.x = 29.0
                msg.angular.y = 75.0
                msg.angular.z = 105.0
            elif self.grab_mode == 2:
                msg.angular.x = 150.0
                msg.angular.y = 75.0
                msg.angular.z = 105.0
            elif self.grab_mode == 3:
                msg.angular.x = 150.0
                msg.angular.y = 160.0
                msg.angular.z = 20.0
            else:
                self.grab_mode = 0

        except KeyError:
            pass
        except AttributeError:
            pass

        self.sent_command.publish(msg)


def main():
    rclpy.init()

    sub = CommandRobo()
    rclpy.spin(sub)

    rclpy.shutdown()


if __name__ == "__main__":
    main()
