
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
<<<<<<< HEAD
    this->cur_state.pose.x = default_x;
    this->cur_state.pose.y = default_y;
    this->cur_state.pose.theta = default_yaw;
    this->cur_state.steering.angle = 0;
    this->cur_state.steering.speed = 0;
    this->cur_state.heading_speed = 0;
    this->CurState = STOP;
    //
    this->ref_pose.x = 0;
    this->ref_pose.y = 20;
    this->ref_pose.theta = 0;

    geometry_msgs::Pose2D init_pose;
    init_pose.x = 0;
    init_pose.y = 20;
    GenerateCircle(20, 100, init_pose);
=======
    this->pose.coord.x =  default_x;
    this->pose.coord.y = default_y;
    this->pose.yaw =  default_yaw;
    this->wheels.omega = 0;
    this->wheels.alpha = 0;
    this->CurState = STOP;
>>>>>>> b45cb02c4b15ab5103ac7fe4cc7e88b829370475
}
            
// Communication callbacks
void SteerBot::Robot::statusCallback(const phoenix_msgs::Status::ConstPtr &data){
<<<<<<< HEAD
    
    this->cur_state.heading_speed =data->currentForwardSpeed;
    this->cur_state.steering.angle = data->currentTurnAngle;

}
            
void SteerBot::Robot::odomCallback(const nav_msgs::Odometry::ConstPtr &data){
    this->cur_state.pose.x = data->pose.pose.position.x;
    this->cur_state.pose.y = data->pose.pose.position.y;
    this->cur_state.pose.theta = QuatToYaw(data->pose.pose.orientation);
    
    //printf("(x, y) = (%f, %f), yaw = %f\n", this->cur_state.pose.x, this->cur_state.pose.y, this->cur_state.pose.theta);
    //PathController(this->ref_pose, this->cur_state);
=======
    this->wheels.omega = data->currentForwardSpeed;
    this->wheels.alpha = data->currentTurnAngle;
    
}
            
void SteerBot::Robot::odomCallback(const nav_msgs::Odometry::ConstPtr &data){
    this->pose.coord.x = data->pose.pose.position.x;
    this->pose.coord.y = data->pose.pose.position.y;
    this->pose.yaw = QuatToYaw(data->pose.pose.orientation);
>>>>>>> b45cb02c4b15ab5103ac7fe4cc7e88b829370475
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

<<<<<<< HEAD
void SteerBot::Robot::SetVelocity(float heading_speed, float steering_speed){
    msg_vel.linear.x =heading_speed;
    msg_vel.angular.z = steering_speed;
  
 
    this->cmd_vel.publish(msg_vel); 
   
}
void SteerBot::Robot::GenerateCircle(float radius, float time_step, geometry_msgs::Pose2D init_pose){

    Parameters::RobotState new_state;
    float sampling_frequence = 2*M_PI/time_step;
    
    for (int k=0 ; k<=time_step; k++){
       
       new_state.pose.x = init_pose.x + radius*cos(sampling_frequence*k);
       new_state.pose.y = init_pose.y + radius*sin(sampling_frequence*k);
       new_state.pose.theta = atan2(new_state.pose.y, new_state.pose.x);
       this->trajectory.push_back(new_state); 

    }
}
void SteerBot::Robot::TrajectoryFollower(){
       // ADD SMTH


}

bool SteerBot::Robot::PathController(geometry_msgs::Pose2D  ref_state, Parameters::RobotState tmp_cur_state){
    
    

    geometry_msgs::Pose2D tmp_direct_vector, direct_vector;
    tmp_direct_vector.x = ref_state.x - tmp_cur_state.pose.x;
    tmp_direct_vector.y =  ref_state.y - tmp_cur_state.pose.y;
    tmp_direct_vector.theta = NormAngle(tmp_cur_state.steering.angle + tmp_cur_state.pose.theta);
    if (abs(tmp_direct_vector.x) < 0.1 && abs(tmp_direct_vector.y)<0.1){ // Position was founded!
        printf("Point is achieved!dx = %f, dy = %f \n", tmp_direct_vector.x, tmp_direct_vector.y);
        SetVelocity(0,0);
        return true;
    }
    
    float K_P = 10;
    float heading_speed, steering_speed, yawing_speed;
    
    // Transformation
    direct_vector.x = cos(tmp_direct_vector.theta)*tmp_direct_vector.x  + sin(tmp_direct_vector.theta)*tmp_direct_vector.y;
    direct_vector.y =  -sin(tmp_direct_vector.theta)*tmp_direct_vector.x  + cos(tmp_direct_vector.theta)*tmp_direct_vector.y;
    direct_vector.theta = tmp_direct_vector.theta ;
    
    
    
    heading_speed = direct_vector.x;
    yawing_speed = direct_vector.y/NormVector(direct_vector);
    steering_speed = yawing_speed*NormVector(direct_vector)/heading_speed - sin(tmp_cur_state.steering.angle);
    steering_speed*= K_P;
    

    /*
    heading_speed = NormVector(tmp_direct_vector)*0.5;
    steering_speed = K_P*NormAngle(atan2(tmp_direct_vector.y, tmp_direct_vector.x) - tmp_direct_vector.theta);
    */

    //printf("frw = %f, ang_sp =%f, st_angle = %f\n", heading_speed, steering_speed, tmp_cur_state.steering.angle);
    SetVelocity(heading_speed, steering_speed);
    return false;
}
=======
void SteerBot::Robot::SetVelocity(float req_omega, float req_alpha){
    msg_vel.linear.x = req_omega;
    msg_vel.angular.z = req_alpha;
    wheels.pub_wheels.linear, wheels.pub_wheels.angular = req_omega, req_alpha;
    this->cmd_vel.publish(msg_vel); 
    std::cout<<"linear = " << msg_vel.linear.x << "angular = " << msg_vel.angular.z << std::endl;
}


>>>>>>> b45cb02c4b15ab5103ac7fe4cc7e88b829370475

