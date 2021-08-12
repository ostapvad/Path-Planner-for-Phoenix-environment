#include <cmath>
#include <fstream>
#include "nav_msgs/Odometry.h"
#include "phoenix_msgs/Status.h"
#include "nav_msgs/Odometry.h"
#include "gazebo_msgs/SpawnModel.h" 




float QuatToYaw(const ::geometry_msgs::Quaternion quat){
    float sin_yaw = 2 * (quat.w * quat.z + quat.x * quat.y);
    float cos_yaw = 1 - 2 * (quat.y * quat.y + quat.z * quat.z);
    return  std::atan2(sin_yaw, cos_yaw); 
}
float VectorNorm(float x1, float y1, float x2, float y2){
    return sqrt(pow(x2 -x1, 2) + pow(y2-y1, 2));
}

float RadToDeg(float angle_rad){
    return angle_rad*180/M_PI;

}

/*Reads the file content in the string */
std::string readFileIntoString2(const std::string & str_path) { 
    auto ss = std::ostringstream{};
    //std::filesystem::path;

    std::ifstream input_file(str_path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
             << str_path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}
