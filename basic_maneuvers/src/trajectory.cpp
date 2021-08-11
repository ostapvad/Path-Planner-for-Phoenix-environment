#include "trajectory.hpp"


Paths::Generator::Generator(){
    SetPeriod(1/SAMPLING);
}


Paths::Generator::~Generator(){
    std::cout << "Trajectory generator was deleted\n";
}

void Paths::Generator::SetPeriod(float new_period){
    this->Period = new_period;
}

float Paths::Generator::GetPeriod(){
    return this->Period;
} 

void Paths::Generator::SetCurTime(float new_time){
    this->CurTime = new_time;
}

float Paths::Generator::GetCurTime(){
    return this->CurTime;
}

void Paths::Generator::SetIndex(int new_index){
    this->index = new_index;
}
int Paths::Generator::GetIndex(){
    return this->index;
}

Paths::Trajectory Paths::Generator::GetTrajectory(){
    return this->trajectory;

}

States::SteerbotState Paths::Generator::GetRobotState(){
    return this->robot_state;

}

void Paths::Generator::TrajToState(){
    float w = 2*M_PI/this->Period;
    float t = this->CurTime;
    geometry_msgs::Pose2D robot_pose; robot_pose.x = this->trajectory.pose.x; robot_pose.y = this->trajectory.pose.y; 
    float angle_sum = this->trajectory.pose.theta;
  
    
    float v = sqrt(pow(this->trajectory.dot_pose.x, 2) + pow(this->trajectory.dot_pose.y, 2));// Consider the forward movement {0, 1}
    States::Steering new_steering = {asin(w*WHEELBASE/v), -1}; // only for [-PI/2, PI/2]
    robot_pose.theta =   NormAngle(angle_sum - new_steering.angle);
    // Calculating steering speed
    float sum_dot_angles = (-sin(angle_sum*this->trajectory.ddot_pose.x + cos(angle_sum)*this->trajectory.ddot_pose.y))/v; // v != 0 
    new_steering.speed = sum_dot_angles - w;
    this->robot_state.InitState(this->index, robot_pose, new_steering, v, t);
}
void Paths::Generator::SetTrajAngles(){
    this->trajectory.pose.theta = atan2(this->trajectory.dot_pose.y,this->trajectory.dot_pose.x);
    this->trajectory.dot_pose.theta = 2*M_PI/this->Period;
    this->trajectory.ddot_pose.theta = 0;

}



void Paths::Generator::Circle(geometry_msgs::Pose2D circle_pose, float Radius = RADIUS){
        float w = 2*M_PI/this->Period;
        float t = this->CurTime;
        this->trajectory.pose.x =  circle_pose.x + Radius*cos(w*t); this->trajectory.pose.y =  circle_pose.y + Radius*sin(w*t);
        this->trajectory.dot_pose.x = -w*Radius*sin(w*t); this->trajectory.dot_pose.y = w*Radius*cos(w*t);
        this->trajectory.ddot_pose.x = -Radius*cos(w*t)*pow(w,2); this->trajectory.ddot_pose.y = -Radius*sin(w*t)*pow(w,2);
        
        SetTrajAngles();
        TrajToState();
}


void Paths::Generator::Ellipse(geometry_msgs::Pose2D ellipse_pose, float a = Ellipse_a, float b = Ellipse_b){
    
    float w = 2*M_PI/this->Period;
    float t = this->CurTime;

    this->trajectory.pose.x =   ellipse_pose.x + a*cos(w*t); this->trajectory.pose.y = ellipse_pose.y + b*sin(w*t);
    this->trajectory.dot_pose.x = -a*sin(w*t)*w;  this->trajectory.dot_pose.y =  b*cos(w*t)*w;
    this->trajectory.ddot_pose.x =  -a*cos(w*t)*pow(w,2); this->trajectory.ddot_pose.y = -b*sin(w*t)*pow(w,2);

    SetTrajAngles();
    TrajToState();

}

void Paths::Generator::BernoulliLemniscate(float focal_distance = 10){
    
    
    float a = sqrt(2)*focal_distance; // parameter of curve
    
    float w = 2*M_PI/this->Period;
    float t = this->CurTime;

    this->trajectory.pose.x = (a*cos(w*t))/(1 + pow(sin(w*t),2)); this->trajectory.pose.y = (a*sin(w*t)*cos(w*t))/(1 + pow(sin(w*t),2));
    this->trajectory.dot_pose.x = a*(-w*sin(w*t))/(1 + pow(sin(w*t),2))  - (2*w*pow(cos(w*t),2)*sin(w*t))/pow(1 + pow(sin(w*t),2),2); this->trajectory.dot_pose.y = a*(2*w*(3*cos(2*w*t) -1))/pow(cos(2*w*t)-3, 2);
    this->trajectory.ddot_pose.x = -a*(pow(w,2)*cos(w*t)*(pow(cos(w*t), 2) + 10*pow(cos(w*t), 2) -8))/pow(1 + pow(sin(w*t),2),3); this->trajectory.ddot_pose.y = -a*(4*pow(w,2)*(3*cos(2*w*t) + 7)*sin(2*w*t))/pow(cos(2*w*t)-3, 3);
    
    SetTrajAngles();
    TrajToState();
}

void Paths::Generator::Line(float k=LINE_k, float b=LINE_b){
    
    float w = 2*M_PI/this->Period; // ? 
    float t = this->CurTime;

    this->trajectory.pose.x = w*t; this->trajectory.pose.y =  pow(w,2)*k*t + b;
    this->trajectory.dot_pose.x = w; this->trajectory.dot_pose.y = pow(w,2)*k;
    this->trajectory.ddot_pose.x = 0; this->trajectory.ddot_pose.y = 0;
    
    SetTrajAngles();
    TrajToState();
}
 // Not implemented
void Paths::Generator::Polynomial(std::vector<float>coefficients = {0}){

    this->trajectory.pose.x = 0; this->trajectory.pose.y =  0;
    this->trajectory.dot_pose.x = 0; this->trajectory.dot_pose.y = 0;
    this->trajectory.ddot_pose.x = 0; this->trajectory.ddot_pose.y = 0;
    
    SetTrajAngles();
    TrajToState();
        
     

}

void Paths::Generator::GenerateTrajectory(int trajectory_type, int new_index = 0){
    
    geometry_msgs::Pose2D pose; pose.x = POSE_X; pose.y = POSE_Y; pose.theta = 0;
    SetIndex(new_index);
    switch (trajectory_type)
    {
    case CIRCLE:
        return Circle(pose);
    case ELLIPSE:
        return Ellipse(pose);
    case BERNOULLI:
        return BernoulliLemniscate(FOCAL_DISTANCE);
    case LINE:
        return Line();
    case POLYNOMIAL: // Not implemented
        return Polynomial();
    default:
        return Circle(pose);
    }

}