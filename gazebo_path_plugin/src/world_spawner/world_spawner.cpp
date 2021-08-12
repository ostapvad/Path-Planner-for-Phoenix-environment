// Definition of world_spawner service-client node and it's logic

// Written files
#include "parameters.cpp"
#include "parking_boxes.cpp"

// ROS
#include "ros/package.h"

//Data msgs  
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"

// server-client msgs
#include "gazebo_msgs/SpawnModel.h" 
#include "gazebo_msgs/GetModelState.h"
#include "gazebo_msgs/DeleteModel.h"
#include "gazebo_msgs/GetWorldProperties.h"

#define NULL __null

// Paths to the models 
std::string path_prefix = ros::package::getPath("gazebo_path_plugin") + "/models";
std::vector<std::string> created_models = {path_prefix + "/parking_slot/model.sdf", path_prefix + "/car/model.sdf", path_prefix + "/asphalt/model.sdf", path_prefix + "/ground_plane/model.sdf"}; // {parking_slot, car, asphalt, ground_plane}


// Definition of spawner class
namespace SpawnedWorld{
class World {
    public: 

        float w_s;    //[m] - width of moving path for the steerbot( parameter)
        float w_p = 2.5;  //[m] - width of parking slot
        float l_p = 5.5;  //[m] - length of parking slot
        float line_width = 0.1; // width of parking white line
        int plane_length, plane_width; // size of the plane (parameter)
        std::vector<float> plane_xy = {0, 0};
        float car_height = 0.321049; // z coordinate of the car
        int total_spawned_cars = 0; // number of spawned cars
        int total_slots = 0;       // number of spawned slots
        int num_recently_added_boxes = 0;   // boxes added on the previouse plane
        std::string plane_postfix = "_1"; // plane number

        float next_pose[2] = {0, 0}; // last pose of D parking to spawn full parking
        
        std::vector<ParkingBoxes::Box*> created_boxes; // array of created parking blocks/boxes

        // init the required Nodes
        ros::NodeHandle spawner;
       
        ros::ServiceClient client_spawner = spawner.serviceClient<gazebo_msgs::SpawnModel>("/gazebo/spawn_sdf_model");
        ros::ServiceClient state_client =  spawner.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
        ros::ServiceClient delete_client = spawner.serviceClient<gazebo_msgs::DeleteModel>("/gazebo/delete_model");
        ros::ServiceClient world_status_client = spawner.serviceClient<gazebo_msgs::GetWorldProperties>("/gazebo/get_world_properties");
        gazebo_msgs::SpawnModel srv_spawner;
        gazebo_msgs::GetModelState srv_state;
        gazebo_msgs::DeleteModel srv_deleter;
        gazebo_msgs::GetWorldProperties srv_status;
        
        // Parameters of spawner
        Parameters::WorldParameters params;
        
       
        World(bool clear=false){
            if (clear) ClearWorld();   
            params.SetWorldParameters(spawner);
        }

        
        
        /*the main function*/
        void RunSpawner(std::string plane_num = "_1"){
            
            // "init" the new parking plane
            this->num_recently_added_boxes = 0;
            this->plane_postfix = plane_num;
            this->total_slots = 0;
            this->total_spawned_cars = 0;
            
            // running by given parameters
            this->w_s = params.steerbot_road_width;
            this->plane_length = params.plane_sizes[0];
            this->plane_width = params.plane_sizes[1];
            this->plane_xy = params.plane_pose;
            
    
            // asphalt or empty plane
            if(params.enable_asphalt) SpawnPlane(this->plane_length, this->plane_width, created_models[2], "asphalt" + this->plane_postfix, this->plane_xy[0], this->plane_xy[1]);
            else  SpawnPlane(this->plane_length, this->plane_width, created_models[3], "ground_plane" + this->plane_postfix, this->plane_xy[0], this->plane_xy[1]);
            
            // boxes to spawn
            for (int i=0; i<params.boxes.size(); i++){
        
                switch (params.boxes[i]->parking_type)
                {
                case 'C':
                
                    BuildBlockC(params.boxes[i]->parking_box_rows, params.boxes[i]->init_xy);
                    break;
                case 'D':
                    
                    BuildBlockD(params.boxes[i]->parking_box_rows, params.boxes[i]->init_xy);
                    break;
                case 'F':
                    BuildFull(params.boxes[i]->parking_box_rows, params.boxes[i]->init_xy);
                    break;
                default:
                
                    int type_id = params.boxes[i]->parking_type - '0';
                    InitParkingBox(type_id, params.boxes[i]->init_xy, params.boxes[i]->parking_box_rows);
                    break;
                }
            

            }

            // choose random car spawner
            int NumCarsOnSlots = 0;
            if(params.random_type_spawn[0]){
                SpawnRandomPos_Box(params.cars_to_spawn[0]);}
                NumCarsOnSlots = this->total_spawned_cars;
            if(params.random_type_spawn[1] && params.random_rectangle_area.size() == 4){
                float xy[2] = {params.random_rectangle_area[0], params.random_rectangle_area[1]};
                int rectangle[2] = {(int) abs(params.random_rectangle_area[2]), (int) abs(params.random_rectangle_area[3])};
                SpawnRandomPos(xy, rectangle, params.cars_to_spawn[1]);
            }
            
            params.ShowParameters();
            ShowParkingInfo(NumCarsOnSlots);

        }
        /*prints the parking info on the screen*/
        void ShowParkingInfo(int CarsOnSlots){
            printf("------------------------------------\nParking information:\n");
            printf("Number of slots spawned = %d\n", this->total_slots);
            printf("Number of randomly spawned cars = %d \n", this->total_spawned_cars);
            printf("a) On the parking slots: %d \nb) On the rectangle area: %d\n", CarsOnSlots, this->total_spawned_cars - CarsOnSlots);
            printf("------------------------------------\n");


        }

        /*Build the full parking: D and C block*/
        void BuildFull(int slots_number, float init_xy[2]){
                BuildBlockD(slots_number, init_xy);
                BuildBlockC(slots_number, next_pose);

        }
        
        /*Deletes the model of the world*/
        void DeleteModel(std::string mod_name){
            this->srv_deleter.request.model_name = mod_name;
            if (this->delete_client.call(srv_deleter) && this->delete_client.exists()){ ROS_INFO("Response: %s\n", srv_deleter.response.status_message.c_str());}
            else {ROS_ERROR("Failed to call service\n");} 

        }
        
        /*Deletes all the models of the world*/
        void ClearWorld(){
            
            std::vector<std::string> models;
            std::cout<<"Resetting the world!\n";
            if (this->world_status_client.call(srv_status) && this->world_status_client.exists()){ 
                ROS_INFO("Response: %s\n", srv_status.response.status_message.c_str());
                models = srv_status.response.model_names;
            }
            else {
                ROS_ERROR("Failed to call service\n");
                exit(1);
                } 
            
            for (int i=0; i< models.size(); i++){
                DeleteModel(models[i]);
            }



        }
    
       /*Spawns the cars on the parking slots*/
       void SpawnRandomPos_Box(int num_cars){
            int rand_id, rand_row, rand_col;
             
            while (this->total_spawned_cars < num_cars && this->num_recently_added_boxes !=0){
            
                if (this->total_spawned_cars >= this->total_slots){

                        std::cout <<"Places are full!\n"; 
                        break;
                }
              
                do{rand_id =  rand() % this->created_boxes.size();}
                while(created_boxes.size() - this->num_recently_added_boxes > rand_id ) ;
        
                rand_row =  rand() % div(this->created_boxes[rand_id]->slots_number, this->created_boxes[rand_id]->rotations.size()).quot;
                rand_col = rand() %  this->created_boxes[rand_id]->rotations.size();
                
                geometry_msgs:: Pose *pose = GetPose(rand_id, rand_row, rand_col, this->state_client, this->srv_state);
                      
                if (pose == NULL || this->created_boxes[rand_id]->is_full(rand_row, rand_col)) std::cout << "\n........Parking slot isn't availiable!........\n";
                else {
            
                    SpawnModel(*pose, created_models[1], "car" +std::to_string(this->total_spawned_cars) + this->plane_postfix);
                    this->created_boxes[rand_id]->UpdatePlace(rand_row, rand_col, true);
                    this->total_spawned_cars ++;
                    std::cout << "Cars spawned: " << this->total_spawned_cars << std::endl;
                }
               

            }
             
       }
 
        /*returns the position of the slot by its parameters: id, row and columns*/      
       geometry_msgs::Pose * GetPose(int id, int row, int col, ros::ServiceClient client, gazebo_msgs::GetModelState srv){
                std::string model_name = "id: " + std::to_string(id) + " r: " + std::to_string(row) + " c: " + std::to_string(col);
                srv.request.model_name = model_name;
          
                if (client.call(srv) && client.exists()){     
                    ROS_INFO("Response: %s\n", srv.response.status_message.c_str());
                    geometry_msgs:: Pose *pose;
                    pose->position =  srv.response.pose.position;
                    pose->position.z = car_height;
                    return pose;
                    }
        
                
            ROS_ERROR("Failed to call service\n");
            return NULL; 
        }

        /*Spawns the plane of reaquired type and size*/
        void SpawnPlane(int length, int width, std::string PathToFile, std::string model_name, float init_x = 0, float init_y = 0){
            
            // Update the sizes of the plane in the file
            std::string asphalt =readFileIntoString2(PathToFile);
            std::string sizes = std::to_string(length) + " " + std::to_string(width);
            std::size_t from = asphalt.find("size");
            std::size_t to = asphalt.find("<", from+1);
            asphalt.replace(from+5, to - from -5, sizes);
            from = asphalt.find("size", to+5);
            to = asphalt.find("<", from+1);
            asphalt.replace(from+5, to - from -5, sizes);
            
            // Update srv  
            
            // pose in 2D dimension
            this->srv_spawner.request.initial_pose.position.x = init_x;
            this->srv_spawner.request.initial_pose.position.y = init_y;
            this->srv_spawner.request.initial_pose.position.z = 0;
            
            // model and  name
            this->srv_spawner.request.model_name = model_name;
            this->srv_spawner.request.model_xml = asphalt;
        
            // Call spawner and show result
            bool result = CallSpawner(this->client_spawner, this->srv_spawner);
            std::string output = (result)? "Spawned!\n" : "Error!\n";
            std::cout << output;




        }

        /* Spawns the cars on the rectangle area randomly*/
        void SpawnRandomPos(float* xy, int * rectangle, int car_numbers){ // [x_start, y_start, l, w]
            geometry_msgs::Pose car_pose;
            car_pose.position.z = car_height;
            
            for (int i = 0; i < car_numbers; i++){
                
                // random pose
                car_pose.position.x = rand() % rectangle[0] + xy[0];
                car_pose.position.y = rand() % rectangle[1] + xy[0]; 
                
                // Random yam orientaion in [0; 360] deg
                RotZToQuat(car_pose.orientation, rand() % 361);

                //Spawn and update info
                SpawnModel(car_pose, created_models[1], "car" +std::to_string(this->total_spawned_cars) + this->plane_postfix);
                total_spawned_cars ++;
                std::cout << "Cars spawned: " << total_spawned_cars << std::endl;

            }


        }
        
               
        /* Spawns the model on the required position*/
        bool SpawnModel(geometry_msgs::Pose pose, std::string PathToFile, std::string model_name){
            // Update srv   
            this->srv_spawner.request.model_name = model_name;
            this->srv_spawner.request.model_xml = readFileIntoString2(PathToFile);
            this->srv_spawner.request.initial_pose = pose;
        
            // Call spawner and show result
            bool result = CallSpawner(this->client_spawner, this->srv_spawner);
            std::string output = (result)? "Spawned!\n" : "Error!\n";
            std::cout << output;
            return result;
        }
        
        /* Calls the "/gazebo/spawn_sdf_model" service */
        bool CallSpawner(ros::ServiceClient client, gazebo_msgs::SpawnModel srv){
            if (client.call(srv) && client.exists()){ ROS_INFO("Response: %s\n", srv.response.status_message.c_str());}
            else {
                ROS_ERROR("Failed to call service\n");
                return false;
                }

            return true; 
        }
        

        /*Builds the parking box*/
        void BuildParkingBox(ParkingBoxes::Box *b, int num_slots){ // pointer to parking box, number of rows to spawn
           
                // Init the name of the parking slot {id: sim_id r: i c: j}
                std::string tmp_model_name, model_name;
                model_name = "id: ";
                model_name += std::to_string(b->sim_id);
                geometry_msgs::Pose new_pose;

                for (int i = 0; i < num_slots; i++){ // number of rows
                
                    for (int j = 0; j < b->rotations.size(); j++) // number of rotation, columns
                    {   
                        // parse the model name
                        tmp_model_name = model_name;
                        tmp_model_name += " r: " + std::to_string(i) +" c: " + std::to_string(j);

                        // Complicated calculations ;)
                        new_pose.position.x = b->pose.x - i*(w_p - line_width);
                        new_pose.position.y = b->pose.y - j*(l_p - line_width);
                    
                        RotZToQuat(new_pose.orientation, b->rotations[j]);
                        
                        // Spawn model !
                        if(SpawnModel(new_pose, created_models[0], tmp_model_name)) {
                            b->CreateSlot(i,j);
                            total_slots++;
                            }
                    
                        
                    }           
            
                }

        }

        /*Builds the block C of the parking*/
        void BuildBlockC(int slots_number, float init_xy[2]){
            
            // some definitions for blok C 
            int num_parking_boxes = 8;
         
            int type_id;
            for (int i = 0; i < num_parking_boxes ; i++){
                
                // Choose type of parking box, shit code :(
                if (i < 2){ type_id  = 1;}
                else if (i>=2 && i< 6){type_id = 5;}
                else{  type_id = 4;}
                
                InitParkingBox(type_id, init_xy, slots_number);
                
                // Update pose for the next box
                if (i % 2 == 0){
                    init_xy[0]-= w_p + (slots_number-1)*(w_p - line_width) + w_s;   
                }
                else {
                    
                    init_xy[0] +=   w_p + (slots_number-1)*(w_p - line_width) + w_s;   
                    init_xy[1] -=   created_boxes.back()->rotations.size()*l_p + w_s;
                    }

            }

        }
   
        /*Pushes the new box to the created_boxes array*/
        void InitParkingBox(int type_id, float init_xy[2], int slots_number){
            
            ParkingBoxes::Box *b = new ParkingBoxes::Box (type_id, init_xy, created_boxes.size()); 
            this->created_boxes.push_back(b);
            BuildParkingBox(b, slots_number);
            this->num_recently_added_boxes ++;
        }

        /*Build the blok D of the parking*/
        void BuildBlockD(int slots_number, float init_xy[2]){
                
                // some definitions for blok D 
                int num_parking_boxes = 10; // boxes - 1
            
                
                for (int i = 0; i < num_parking_boxes ; i++){
                    
                    // Choose type of parking box, shit code :(
                
                    InitParkingBox(5, init_xy, slots_number);

                    if (i==1){
                        init_xy[1] +=  l_p + w_s;
                        InitParkingBox(2, init_xy, slots_number);
                        init_xy[1] -=  l_p + w_s;
                    }
                
                    // Update pose for the next box
                    if (i % 2 == 0){
                        init_xy[0]-= w_p + (slots_number-1)*(w_p - line_width) + w_s;   
                    }
                    else {
                        init_xy[0] +=   w_p + (slots_number-1)*(w_p - line_width) + w_s;   
                        init_xy[1] -=   this->created_boxes.back()->rotations.size()*l_p + w_s;
                        }

                }
                this->next_pose[0] = init_xy[0];
                this->next_pose[1] = init_xy[1];

            }
        
    };
}



