//===============================================================
// talker.cpp
//---------------------------------------------------------------
//
// http://wiki.ros.org/ROS/Tutorials/WritingPublisherSubscriber%28c%2B%2B%29
//
//===============================================================

#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "talker");
    ros::NodeHandle n; // ROS Node handler
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000); // ROS Topic "chatter"
    ros::Rate loop_rate(10); // ROS Node activation frequency, here 10Hz, so 100ms between each sleep()
    int count = 0;

    while (ros::ok())
    {
        std_msgs::String msg;
        std::stringstream ss;
        ss << "hello world " << count;
        msg.data = ss.str(); // fill ROS Message 'std_msgs' with the string 'ss' created

        ROS_INFO("%s", msg.data.c_str()); // echo on terminal of ROS Message content
        chatter_pub.publish(msg); // publish on ROS Topic the message

        ros::spinOnce(); // enable callbacks treatment
        loop_rate.sleep(); // wait for next ROS Node activation
        ++count;
    }

    return 0;
}
