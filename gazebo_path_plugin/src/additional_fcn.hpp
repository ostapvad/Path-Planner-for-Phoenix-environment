#include "ros/ros.h"
#include <iostream>
#include <fstream>
#include "geometry_msgs/Quaternion.h"
#include <cmath>



std::string readFileIntoString2(const std::string & path) { // To read the file into the string
    auto ss = std::ostringstream{};
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
             << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}
void RotZToQuat(geometry_msgs::Quaternion & new_pose, float angle_deg){ // To calculate rotation of object around the z axis 

 
  new_pose.w = sqrt(2 + 2*cos(angle_deg*M_PI/180.0))/2;
  new_pose.x = new_pose.y = 0;
  new_pose.z = sin(angle_deg* M_PI/180.0)/(2*new_pose.w);

}
