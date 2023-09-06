import rclpy
from rclpy.node import Node
from nav2_simple_commander.robort_navigator import BasicNavigator, Navigatorresult
from geometry_msgs.msg import PoseStamped


class Navigator(Node):
    def __init__(self):
        super().__init__("nnav2_cmd_node")
        self.nav = BasicNavigator()

        self.nav.waitYntilNav2Active()
        self.ip = PoseStamped()

        self.set_initial_pose()

    def set_initial_pose(self):
        self.ip.header.frame_id = "map"
        self.ip.header.stamp = self.get_clock().now().to_msg()

        # แก้เป็นตำแหน่งเริ่มต้นหุ่นยนต์
        self.ip.pose.position.x = 0.0
        self.ip.pose.position.y = 0.0
        self.ip.pose.position.z = 0.0
        self.ip.pose.orientation.x = 0.0
        self.ip.pose.orientation.y = 0.0
        self.ip.pose.orientation.z = 0.0
        self.ip.pose.orientation.w = 1.0

        self.nav.setInitialPose(self.ip)

    def set_point(self, x, y, q):
        goal_pose = PoseStamped()
        goal_pose.header.frame_id = "map"
        goal_pose.header.stamp = self.get_clock().now().to_msg()
        goal_pose.pose.position.x = x
        goal_pose.pose.position.y = y
        goal_pose.pose.position.z = 0.0
        goal_pose.pose.orientation.x = q[0]
        goal_pose.pose.orientation.y = q[1]
        goal_pose.pose.orientation.z = q[2]
        goal_pose.pose.orientation.w = q[3]
        return goal_pose

    def goto(self, x, y, q):
        target = self.set_point(x, y, q)
        self.nav.goToPose(target)


def main():
    rclpy.init()
    nr = navRobot()
    nr.goto(1.2, 1.5, [0, 0, 0, 1])
    nr.destroy_node()


if __name__ == "__main__":
    main()
