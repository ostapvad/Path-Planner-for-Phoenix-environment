#include "status_receiver.hpp"


PathViewer::Trajectory::Trajectory(ros::NodeHandle *node){
        initSubsribers(node);
 
        initClientsServices(node);
        this->robot_states.push_back(this->prev_state);

}

void PathViewer::Trajectory::initSubsribers(ros::NodeHandle *node){
    this->status = node->subscribe("/status", 1000, &Trajectory::statusCallback, this);
    this->odometry = node->subscribe("/odom", 1000, &Trajectory::odomCallback, this);
}

void PathViewer::Trajectory::initClientsServices(ros::NodeHandle *node){
    this->client_spawner = node->serviceClient<gazebo_msgs::SpawnModel>("/gazebo/spawn_sdf_model");
}



bool PathViewer::Trajectory::PushToStates(){
        States::RobotState last_pushed_state = this->robot_states.back();
     
        if (VectorNorm(last_pushed_state.dof.coord.x, last_pushed_state.dof.coord.y, this->prev_state.dof.coord.x, this->prev_state.dof.coord.y) >= this->devs.distance || abs(RadToDeg(last_pushed_state.dof.yaw - this->prev_state.dof.yaw))>=this->devs.orientation_difference){
         
            this->robot_states.push_back(this->prev_state);
            printf("x = %f, y = %f, yaw = %f\n", this->prev_state.dof.coord.x, this->prev_state.dof.coord.y, this->prev_state.dof.yaw);
            return true;
        }
           std::cout << "Not correct but\n-----------------------\n";
         printf("x = %f, y = %f, yaw = %f\n", this->prev_state.dof.coord.x, this->prev_state.dof.coord.y, this->prev_state.dof.yaw);
        return false;
}



void PathViewer::Trajectory::statusCallback(const phoenix_msgs::Status::ConstPtr &data){
    this->prev_state.wheels_speed.forward =data->currentForwardSpeed;
    this->prev_state.wheels_speed.turning = data->currentTurnAngle;

}
 
void PathViewer::Trajectory::odomCallback(const nav_msgs::Odometry::ConstPtr &data){
    this->prev_state.dof.coord.x = data->pose.pose.position.x;
    this->prev_state.dof.coord.y =  data->pose.pose.position.y;
    this->prev_state.dof.yaw =  QuatToYaw(data->pose.pose.orientation);
    std::cout <<"Came\n";
    if (PushToStates()){
        spawnState(this->robot_states.back(), created_models[0], "point_" + std::to_string(this->robot_states.size()));
        // SpawnPLane
    }

    
    

}
void PathViewer::Trajectory::spawnState(States::RobotState state, std::string pathToModel, std::string model_name){
        geometry_msgs::Pose new_pose;
        new_pose.position.x = state.dof.coord.x;
        new_pose.position.y = state.dof.coord.y;

        this->srv_spawner.request.model_name = model_name;
        this->srv_spawner.request.model_xml = readFileIntoString2(pathToModel);
        this->srv_spawner.request.initial_pose = new_pose;
        
            // Call spawner and show result
            if (this->client_spawner.call(this->srv_spawner) &&  this->client_spawner.exists())
            {// ROS_INFO("Response: %s\n", this->srv_spawner.status_message.c_str());}
            }
            else {ROS_ERROR("Failed to call service\n");}
            

}
