#include "parameters.hpp"
#include "addition_fcn.hpp"
#include <vector>
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"

//#include "tf2/LinearMath/Quaternion.h"
//#include "geometry_msgs/Pose.h"

#include "gazebo_msgs/SpawnModel.h" 


std::string path_to_file = "/home/ostapvad/Documents/Gazebo_ws/src/gazebo_path_plugin/models/parking_slot/model.sdf"; // path to model




namespace SpawnedWorld{
class World {
    public: 

        int w_s = 4;    //[m] - width of moving path for the steerbot
        float w_p = 2.5;  //[m] - width of parking slot
        float l_p = 5.5;  //[m] - length of parking slot
        int N = 20;     //[m] - number of parking slots in one row
        float line_width = 0.1; // width of parking white line
        std::vector<void*> spawned_objects;  // pointers to spawned objects
        std::vector<std::string> models_names = {"p_s", "car"}; // default models names

        World(int argc, char **argv){

            ros::init(argc, argv, "spawn_world_client");
        }
        void ShowMap(){
            std::cout << "Must return some map\n";

        }
        bool RemoveObject(int id, int sub_id){
            std::cout << "The required object wasn't removed\n";
           return false;
        }
        void ShowObjectProperties(){(w_p - line_width);
            std::cout << "Shows properties of the object\n";
        }
        bool SpawnParkingBox(int id){
                // Write separate function for calling other service!
                ros::NodeHandle n;
                ros::ServiceClient client = n.serviceClient<gazebo_msgs::SpawnModel>("/gazebo/spawn_sdf_model");
                gazebo_msgs::SpawnModel srv;
                

                // find better way to transform to geometry msgs
                //tf2::Quaternion new_orientation;
                //new_orientation.setRPY(0, 0, 1.57);
                std::string xml_data = readFileIntoString2(path_to_file);
                
                // Top Right parking slot of parking box
                float init_x, init_y;
                init_x = 10;
                init_y = 20;

                std_msgs::Float64 px, py;
                std::string tmp_model_name, model_name;
                model_name = "id: ";
                model_name += std::to_string(id);

            for (int i = 0; i < N; i++){
            
        
                float tmp_z = -0.7071068;
                for (int j = 0; j < 2; j++) // number of rotation
                {   
                    tmp_model_name = model_name;

                    tmp_model_name += " r: " + std::to_string(i) +" c: " + std::to_string(j);
                    
                    //TODO! CHANGE THAT PART OF CODE
                    px.data = init_x -  i*(w_p - line_width);
                    py.data = init_y  - j*(l_p - line_width);
                    srv.request.initial_pose.orientation.z = tmp_z;
              
                    //tmp_model_name.append(std::to_string(j));
                    tmp_z = -tmp_z;
                    srv.request.model_name = tmp_model_name;
                    srv.request.model_xml = xml_data;
                    srv.request.initial_pose.position.x = px.data;
                    srv.request.initial_pose.position.y = py.data;
                    //std::cout << "pos_x "<<init_x <<std::endl;
                    // 90 digrees of z axis rotaion z=w=0.7071068
                    // -90 digrees of z axis rotaion z=-0.7071068; w=0.7071068
        
                    srv.request.initial_pose.orientation.w =  0.7071068;

                    if(client.call(srv)){ ROS_INFO("Response: %s, Model name: %s\n", srv.response.status_message.c_str(), tmp_model_name.c_str());}
                    else {
                        ROS_ERROR("Failed to call service");
                        return 1;
                        }
                    
                }

                
            


           
            
        }
            std::cout << "The required parking box was spawned\n";
            return false;

        }
};
}


