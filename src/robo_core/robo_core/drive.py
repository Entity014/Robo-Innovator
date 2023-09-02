import rclpy
import numpy as np
import math
import time

from rclpy.node import Node
from std_msgs.msg import Bool
from geometry_msgs.msg import Twist, Vector3
from robo_interfaces.msg import Dict
from rclpy import qos


class DriveRobo(Node):
    def __init__(self):
        super().__init__("drive_node")
        self.dat = self.create_subscription(
            Dict,
            "joystick_topic",
            self.sub_callback,
            qos_profile=qos.qos_profile_sensor_data,
        )
        self.dat

        self.sent_drive = self.create_publisher(
            Twist, "drive_topic", qos_profile=qos.qos_profile_system_default
        )
        self.sent_drive_timer = self.create_timer(0.05, self.sent_drive_callback)

        self.declare_parameters("", [("speed_motor", None), ("path_auto", None)])
        self.speed = (
            self.get_parameter("speed_motor").get_parameter_value().double_value
        ) / 255

    def sub_callback(self, msg):
        self.axes = dict(zip(msg.key_axes, msg.value_axes))
        self.buttons = dict(zip(msg.key_buttons, msg.value_buttons))

    def sent_drive_callback(self):  # publisher drive topic
        msg = Twist()

        x = 0.0
        y = 0.0
        # motor
        msg.linear.x = 0.0
        msg.linear.y = 0.0
        msg.linear.z = 0.0
        # Servo
        msg.angular.x = 0.0
        msg.angular.y = 0.0
        msg.angular.z = 0.0
        servoFront = 0.0
        servoLeft = 0.0
        servoRight = 0.0

        try:
            if (self.axes["AX"] != 0) or (self.axes["AY"] != 0):
                x = -1 * self.axes["AX"]
                y = self.axes["AY"]

            else:
                x = -1 * self.axes["LX"]
                y = self.axes["LY"]

            turn = np.interp(self.axes["RX"], [-1, 1], [-1 * self.speed, self.speed])
            theta = math.atan2(y, x)
            thetaDiff = lambda x: x - 180 if x > 180 else x + 180 if x < 0 else x
            thetaCircle = lambda x: x % 360 if x > 360 else x + 360 if x < 0 else x
            if turn != 0:
                servoFront = 35
                servoLeft = 130
                servoRight = 60
            elif x != 0 or y != 0:
                servoFront = thetaDiff(88.70085 * math.sin(abs(theta)) + 30.97)
                servoLeft = thetaDiff(100.81482 * math.sin(abs(theta)) + 60.37)
                servoRight = thetaDiff(95.81446 * math.sin(abs(theta)) + 145.23)
            power = math.hypot(x, y)
            sin = math.sin(theta - math.pi / 4)
            cos = math.cos(theta - math.pi / 4)
            Max = max(abs(sin), abs(cos))
            motorFront = power * sin / Max + turn
            motorLeft = power * sin / Max - turn
            motorRight = power * cos / Max + turn

            if (power + abs(turn)) > 1:
                motorFront /= power + abs(turn)
                motorLeft /= power + abs(turn)
                motorRight /= power + abs(turn)

            msg.angular.x = float(round(servoFront))
            msg.angular.y = float(round(servoLeft))
            msg.angular.z = float(round(servoRight))
            msg.linear.x = float(round(motorFront * 255))
            msg.linear.y = float(round(motorLeft * 255))
            msg.linear.z = float(round(motorRight * 255))

        except KeyError:
            pass
        except AttributeError:
            pass

        self.sent_drive.publish(msg)


def main():
    rclpy.init()

    sub = DriveRobo()
    rclpy.spin(sub)

    rclpy.shutdown()


if __name__ == "__main__":
    main()
