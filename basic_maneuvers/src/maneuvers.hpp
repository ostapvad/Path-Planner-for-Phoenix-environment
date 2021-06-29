#include "calculation.hpp"

#ifndef _STEERBOT_
#define _STEERBOT_

namespace SteerBot{
    namespace Parameters{
        typedef struct {
            struct Coord{
                float x, y;
            }coord;
            float yaw; 
        }RobotPose;

        typedef struct{
            float omega, alpha; // linear speed, wheels angle;
            struct Published
            {
                float linear, angular = 0;
            }pub_wheels;
            
        }Wheels;
    };
    class Robot{
         private:
                ros::Subscriber status, odometry;
                ros::Publisher cmd_vel;
                geometry_msgs::Twist msg_vel;
            public:
                int CurState;
                Parameters::RobotPose pose;
                Parameters::Wheels wheels;
                Robot(ros::NodeHandle *node);
                void initSubsribers(ros::NodeHandle *node);
                void initPublishers(ros::NodeHandle *node);
                void initParameters();
                void statusCallback(const phoenix_msgs::Status::ConstPtr &data);
                void odomCallback(const nav_msgs::Odometry::ConstPtr &data);
                std::string GetCurrentState();
                void SetVelocity(float req_omega, float req_alpha);

    };

};
#endif