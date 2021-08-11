
#include "robot.hpp"

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
    
    /*  this->cur_state.pose.x = default_x;
    this->cur_state.pose.y = default_y;
    this->cur_state.pose.theta = default_yaw;
    this->cur_state.steering.angle = 0;
    this->cur_state.steering.speed = 0;
    this->cur_state.heading_speed = 0;
    //
    this->ref_pose.x = 0;
    this->ref_pose.y = 20;
    this->ref_pose.theta = 0;
    */
}
            
// Communication callbacks
void SteerBot::Robot::statusCallback(const phoenix_msgs::Status::ConstPtr &data){
    
    this->cur_state.heading_speed =data->currentForwardSpeed;
    this->cur_state.steering.angle = data->currentTurnAngle;

}
            
void SteerBot::Robot::odomCallback(const nav_msgs::Odometry::ConstPtr &data){
    this->cur_state.pose.x = data->pose.pose.position.x;
    this->cur_state.pose.y = data->pose.pose.position.y;
    this->cur_state.pose.theta = QuatToYaw(data->pose.pose.orientation);
    printf("(x, y) = (%f, %f), yaw = %f\n, psi = %f, v = %f \n", this->cur_state.pose.x, this->cur_state.pose.y, this->cur_state.pose.theta, this->cur_state.steering.angle, this->cur_state.heading_speed);
}



void SteerBot::Robot::SetVelocity(float req_frw_vel, float target_angle){
    msg_vel.linear.x = req_frw_vel;
    msg_vel.angular.z = target_angle;
    this->cmd_vel.publish(msg_vel); 
   
}


