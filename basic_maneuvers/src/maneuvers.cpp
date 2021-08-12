<<<<<<< HEAD

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
=======
#include "maneuvers.hpp"


RobotManeuvers::Maneuvers::Maneuvers(float frw_vel = 0, float steer_ang = 0){
        SetInput(frw_vel, steer_ang);
}
RobotManeuvers::Maneuvers::~Maneuvers(){
    std::cout<<"No maneuvers for robot\n";
}

void RobotManeuvers::Maneuvers::SetInput(float frw_vel = 0, float steer_ang = 0){
        this->input.frw_vel = frw_vel;
        this->input.steer_ang = steer_ang;
}
RobotManeuvers::RobotInput RobotManeuvers::Maneuvers::GetInput(){
    return this->input;
}

geometry_msgs::Pose2D RobotManeuvers::Maneuvers::TransformSS(geometry_msgs::Pose2D cur_pose, geometry_msgs::Pose2D goal_pose, float angle){
    
    geometry_msgs::Pose2D transformed_cur_pose;
    transformed_cur_pose.x = cos(angle)*(goal_pose.x - cur_pose.x)  + sin(angle)*(goal_pose.y - cur_pose.y);
    transformed_cur_pose.y = -sin(angle)*(goal_pose.x - cur_pose.x) + cos(angle)*(goal_pose.y - cur_pose.y);
    transformed_cur_pose.theta = NormAngle(goal_pose.theta - cur_pose.theta);
    return transformed_cur_pose;

}
// Proportional controller for steering speed

float RobotManeuvers::Maneuvers::YawToSteerAng(float yawing_speed, float heading_speed, float cur_steering_angle, float C = 10){
    float target_angle = asin(yawing_speed*WHEELBASE/heading_speed);
    return  C*(target_angle - cur_steering_angle);
    /*
    float steering_angle = yawing_speed*WHEELBASE/heading_speed - sin(cur_steering_angle);
    steering_speed*= C;
    return steering_speed;
    */
}
bool RobotManeuvers::Maneuvers::PointToPoint(geometry_msgs::Pose2D goal_point, States::SteerbotState cur_state){

    geometry_msgs::Pose2D transformed_cur_pose =  TransformSS(cur_state.GetPose(), goal_point, cur_state.GetSteering().angle + cur_state.GetPose().theta);
    float magnitude  = sqrt(pow(transformed_cur_pose.x, 2) + pow(transformed_cur_pose.y,2));
    if (magnitude < DEVIATION){
        printf("Point is achieved!dx = %f, dy = %f \n", transformed_cur_pose.x, transformed_cur_pose.y);
        SetInput(0, 0);
        return true;
    }
    float K_P = 10;  // Proportional constant

    float heading_speed, yawing_speed;
    
    heading_speed = transformed_cur_pose.x; // v
    yawing_speed = transformed_cur_pose.y/magnitude; // w
    SetInput(heading_speed, YawToSteerAng(yawing_speed, heading_speed, sin(cur_state.GetSteering().angle), K_P));
    
    return false;
}

bool RobotManeuvers::Maneuvers::TurnAngle(float ref_angle, States::SteerbotState cur_state){

    float d_yaw = NormAngle(ref_angle - cur_state.GetPose().theta);
    if (abs(d_yaw) < DEVIATION/2){
        SetInput(0, 0);
        return true;
    }
    float K_P = 10; // Proportional constant
    float heading_speed, steering_speed;
    
    if(abs(abs(cur_state.GetSteering().angle) - M_PI) < DEVIATION/2 || GetInput().frw_vel != 0) heading_speed = 0.1;
    else heading_speed = 0;


    float steering_speed = K_P*d_yaw;
   
    SetInput(heading_speed, steering_speed);
    return false;
}

bool RobotManeuvers::Maneuvers::PathController(States::SteerbotState ref_state, States::SteerbotState cur_state){
    States::SteerbotState tmp_cur_state = cur_state;
    geometry_msgs::Pose2D transf_cur_pose =  TransformSS(cur_state.GetPose(), ref_state.GetPose(), cur_state.GetSteering().angle + cur_state.GetPose().theta);
    tmp_cur_state.SetPose(transf_cur_pose);
    float magnitude  = sqrt(pow(transf_cur_pose.x, 2) + pow(transf_cur_pose.y,2));
    if (magnitude < DEVIATION){
        printf("Point is achieved!dx = %f, dy = %f \n", transf_cur_pose.x, transf_cur_pose.y);
        SetInput(0, 0);
        return true;
    }

    float heading_speed, yawing_speed;
    float K_P_1 = 1; // P1 = P3
    float K_P_2 = 3; // P2
    float C = 10; // Proportional gain for steering angle


  
    // FeedForward input
    float u_r1 = ref_state.GetFrwSpeed();
    float u_r2 = ref_state.GetYawingSpeed();

    float v1 = -K_P_1*tmp_cur_state.GetPose().x;
    float v2 = -sign_fcn(u_r1)*K_P_2*tmp_cur_state.GetPose().y - K_P_1*tmp_cur_state.GetPose().theta;
    
    heading_speed = u_r1*cos(tmp_cur_state.GetPose().theta) - v1;
    yawing_speed = u_r2 - v2;

    SetInput(heading_speed, YawToSteerAng(yawing_speed, heading_speed, sin(cur_state.GetSteering().angle), C));
  
    return false;

}

bool RobotManeuvers::Maneuvers::PathControllerAcceleration(Paths::Trajectory ref_trajectory, States::SteerbotState cur_state){
  
    
    float magnitude  = sqrt(pow(ref_trajectory.pose.x - cur_state.GetPose().x, 2) + pow(ref_trajectory.pose.y - cur_state.GetPose().y, 2));
    if (magnitude < DEVIATION){
        printf("Point is achieved!dx = %f, dy = %f \n", ref_trajectory.pose.x - cur_state.GetPose().x, ref_trajectory.pose.y - cur_state.GetPose().y);
        SetInput(0, 0);
        return true;
    }
    // PD controller
    float P = 1;
    float D = 2;

    geometry_msgs::Pose2D  ddot_pose;
    geometry_msgs::Pose2D ddot_ref_pose;
    
    float C = 10;
    ddot_pose.x = ref_trajectory.ddot_pose.x + P*(ref_trajectory.pose.x - cur_state.GetPose().x) + D*(ref_trajectory.dot_pose.x - cur_state.GetDot_x());
    ddot_pose.y = ref_trajectory.ddot_pose.y + P*(ref_trajectory.pose.y - cur_state.GetPose().y) + D*(ref_trajectory.dot_pose.y - cur_state.GetDot_y());
    geometry_msgs::Pose2D transf_cur_pose =  TransformSS( ddot_ref_pose,  ddot_pose, cur_state.GetSteering().angle + cur_state.GetPose().theta);
    float a, w, v;
    a = transf_cur_pose.x;
    w = transf_cur_pose.y/cur_state.GetFrwSpeed();
    v += a*0.01;
    SetInput(v, YawToSteerAng(w, v, sin(cur_state.GetSteering().angle), C));
  



}
>>>>>>> 8a200bd8873c353568386cc66abc309758d39615

