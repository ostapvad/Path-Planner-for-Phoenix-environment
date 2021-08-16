#include "libraries.hpp"

#ifndef _STEERBOT_
#define _STEERBOT_
#define NULL __null
#define SAMPLING 100
#define RADIUS 10
#define WHEELBASE 4.2

namespace SteerBot{

    namespace Parameters{
        typedef struct{
            geometry_msgs::Pose2D pose;
            float steering_angle;
            float heading_speed;


        }RobotState;
       
    };
    
    class Robot{
         private:
                ros::Subscriber status, odometry;
                ros::Publisher cmd_vel;
                geometry_msgs::Twist msg_vel;
               
            public:
               
                //geometry_msgs::Pose2D ref_pose;
                Parameters::RobotState cur_state; 
                Robot(ros::NodeHandle *node);
                void initSubsribers(ros::NodeHandle *node);
                void initPublishers(ros::NodeHandle *node);
                void initParameters();
                void statusCallback(const phoenix_msgs::Status::ConstPtr &data);
                void odomCallback(const nav_msgs::Odometry::ConstPtr &data);
                void SetVelocity(float req_frw_vel, float target_angle);
                struct  timeval init_time;
		    
                bool PathController(Parameters::RobotState ref_state, Parameters::RobotState tmp_cur_state);
                Parameters::RobotState GenerateCircle();
                /*
                bool NaiveController(geometry_msgs::Pose2D ref_state, Parameters::RobotState tmp_cur_state);
                bool TurnAngle(float yaw_d, Parameters::RobotState tmp_cur_state);
                */
    };

};
#endif
