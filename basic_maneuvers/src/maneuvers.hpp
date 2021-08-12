#include "calculation.hpp"

#ifndef _STEERBOT_
#define _STEERBOT_

namespace SteerBot{
    namespace Parameters{
<<<<<<< HEAD
        typedef struct{
            geometry_msgs::Pose2D pose;
            struct Steering {
                float angle, speed;
            }steering;
            float heading_speed;


        }RobotState;
       
=======
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
>>>>>>> b45cb02c4b15ab5103ac7fe4cc7e88b829370475
    };
    class Robot{
         private:
                ros::Subscriber status, odometry;
                ros::Publisher cmd_vel;
                geometry_msgs::Twist msg_vel;
            public:
                int CurState;
<<<<<<< HEAD
                bool setter =false;
                geometry_msgs::Pose2D ref_pose;
                Parameters::RobotState cur_state;
                std::vector<Parameters::RobotState> trajectory;
=======
                Parameters::RobotPose pose;
                Parameters::Wheels wheels;
>>>>>>> b45cb02c4b15ab5103ac7fe4cc7e88b829370475
                Robot(ros::NodeHandle *node);
                void initSubsribers(ros::NodeHandle *node);
                void initPublishers(ros::NodeHandle *node);
                void initParameters();
                void statusCallback(const phoenix_msgs::Status::ConstPtr &data);
                void odomCallback(const nav_msgs::Odometry::ConstPtr &data);
                std::string GetCurrentState();
                void SetVelocity(float req_omega, float req_alpha);
<<<<<<< HEAD
                bool PathController(geometry_msgs::Pose2D ref_pose, Parameters::RobotState tmp_cur_state);
                void GenerateCircle(float radius, float time_step, geometry_msgs::Pose2D init_pose);
                void TrajectoryFollower();
                
=======
>>>>>>> b45cb02c4b15ab5103ac7fe4cc7e88b829370475

    };

};
<<<<<<< HEAD
#endif
/*
ToDo LIST!

1) Implement a simple database, that can be readed or save the generated trajectories in csv format.
2) Enable to make a simple maneuver from database: if point to point, then a database is two points.
3) Add more trajectories generation.
4) Connect to distant PC.

*/
=======
#endif
>>>>>>> b45cb02c4b15ab5103ac7fe4cc7e88b829370475
