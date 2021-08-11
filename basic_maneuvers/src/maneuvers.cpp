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

