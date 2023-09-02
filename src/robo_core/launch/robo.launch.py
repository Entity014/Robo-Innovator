import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    ld = LaunchDescription()

    config = os.path.join(
        get_package_share_directory("robo_core"), "config", "params.yaml"
    )

    node_microros = Node(
        package="micro_ros_agent",
        executable="micro_ros_agent",
        output="screen",
        arguments=["serial", "--dev", "/dev/ttyACM0"],
    )
    node_controller = Node(package="robo_core", executable="controller_node")
    node_joy = Node(package="joy", executable="joy_node")
    node_drive = Node(package="robo_core", executable="drive_node", parameters=[config])
    node_command = Node(
        package="robo_core",
        executable="command_node",
    )
    node_rviz = Node(
        package="robo_core",
        executable="rviz_node",
    )

    ld.add_action(node_microros)
    ld.add_action(node_joy)
    ld.add_action(node_controller)
    ld.add_action(node_drive)
    ld.add_action(node_command)
    ld.add_action(node_rviz)

    return ld
