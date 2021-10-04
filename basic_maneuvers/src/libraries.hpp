#include "ros/ros.h"
#include "phoenix_msgs/Status.h"
#include "geometry_msgs/Pose2D.h"
#include "nav_msgs/Odometry.h"
#include <tf/transform_datatypes.h>
#include <cmath>
#include <sys/time.h> 
#include <random>

#define SpeedRAMP 0.2
#define RotRAMP 0.34
#define MAXFrwSPEED 1.0
#define MAXSteerANGLE M_PI/2.0
#define WHEELBASE 4.2
#define SAMPLING 100
#define RADIUS 10


#define NULL __null

namespace Parameters{
    typedef struct{
            geometry_msgs::Pose2D pose;
            float steering_angle;
            float heading_speed;


    }RobotState;
   
};

float Dist(float dx, float dy, float dtheta = 0){
        return sqrt(pow(dx, 2) + pow(dy, 2) + pow(dtheta, 2));
}


float NormAngle(float angle){
    while (angle <= -M_PI) angle += 2*M_PI;
    while (angle > M_PI) angle -= 2*M_PI;
    return angle;
}

int sign_fcn(float value){
    if(value != 0) return value/abs(value);
    return 0;
}

float Ramp(float input, float max_value){
    if(abs(input) > max_value) return sign_fcn(input)*max_value;
    return input;   
}

float QuatToYaw(const ::geometry_msgs::Quaternion quat){
    tf::Quaternion q(quat.x, quat.y, quat.z, quat.w);
    tf::Matrix3x3 m(q);
	double roll, pitch, yaw;
	m.getRPY(roll, pitch, yaw);
   return yaw; 
}

float GenerateRandNum(){
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}


float RandomSample(float start, float end, int NumSamples, int RandSample){
    if (RandSample > NumSamples) return start;
    
    float dv = abs(end -start)/NumSamples;
    return start + dv*RandSample;
}
bool EqualFloats(float f1, float f2){
    if(abs(f1 - f2) <= 0.00001)return true;
    return false;
}

template<typename T>
T RandomDistribution(T range_from, T range_to) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<T>   distr(range_from, range_to);
    return distr(generator);
}
  /*
            if(counter < new_search.tree.nodes.size()){
            
                    points.header.frame_id = line_strip.header.frame_id = line_list.header.frame_id = "/my_frame";
                    points.header.stamp = line_strip.header.stamp = line_list.header.stamp = ros::Time::now();
                    points.ns = line_strip.ns = line_list.ns = "points_and_lines";
                    points.action = line_strip.action = line_list.action = visualization_msgs::Marker::ADD;
                    points.pose.orientation.w = line_strip.pose.orientation.w = line_list.pose.orientation.w = 1.0;
  
                    points.id = 0;
                    line_strip.id = 1;
                    line_list.id = 2;
   
                
                    points.type = visualization_msgs::Marker::POINTS;
                    line_strip.type = visualization_msgs::Marker::LINE_STRIP;
                    line_list.type = visualization_msgs::Marker::LINE_LIST;
        
            
                    // POINTS markers use x and y scale for width/height respectively
                    points.scale.x = 0.5;
                    points.scale.y = 0.5;
                    points.scale.z = 0.5;
            
                    // LINE_STRIP/LINE_LIST markers use only the x component of scale, for the line width
                    line_strip.scale.x = 0.1;
                    line_list.scale.x = 0.1;

                   
                    points.color.g = 1.0f;
                    points.color.a = 1.0;
                   
                    // Line strip is blue
                    line_strip.color.b = 1.0;
                    line_strip.color.a = 1.0;
                
                    // Line list is red
                    line_list.color.r = 1.0;
                    line_list.color.a = 1.0;
                    geometry_msgs::Point p;
    
                    p.x = new_search.tree.nodes[counter].state.pose.x*10;
                    p.y = new_search.tree.nodes[counter].state.pose.y*10;
                    p.z = new_search.tree.nodes[counter].state.pose.theta*10;

                    
               
                    points.points.push_back(p);
                    line_strip.points.push_back(p);
                    counter++;
                    marker_pub.publish(points);
            //marker_pub.publish(line_strip);
            
            }
            */