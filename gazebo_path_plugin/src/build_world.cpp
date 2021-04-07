#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "gazebo_msgs/SpawnModel.h" 
#include <iostream>
#include <fstream>

int w_s = 4;    //[m] - width of moving path for the steerbot
int w_p = 2.5;  //[m] - width of parking slot
int l_p = 5.5;  //[m] - length of parking slot
int N = 10;     //[m] - number of parking slots in one row
std::string path_to_file = "/home/ostapvad/Documents/Gazebo_ws/src/gazebo_path_plugin/models/parking_slot/model.sdf"; // path to model

std::string readFileIntoString2(const std::string & path); // reads file into string
//void spawn_pslots();
//void call_spawn(const std_msgs::String::ConstPtr& model_name, int px, int py);


int main(int argc, char **argv){
    ros::init(argc, argv, "build_world_client");
    if(argc > 1){
        ROS_INFO("Incorrect usage of build_world. Return error");
        return 1;
    }
    ros::NodeHandle n;

    ros::ServiceClient client = n.serviceClient<gazebo_msgs::SpawnModel>("/gazebo/spawn_sdf_model");
    gazebo_msgs::SpawnModel srv;
    std::string model_name = "parking_slot";

    
    std::string xml_data = readFileIntoString2(path_to_file);
  
    std_msgs::Float64 px, py;
   
   
    std::string tmp_model_name;
    for (int i = 0; i < N; i++){
        tmp_model_name = model_name;
        tmp_model_name.append(std::to_string(i));
        px.data = w_s/2 + w_p*(N- i + 0.5);
        py.data = w_s/2 + l_p/2;
        srv.request.model_name = tmp_model_name;
        srv.request.model_xml = xml_data;
        //srv.request.robot_namespace = "-file";
        //srv.request.reference_frame = "parking_slot1";
        srv.request.initial_pose.position.x = px.data;
        srv.request.initial_pose.position.y = py.data;

        if(client.call(srv)){ 
            //std::cout<<srv.response.status_message<<" " << tmp_model_name <<std::endl;
            ROS_INFO("Response: %s, Model name: %s\n", srv.response.status_message.c_str(), tmp_model_name.c_str());
        }
        else {
            ROS_ERROR("Failed to call service");
            return 1;
        }
        
    }
    
    return 0;
 }


std::string readFileIntoString2(const std::string & path) {
    auto ss = std::ostringstream{};
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
             << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}

