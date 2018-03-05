//Team name: THAU
//Member: Anette Ulrichsen		amulrichsen@csu.fullerton.edu
//Member: Tyler Hatzenbuhler		thatzenbuhler@csu.fullerton.edu
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <sstream>
//create global publisher
ros::Publisher velocity_publisher;
ros::Subscriber pose_subscriber;
turtlesim::Pose turtlesim_pose;


//move turtle with a specified linear and angular velocity
// positive angle = counter clockwise
void moverot(double speed, double dist, double ang_speed);
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message);

int main (int argc, char **argv)
{
	ros::init(argc, argv, "turtle");
	ros::NodeHandle n;
	ros::Rate loop_rate(10);
	
	//initialize publisher
	velocity_publisher = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);
	pose_subscriber = n.subscribe("/turtle1/pose", 10, poseCallback);

	
	//test case -- change later
	moverot(0.8, 2.0, 1.2);
	moverot(3.0, 4.0, 1.5);
	moverot(3.0, 4.0, 1.5);
	moverot(0.8, 2.0, 1.3);
	moverot(0.8, 2.1, -0.9);
	moverot(0.8, 2.0, -0.9);
	moverot(0.1, 0.1, 1);

	ROS_INFO_STREAM("Shape 2 Complete. Press Cntrl-C to exit.\n");
	ros::spin();
}

void moverot(double speed, double dist, double ang_speed)
{
	geometry_msgs::Twist vel_msg;
	double t0 = ros::Time::now().toSec();
	double curr_dist = 0;
	ros::Rate loop_rate(100);
	
	vel_msg.linear.x = speed;
	vel_msg.linear.y = 0;
	vel_msg.linear.z = 0;
	vel_msg.angular.x = 0;
	vel_msg.angular.y = 0;
	vel_msg.angular.z = ang_speed;
	
	do {
		velocity_publisher.publish(vel_msg);
		double t1 = ros::Time::now().toSec();
		curr_dist = speed * (t1 - t0);
		ros::spinOnce();
		loop_rate.sleep();
	
	}while (curr_dist < dist);
	
	//force turtle to stop moving once loop is complete
	vel_msg.angular.z = 0;
	vel_msg.linear.x = 0;
	velocity_publisher.publish(vel_msg);
}

void poseCallback(const turtlesim::Pose::ConstPtr & pose_message)
{
	turtlesim_pose.x = pose_message -> x;
	turtlesim_pose.y = pose_message -> y;
	turtlesim_pose.theta = pose_message -> theta;
}
