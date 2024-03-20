#include <ros/ros.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <sstream>
#include <geometry_msgs/Twist.h>

int getch() {
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_lflag |= IGNBRK;
    newt.c_lflag &= ~(INLCR | ICRNL | IXON | IXOFF);
    newt.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &newt);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "cmd");
    ros::NodeHandle nh;
    ros::Publisher cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    ros::Rate loop_rate(1);


    while (ros::ok()) {
       
        geometry_msgs::Twist twist;
        int clavier = getch();

         if (clavier == '8') {
            
            twist.linear.x = 2.0;
            twist.angular.z = 0;
         }  
                     
        if (clavier == '2'){
                twist.linear.x = -2.0;  // Reculer
                twist.angular.z = 0;
        }
              
        if (clavier == '4'){
                twist.angular.z = 2.0;  // Tourner à gauche
                twist.linear.x = 0;
        }
               
        if (clavier == '6'){
                twist.angular.z = -2.0;  // Tourner à droite
                twist.linear.x = 0;
        }

        if (clavier == '5'){
                twist.angular.z = 0;  // Arret
                twist.linear.x = 0;                       
        }
                
        

        cmd_vel_pub.publish(twist);
        loop_rate.sleep();
    }

    return 0;
}
