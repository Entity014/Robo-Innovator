import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    map_file = os.path.join(
        get_package_share_directory("robo_core"), "config", "my_map.yaml"
    )
    return LaunchDescription(
        [
            Node(
                package="nav2_map_server",
                executable="map_server",
                name="map_server",
                output="screen",
                parameters=[{"use_sim_time": False}, {"yaml_filename": map_file}],
            ),
            Node(
                package="rviz2",
                executable="rviz2",
                name="rviz2",
                output="screen",
                parameters=[{"use_sim_time": False}],
            ),
            Node(
                package="nav2_lifecycle_manager",
                executable="lifecycle_manager",
                name="lifecycle_manager_mapper",
                output="screen",
                parameters=[
                    {"use_sim_time": False},
                    {"autostart": True},
                    {"node_names": ["map_server"]},
                ],
            ),
        ]
    )
