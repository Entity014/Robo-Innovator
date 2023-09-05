import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    ld = LaunchDescription()
    channel_type = LaunchConfiguration("channel_type", default="serial")
    serial_port = LaunchConfiguration("serial_port", default="/dev/ttyUSB0")
    serial_baudrate = LaunchConfiguration("serial_baudrate", default="256000")
    frame_id = LaunchConfiguration("frame_id", default="laser")
    inverted = LaunchConfiguration("inverted", default="false")
    angle_compensate = LaunchConfiguration("angle_compensate", default="true")
    scan_mode = LaunchConfiguration("scan_mode", default="Sensitivity")

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
        package="robo_core", executable="command_node", parameters=[config]
    )
    node_rviz = Node(
        package="imu_filter_madgwick",
        executable="imu_filter_madgwick_node",
        parameters=[{"use_mag": True}],
        remappings=[("/imu/data_raw", "/imu/data_raw")],
    )
    node_lidar = Node(
        package="sllidar_ros2",
        executable="sllidar_node",
        name="sllidar_node",
        parameters=[
            {
                "channel_type": channel_type,
                "serial_port": serial_port,
                "serial_baudrate": serial_baudrate,
                "frame_id": frame_id,
                "inverted": inverted,
                "angle_compensate": angle_compensate,
                "scan_mode": scan_mode,
            }
        ],
        output="screen",
    )
    node_pub_odom = Node(package="robo_core", executable="pub_odom_node")
    tf2_node = Node(
        package="tf2_ros",
        executable="static_transform_publisher",
        name="static_tf_pub_laser",
        arguments=["0.23", "0", "0.13", "0", "0", "0", "base_link", "laser"],
    )
    ld.add_action(node_microros)
    ld.add_action(node_pub_odom)
    ld.add_action(node_joy)
    ld.add_action(node_controller)
    ld.add_action(node_drive)
    ld.add_action(node_command)
    # ld.add_action(node_rviz)
    ld.add_action(tf2_node)
    ld.add_action(node_lidar)

    return ld
