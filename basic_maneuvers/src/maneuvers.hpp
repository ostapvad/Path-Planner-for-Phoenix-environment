
#include "trajectory.hpp"

#ifndef _ROBOTManeuvers_
#define _ROBOTManeuvers_

#define DEVIATION 0.2

namespace RobotManeuvers{
    typedef struct{
        float frw_vel, steer_ang;
    }RobotInput;

    class Maneuvers{
        private:
            RobotInput input;
            geometry_msgs::Pose2D TransformSS(geometry_msgs::Pose2D cur_pose, geometry_msgs::Pose2D goal_state, float angle);
            float YawToSteerAng(float yawing_speed, float heading_speed, float cur_steering_angle, float C = 10);
        public:
           Maneuvers(float frw_vel = 0, float steer_ang = 0);
           ~Maneuvers();
           void SetInput(float frw_vel = 0, float steer_ang = 0);
           RobotInput GetInput();
           bool PointToPoint(geometry_msgs::Pose2D goal_point, States::SteerbotState cur_state);
           bool PathController(States::SteerbotState ref_state, States::SteerbotState cur_state);
           bool PathControllerAcceleration(Paths::Trajectory ref_trajectory, States::SteerbotState cur_state);
           bool TurnAngle(float ref_angle, States::SteerbotState cur_state);
          
        
        
    };
 
};

#endif