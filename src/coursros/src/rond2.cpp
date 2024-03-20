#include <ros/ros.h>
#include <sstream>
#include <geometry_msgs/Twist.h>

int main (int argc, char **argv)
{
    ros::init(argc, argv, "rond2");
    ros::NodeHandle n;
    ros::Publisher vel_pub_ = n.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1);
    ros::Rate loop_rate(1);

    int count = 0;
    while (ros::ok())
    {
        geometry_msgs::Twist twist;
        twist.linear.x = 1;
        twist.angular.z = 0.5;
        vel_pub_.publish(twist);
        std::stringstream ss;
        ss << "Cmd#" << count;

        ROS_INFO("%s", ss.str().c_str());

        ros::spinOnce();
        loop_rate.sleep();
        count++;
    }

    return 0;
}
