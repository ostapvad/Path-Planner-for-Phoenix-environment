#include "RRT.cpp"

#ifndef _STEERBOT_
#define _STEERBOT_




namespace SteerBot{    
    class Robot{
        private:
                ros::Subscriber status, odometry;
                ros::Publisher cmd_vel;
                geometry_msgs::Twist msg_vel;
               
        public:
               
                geometry_msgs::Pose2D ref_pose;
                Parameters::RobotState cur_state; 
                SimpleGraph::Graph RRT_tree;
                struct  timeval init_time;

                Robot(ros::NodeHandle *node);
                void initSubsribers(ros::NodeHandle *node);
                void initPublishers(ros::NodeHandle *node);
                void initParameters();
                void statusCallback(const phoenix_msgs::Status::ConstPtr &data);
                void odomCallback(const nav_msgs::Odometry::ConstPtr &data);
                void SetVelocity(float req_frw_vel, float target_angle);
               
		    
                bool PathController(Parameters::RobotState ref_state, Parameters::RobotState tmp_cur_state);
                Parameters::RobotState GenerateCircle();

    };

};
#endif
