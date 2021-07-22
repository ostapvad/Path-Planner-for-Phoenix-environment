
#include "maneuvers.hpp"

enum States {STOP, MoveFW, TURN};
float default_x, default_y, default_yaw = 0;


SteerBot::Robot::Robot(ros::NodeHandle *node){
        initSubsribers(node);
        initPublishers(node);
        initParameters();
}
         
            
// Init communication with Robot
void SteerBot::Robot::initSubsribers(ros::NodeHandle *node){
        this->status = node->subscribe("/status", 1000, &Robot::statusCallback, this);
        this->odometry = node->subscribe("/odom", 1000, &Robot::odomCallback, this);

}
            
void SteerBot::Robot::initPublishers(ros::NodeHandle *node){
    this->cmd_vel = node->advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
}
            
void SteerBot::Robot::initParameters(){
    this->pose.coord.x =  default_x;
    this->pose.coord.y = default_y;
    this->pose.yaw =  default_yaw;
    this->wheels.omega = 0;
    this->wheels.alpha = 0;
    this->CurState = STOP;
}
            
// Communication callbacks
void SteerBot::Robot::statusCallback(const phoenix_msgs::Status::ConstPtr &data){
    this->wheels.omega = data->currentForwardSpeed;
    this->wheels.alpha = data->currentTurnAngle;
    
}
            
void SteerBot::Robot::odomCallback(const nav_msgs::Odometry::ConstPtr &data){
    this->pose.coord.x = data->pose.pose.position.x;
    this->pose.coord.y = data->pose.pose.position.y;
    this->pose.yaw = QuatToYaw(data->pose.pose.orientation);
}

std::string SteerBot::Robot::GetCurrentState(){
    switch (this->CurState)
    {
        case STOP:
            return "State = 0. Robot is stopped\n";
        case MoveFW:
            return "State = 1. Robot is moving forward\n";
        case TURN:
            return "State = 2. Robot is turning\n";
        default:
            return "State is UNKNOWN!\n";
    }
    
}

void SteerBot::Robot::SetVelocity(float req_omega, float req_alpha){
    msg_vel.linear.x = req_omega;
    msg_vel.angular.z = req_alpha;
    wheels.pub_wheels.linear, wheels.pub_wheels.angular = req_omega, req_alpha;
    this->cmd_vel.publish(msg_vel); 
    std::cout<<"linear = " << msg_vel.linear.x << "angular = " << msg_vel.angular.z << std::endl;
}



