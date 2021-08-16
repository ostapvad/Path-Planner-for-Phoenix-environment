#include "ros/ros.h"
#include "phoenix_msgs/Status.h"
#include "geometry_msgs/Pose2D.h"
#include "nav_msgs/Odometry.h"
#include <tf/transform_datatypes.h>
#include <cmath>
#include <sys/time.h> 

float NormAngle(float angle){
    while (angle <= -M_PI) angle += 2*M_PI;
    while (angle > M_PI) angle -= 2*M_PI;
    return angle;
}

float sign_fcn(float value){
    float sign = (value!=0)?value/abs(value):value;
    return value;
}
float QuatToYaw(const ::geometry_msgs::Quaternion quat){
    tf::Quaternion q(quat.x, quat.y, quat.z, quat.w);
    tf::Matrix3x3 m(q);
	double roll, pitch, yaw;
	m.getRPY(roll, pitch, yaw);
   return yaw; 
}

