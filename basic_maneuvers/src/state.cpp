
#include "state.hpp"

States::SteerbotState::SteerbotState(){
    geometry_msgs::Pose2D new_pose; new_pose.x = 0; new_pose.y = 0; new_pose.theta = 0;
    Steering new_steering; new_steering.angle = 0; new_steering.speed = 0;
    InitState(0, new_pose, steering, 0, 0); 
}

void States::SteerbotState::InitState(int index, geometry_msgs::Pose2D pose, Steering steering, float heading_speed, float time){
    SetIndex(index);
    SetPose(pose);
    SetSteering(steering);
    SetFrwSpeed(heading_speed);
    SetTime(time);


}
States::SteerbotState::~SteerbotState(){
    std::cout << "The state was deleted!\n";
}

geometry_msgs::Pose2D  States::SteerbotState::GetPose(){
        return this->pose;
}

void States::SteerbotState::SetPose(geometry_msgs::Pose2D  new_pose){
        this->pose.x =  new_pose.x;
        this->pose.y =  new_pose.y;
        this->pose.theta = new_pose.theta;
}

States::Steering States::SteerbotState::GetSteering(){
        return this->steering;

}

void States::SteerbotState::SetSteering(Steering new_steering){
        this->steering.angle = new_steering.angle;
        this->steering.speed = new_steering.speed;
}

float States::SteerbotState::GetFrwSpeed(){
        return this->heading_speed;

}

void States::SteerbotState::SetFrwSpeed(float new_frw_speed){
    this->heading_speed = new_frw_speed;
}

int States::SteerbotState::GetIndex(){
    return this->index;
}

void States::SteerbotState::SetIndex(int new_index){
    this->index = new_index;
} 

float States::SteerbotState::GetTime(){
    return this->time;
}

void States::SteerbotState::SetTime(float new_time){
    this->time = new_time;
}

float States::SteerbotState::GetYawingSpeed(){
    return (GetFrwSpeed()/WHEELBASE)*sin(GetSteering().angle);
}


float States::SteerbotState::GetDot_x(){
    return GetFrwSpeed()*cos(GetPose().theta + GetSteering().angle);
}          

float States::SteerbotState::GetDot_y(){
    return GetFrwSpeed()*sin(GetPose().theta + GetSteering().angle);
}