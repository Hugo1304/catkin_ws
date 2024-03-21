#include <ros/ros.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <sstream>
#include <geometry_msgs/Twist.h>

const double linear_max_speed = 0.1; // Vitesse maximale linéaire
const double angular_max_speed = 0.4; // Vitesse maximale angulaire
const double linear_acceleration = 0.01; // Accélération linéaire
const double angular_acceleration = 0.1; // Accélération angulaire
const double speed_increment = 0.02; // Incrément de vitesse

double clamp(double value, double min, double max) {
    return std::min(std::max(value, min), max);
}

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
    ros::Publisher cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 100);
    ros::Rate loop_rate(50); // Taux de boucle à 50 Hz pour un contrôle plus réactif

    double linear_speed = 0.0; // Vitesse linéaire actuelle
    double angular_speed = 0.0; // Vitesse angulaire actuelle

    while (ros::ok()) {
        geometry_msgs::Twist twist;

        int clavier = getch();

        // Accélérer lorsque la touche 'z' est enfoncée
        if (clavier == 'z') {
            linear_speed = clamp(linear_speed + linear_acceleration, -linear_max_speed, linear_max_speed);
        }
        // Ralentir la vitesse linéaire lorsque nous tournons
        else if (clavier == 'q' || clavier == 'd') {
            linear_speed = clamp(linear_speed - speed_increment, -linear_max_speed, linear_max_speed);
        }
        // Tourner à gauche
        else if (clavier == 'q') {
            angular_speed = clamp(angular_speed + angular_acceleration, -angular_max_speed, angular_max_speed);
        }
        // Tourner à droite
        else if (clavier == 'd') {
            angular_speed = clamp(angular_speed - angular_acceleration, -angular_max_speed, angular_max_speed);
        }
        // Augmenter la vitesse linéaire avec la touche '8'
        else if (clavier == '8') {
            linear_speed = clamp(linear_speed + speed_increment, -linear_max_speed, linear_max_speed);
        }
        // Diminuer la vitesse linéaire avec la touche '5'
        else if (clavier == '5') {
            linear_speed = clamp(linear_speed - speed_increment, -linear_max_speed, linear_max_speed);
        }
        // Arrêter les mouvements
        else if (clavier == 'a') {
            linear_speed = 0.0;
            angular_speed = 0.0;
        }
        // Arrêter le programme
        else if (clavier == 'h') {
            ROS_INFO("Arrêt du programme !");
            ros::shutdown(); // Arrêter le noeud ROS
            break; // Sortir de la boucle while
        }

        twist.linear.x = linear_speed;
        twist.angular.z = angular_speed;

        cmd_vel_pub.publish(twist);
        loop_rate.sleep();
    }

    return 0;
}