
#include "ros/ros.h"
#include "phoenix_msgs/Status.h"
#include "nav_msgs/Odometry.h"
#include <cmath>

float QuatToYaw(const ::geometry_msgs::Quaternion quat){
    float sin_yaw = 2 * (quat.w * quat.z + quat.x * quat.y);
    float cos_yaw = 1 - 2 * (quat.y * quat.y + quat.z * quat.z);
    return  std::atan2(sin_yaw, cos_yaw); 
}
