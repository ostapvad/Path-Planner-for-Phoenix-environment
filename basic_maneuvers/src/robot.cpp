
#include "robot.hpp"

//enum States {STOP, MoveFW, TURN};
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
    this->cur_state.pose.x = default_x;  this->cur_state.pose.y = default_y;  this->cur_state.pose.theta = default_yaw;
    this->cur_state.heading_speed = 0; this->cur_state.steering_angle = 0;
    gettimeofday(&this->init_time, NULL);
   
}
            
// Communication callbacks
void SteerBot::Robot::statusCallback(const phoenix_msgs::Status::ConstPtr &data){
    this->cur_state.steering_angle = data->currentTurnAngle;
    this->cur_state.heading_speed = data->currentForwardSpeed;

}
            
void SteerBot::Robot::odomCallback(const nav_msgs::Odometry::ConstPtr &data){
    this->cur_state.pose.x = data->pose.pose.position.x;
    this->cur_state.pose.y = data->pose.pose.position.y;
    this->cur_state.pose.theta = QuatToYaw(data->pose.pose.orientation);    

}



void SteerBot::Robot::SetVelocity(float req_frw_vel, float target_angle){
    msg_vel.linear.x = req_frw_vel;
    msg_vel.angular.z = target_angle;
    this->cmd_vel.publish(msg_vel); 
   
}
/// n1 + n2 + n3 = 100
// 

bool SteerBot::Robot::PathController(Parameters::RobotState ref_state, Parameters::RobotState tmp_cur_state){
    float dx = ref_state.pose.x - tmp_cur_state.pose.x;
    float dy = ref_state.pose.y - tmp_cur_state.pose.y;
    float angle = tmp_cur_state.steering_angle + tmp_cur_state.pose.theta;
    Parameters::RobotState error_state;
    error_state.pose.x = cos(angle)*dx + sin(angle)*dy; 
    error_state.pose.y = -sin(angle)*dx + cos(angle)*dy;
    error_state.pose.theta = NormAngle(ref_state.pose.theta - tmp_cur_state.pose.theta);
    error_state.heading_speed = ref_state.heading_speed - tmp_cur_state.heading_speed;
    error_state.steering_angle = ref_state.steering_angle - tmp_cur_state.steering_angle;
    float sum_error = sqrt(pow(error_state.pose.x, 2) + pow(error_state.pose.y, 2) + pow( error_state.pose.theta , 2) + pow(  error_state.heading_speed, 2) + pow(error_state.steering_angle, 2));
    printf("e_x = %f, e_y = %f, e_yaw = %f, e_v = %f, e_psi = %f\n",  error_state.pose.x ,  error_state.pose.y,  error_state.pose.theta,   error_state.heading_speed, error_state.steering_angle );
    if (sum_error < 0.1){
        SetVelocity(0, 0);
        return true;
    }
    float target_v = ref_state.heading_speed*(cos(error_state.pose.theta)*cos(error_state.steering_angle) - sin(error_state.pose.theta)*sin(error_state.steering_angle)) + error_state.pose.x;
    float target_psi = ref_state.steering_angle + atan2(error_state.pose.y, error_state.pose.x) + error_state.pose.theta + target_v/abs(target_v)*error_state.pose.y;
    /*
    float target_v = ref_state.heading_speed + + error_state.pose.x;
    float target_psi = ref_state.steering_angle + atan2(error_state.pose.y,target_v+0.1)
    */
    SetVelocity(target_v, target_psi);
    printf("From Controller: v = %f, %f\n", target_v, target_psi);
    return false;


}

Parameters::RobotState SteerBot::Robot::GenerateCircle(){
    Parameters::RobotState state;

    struct  timeval  now;
	gettimeofday(&now, NULL);
	float iteration = now.tv_sec-this->init_time.tv_sec+(now.tv_usec-this->init_time.tv_usec)/1000000.0;
    float w = 2*M_PI/SAMPLING;
    float angle = w*iteration;
    state.pose.x = RADIUS*cos(angle);
    state.pose.y = RADIUS*sin(angle);
    state.heading_speed = RADIUS*w;
    state.steering_angle = asin(w/state.heading_speed*WHEELBASE);
    state.pose.theta = NormAngle(angle + M_PI/2 -state.steering_angle);
     printf("x_req = %f, y_req = %f, yaw_req = %f, v_req = %f, psi_rq = %f\n", state.pose.x ,  state.pose.y,  state.pose.theta,   state.heading_speed, state.steering_angle );
    return state;

}
