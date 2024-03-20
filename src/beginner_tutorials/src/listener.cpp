//===============================================================
// listener.cpp
//---------------------------------------------------------------
//
// http://wiki.ros.org/ROS/Tutorials/WritingPublisherSubscriber%28c%2B%2B%29
//
//===============================================================

#include "ros/ros.h"
#include "std_msgs/String.h"

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "listener");
    ros::NodeHandle n; // ROS Node handler
    ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback); // ROS Topic "chatter"
    ros::spin(); // enable callbacks treatment
    return 0;
}