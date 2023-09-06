from setuptools import setup
import os
from glob import glob


package_name = "robo_core"

setup(
    name=package_name,
    version="0.0.0",
    packages=[package_name],
    data_files=[
        ("share/ament_index/resource_index/packages", ["resource/" + package_name]),
        ("share/" + package_name, ["package.xml"]),
        (os.path.join("share", package_name), glob("launch/*launch.py")),
        (os.path.join("share", package_name, "config"), glob("config/*.yaml")),
        (os.path.join("share", package_name, "map"), glob("map/*.yaml")),
        (os.path.join("share", package_name, "map"), glob("map/*.pgm")),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="entity014",
    maintainer_email="phytes.narawit@gmail.com",
    description="TODO: Package description",
    license="TODO: License declaration",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "controller_node = robo_core.controller:main",
            "drive_node = robo_core.drive:main",
            "command_node = robo_core.command:main",
            "pub_odom_node = robo_core.pub_odom:main",
        ],
    },
)
