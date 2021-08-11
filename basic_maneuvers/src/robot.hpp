#include "maneuvers.hpp"

#ifndef _STEERBOT_
#define _STEERBOT_

namespace SteerBot{
    /*
    namespace Parameters{
        typedef struct{
            geometry_msgs::Pose2D pose;
            struct Steering {
                float angle, speed;
            }steering;
            float heading_speed;


        }RobotState;
       
    };
    */
    class Robot{
         private:
                ros::Subscriber status, odometry;
                ros::Publisher cmd_vel;
                geometry_msgs::Twist msg_vel;
                States::SteerbotState cur_state; // ?
            public:
               
                //geometry_msgs::Pose2D ref_pose;
                //Parameters::RobotState cur_state;
                
                Robot(ros::NodeHandle *node);
                void initSubsribers(ros::NodeHandle *node);
                void initPublishers(ros::NodeHandle *node);
                void initParameters();
                void statusCallback(const phoenix_msgs::Status::ConstPtr &data);
                void odomCallback(const nav_msgs::Odometry::ConstPtr &data);
                void SetVelocity(float req_frw_vel, float target_angle);
                /*
                std::string GetCurrentState();
              
                bool PathController(geometry_msgs::Pose2D ref_state, Parameters::RobotState tmp_cur_state);
                bool NaiveController(geometry_msgs::Pose2D ref_state, Parameters::RobotState tmp_cur_state);
                bool TurnAngle(float yaw_d, Parameters::RobotState tmp_cur_state);
                */
    };

};
#endif
