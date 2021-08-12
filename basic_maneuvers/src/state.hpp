
#include "libraries.hpp"

#ifndef _RobotSTATE_
#define _RobotSTATE_

#define WHEELBASE 4.2


// Robot State
namespace States{
    
    
    
    typedef struct{
        float angle, speed;
    }Steering;
    
    class SteerbotState{
        private:
            geometry_msgs::Pose2D pose;
            Steering steering;
            float heading_speed;
            int index;
            float time;

        public:
            SteerbotState();
            ~SteerbotState();
            void InitState(int index, geometry_msgs::Pose2D pose, Steering steering, float heading_speed, float time);
            geometry_msgs::Pose2D GetPose();
            void SetPose( geometry_msgs::Pose2D  new_pose);
            Steering GetSteering();
            void SetSteering(Steering new_steering);
            float GetFrwSpeed();
            void SetFrwSpeed(float new_frw_speed);
            int GetIndex();
            void SetIndex(int new_index);
            float GetTime();
            void SetTime(float new_time);
            float GetYawingSpeed();
            float GetDot_x();
            float GetDot_y();
    };
 
};

#endif