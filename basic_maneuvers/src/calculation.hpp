
#include "ros/ros.h"
#include "phoenix_msgs/Status.h"
<<<<<<< HEAD
#include "geometry_msgs/Pose2D.h"
#include "nav_msgs/Odometry.h"
#include <cmath>



      
=======
#include "nav_msgs/Odometry.h"
#include <cmath>

>>>>>>> b45cb02c4b15ab5103ac7fe4cc7e88b829370475
float QuatToYaw(const ::geometry_msgs::Quaternion quat){
    float sin_yaw = 2 * (quat.w * quat.z + quat.x * quat.y);
    float cos_yaw = 1 - 2 * (quat.y * quat.y + quat.z * quat.z);
    return  std::atan2(sin_yaw, cos_yaw); 
}
<<<<<<< HEAD

float sign_fcn(float value){
    if (value > 0) return 1;
    if (value < 0) return -1;
    return 0;
}

float NormAngle(float angle){
    while (angle <= -M_PI) angle += 2*M_PI;
    while (angle > M_PI) angle -= 2*M_PI;
    return angle;
}
float NormVector(geometry_msgs::Pose2D pose){
    return sqrt(pow(pose.x, 2) + pow(pose.y, 2));
}

//?
std::vector <float> Rot2D(geometry_msgs::Pose2D *pose, float angle){
    std::vector <float> new_pose = {0, 0};

    new_pose[0] = cos(angle)*pose->x  - sin(angle)*pose->y;
    new_pose[1] = sin(angle)*pose->x + sin(angle)*pose->y;

    return new_pose;


}
=======
>>>>>>> b45cb02c4b15ab5103ac7fe4cc7e88b829370475
