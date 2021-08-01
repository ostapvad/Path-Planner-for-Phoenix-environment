#include "calculation.hpp"

#ifndef _STEERBOT_
#define _STEERBOT_

namespace SteerBot{
    namespace Parameters{
        typedef struct{
            geometry_msgs::Pose2D pose;
            struct Steering {
                float angle, speed;
            }steering;
            float heading_speed;


        }RobotState;
       
    };
    class Robot{
         private:
                ros::Subscriber status, odometry;
                ros::Publisher cmd_vel;
                geometry_msgs::Twist msg_vel;
            public:
                int CurState;
                bool setter =false;
                geometry_msgs::Pose2D ref_pose;
                Parameters::RobotState cur_state;
                std::vector<Parameters::RobotState> trajectory;
                Robot(ros::NodeHandle *node);
                void initSubsribers(ros::NodeHandle *node);
                void initPublishers(ros::NodeHandle *node);
                void initParameters();
                void statusCallback(const phoenix_msgs::Status::ConstPtr &data);
                void odomCallback(const nav_msgs::Odometry::ConstPtr &data);
                std::string GetCurrentState();
                void SetVelocity(float req_omega, float req_alpha);
                bool PathController(geometry_msgs::Pose2D ref_pose, Parameters::RobotState tmp_cur_state);
                void GenerateCircle(float radius, float time_step, geometry_msgs::Pose2D init_pose);
                void TrajectoryFollower();
                

    };

};
#endif
/*
ToDo LIST!

1) Implement a simple database, that can be readed or save the generated trajectories in csv format.
2) Enable to make a simple maneuver from database: if point to point, then a database is two points.
3) Add more trajectories generation.
4) Connect to distant PC.

*/