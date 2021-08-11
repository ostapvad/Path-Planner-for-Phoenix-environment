#include "state.hpp"

#ifndef _TrajGenerator_
#define _TrajGenerator_

// DO as parameters
#define SAMPLING 100
#define RADIUS 10
#define Ellipse_a 10
#define Ellipse_b 15
#define LINE_k 5
#define LINE_b 10
#define POSE_X 0
#define POSE_Y 0
#define FOCAL_DISTANCE 10

enum TestModes {CIRCLE, ELLIPSE, LINE, BERNOULLI, POLYNOMIAL};

namespace Paths{
    typedef struct{ // FINISH 
            geometry_msgs::Pose2D pose;
            geometry_msgs::Pose2D dot_pose;
            geometry_msgs::Pose2D ddot_pose;
    }Trajectory;

    class Generator{
        private:
            float Period;
            float CurTime;
            int index = 0;
            Trajectory trajectory;
            States::SteerbotState robot_state;
            void TrajToState();
            void SetTrajAngles();
        public:
            Generator();
            ~Generator();
            void SetPeriod(float new_period);
            float GetPeriod();
            void SetCurTime(float new_time);
            float GetCurTime();
            void SetIndex(int new_index);
            int GetIndex();
            Trajectory GetTrajectory();
            States::SteerbotState GetRobotState();
            void GenerateTrajectory(int trajectory_type, int new_index = 0); 
          
            void Circle(geometry_msgs::Pose2D circle_pose, float Radius = RADIUS);
            void Ellipse(geometry_msgs::Pose2D ellipse_pose, float a = Ellipse_a, float b = Ellipse_b);
            void BernoulliLemniscate(float focal_distance = 10);
            void Line(float k=LINE_k, float b=LINE_b);
            void Polynomial(std::vector<float>coefficients = {0});
            
    };
 
};

#endif