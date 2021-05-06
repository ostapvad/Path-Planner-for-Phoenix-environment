// Parameters parsing in world_spawner node
#include "parameters.hpp"

class WorldParameters{
    public:
        std::vector<int> plane_sizes = {200, 200}; //[length, width] of the the plane
        
        bool enable_asphalt = true;
        float steerbot_road_width = 4; // width of moving part for the steerbot 
        std::vector<bool> random_type_spawn = {true, false}; // random types of car spawning, if to spawn = {bool, bool} = {to spawn on the slots, to spawn on the rectangle area}
        
        std::vector <int> cars_to_spawn = {15, 20}; // in two random modes {by slot, by area}
        std::vector <float> random_rectangle_area = {-30, -40, 15, 20}; // rectangle area of the area random car spawner {x, y, length, width}
        std::vector <std::string> boxes_to_spawn;  // a string to get the boxes parameters 
        std::vector <ParkingInfo *> boxes;  // array of  parking boxes to spawn
        
        WorldParameters(ros::NodeHandle &node){        
            init_parking();
            set_asphalt(node);
            set_plane_sizes(node);
            set_random_type(node);
            set_random_cars_to_spawn(node);
            set_random_rectangle_area(node);
            set_boxes_to_spawn(node);
            ShowParameters();
        };
        // Parse default parking parameters to the array
        void init_parking(){
            for (int j=0; j < init_row_nums.size(); j++){
                ParkingInfo *p = new ParkingInfo();
                p->parking_box_rows =  init_row_nums[j] ;
                p->init_xy[0] =  init_pose_x[j]; 
                p->init_xy[1] =  init_pose_y[j];
                p->parking_type =  init_park_types[j];
                boxes.push_back(p);

            }
           

        }
        
        // set the parameters
        void set_asphalt(ros::NodeHandle &node){

            if(!(node.hasParam("/world_spawner/enable_asphalt") && node.getParam("/world_spawner/enable_asphalt", enable_asphalt))){ node.setParam("/world_spawner/enable_asphalt", enable_asphalt);}


        }
        void set_plane_sizes(ros::NodeHandle &node){ 
            if(!(node.hasParam("/world_spawner/plane_sizes") && node.getParam("/world_spawner/plane_sizes", plane_sizes))){ node.setParam("/world_spawner/plane_sizes", plane_sizes);}
        }
        void set_steerbot_road_width(ros::NodeHandle &node){
            if(!(node.hasParam("/world_spawner/steerbot_road_width") && node.getParam("/world_spawner/steerbot_road_width", steerbot_road_width) )) { node.setParam("/world_spawner/steerbot_road_width", steerbot_road_width);}
        }
        void set_random_type(ros::NodeHandle &node){
              if(!(node.hasParam("/world_spawner/random_type_spawn") &&  node.getParam("/world_spawner/random_type_spawn", random_type_spawn))) { node.setParam("/world_spawner/random_type_spawn", random_type_spawn);}
        }
        void set_random_cars_to_spawn(ros::NodeHandle &node){
            if(!(node.hasParam("/world_spawner/random_cars_to_spawn") && node.getParam("/world_spawner/random_cars_to_spawn", cars_to_spawn))){node.setParam("/world_spawner/random_cars_to_spawn", cars_to_spawn);}
   
        }
        void set_random_rectangle_area(ros::NodeHandle &node){
            if(!(node.hasParam("/world_spawner/random_rectangle_area") && node.getParam("/world_spawner/random_rectangle_area", random_rectangle_area))){ node.setParam("/world_spawner/random_rectangle_area", random_rectangle_area);}
        } 
        void set_boxes_to_spawn(ros::NodeHandle &node){
            MultiTuple<std::tuple<int, float, float, std::string>>::Type params_tuple, cmp_tuple;
            InfoToToParameters(params_tuple);
            cmp_tuple = params_tuple;
            
            
            std::vector<int> tmp_rows_nums;
            std::vector<float> tmpose_x, pose_y;



            if(node.hasParam("/world_spawner/boxes_rows_number")){
                node.getParam("/world_spawner/boxes_rows_number", std::get<0>(cmp_tuple));
            }
            if(node.hasParam("/world_spawner/boxes_x_pose")){
                node.getParam("/world_spawner/boxes_x_pose", std::get<1>(cmp_tuple));
            }
            if(node.hasParam("/world_spawner/boxes_y_pose")){
                node.getParam("/world_spawner/boxes_y_pose", std::get<2>(cmp_tuple));
            }
            if(node.hasParam("/world_spawner/boxes_to_spawn")){
                node.getParam("/world_spawner/boxes_to_spawn", std::get<3>(cmp_tuple));
            }
         
    
       
            // length compare!
            if(std::get<0>(cmp_tuple).size() == std::get<1>(cmp_tuple).size() &&  std::get<1>(cmp_tuple).size() == std::get<2>(cmp_tuple).size() && std::get<2>(cmp_tuple).size()== std::get<3>(cmp_tuple).size()){
                std::cout<<"Boxes parameters were modified\n";
                boxes.erase(boxes.begin(), boxes.end());
                for(int i=0; i<std::get<0>(cmp_tuple).size();i++){
                    ParkingInfo *p = new ParkingInfo();
                    p->parking_box_rows = std::get<0>(cmp_tuple)[i];
                    p->init_xy[0] =  std::get<1>(cmp_tuple)[i]; 
                    p->init_xy[1] =  std::get<2>(cmp_tuple)[i];
                    p->parking_type = (std::find(default_parking_types.begin(), default_parking_types.end(), std::get<3>(cmp_tuple)[i][0]) != default_parking_types.end())? std::get<3>(cmp_tuple)[i][0]: 'C' ;
                    boxes.push_back(p);
                }


            }
            else{
                std::cout<<"WARNING! Boxes parameters were set to the defaultes\n";
                node.setParam("/world_spawner/boxes_rows_number", std::get<0>(params_tuple));
                node.setParam("/world_spawner/boxes_x_pose", std::get<1>(params_tuple));
                node.setParam("/world_spawner/boxes_y_pose", std::get<2>(params_tuple));
                node.setParam("/world_spawner/boxes_to_spawn",  std::get<3>(params_tuple));

            }


        }
        
        // parse array of pointers boxes to the tuple
        void InfoToToParameters( MultiTuple<std::tuple<int, float, float, std::string>>::Type &new_tuple){
           
            std::vector<int> rows_nums;
            std::vector<float> pose_x, pose_y;
            std::vector<std::string> parking_types;
            std::string conv;
            for(int i=0; i<boxes.size();i++){
                conv = "";
                rows_nums.push_back(boxes[i]->parking_box_rows);
                pose_x.push_back(boxes[i]->init_xy[0]);
                pose_y.push_back(boxes[i]->init_xy[1]);
                conv+=boxes[i]->parking_type;
                parking_types.push_back(conv);
            }
            new_tuple = std::make_tuple(rows_nums, pose_x, pose_y, parking_types);
       }
        
        // show the parameters
        void ShowParameters(){
            printf("------------------------------------\n......PARAMETERS......\n");
            printf("Enable asphalt model %s\n", (enable_asphalt)?"true":"false");
            printf("Plane sizes: length = %d, width = %d\n",plane_sizes[0], plane_sizes[1]);
            printf("Cars random spawner types status: \n 1)by parking places = %s, cars_to_spawn = %d\n", random_type_spawn[0]?"true":"false", cars_to_spawn[0]);
            printf(" 2)by rectangle area = %s, cars_to_spawn = %d\n", random_type_spawn[1]?"true":"false", cars_to_spawn[1]);
            printf("    rectangle_area_parameters: init_x = %f, init_y = %f;\n    length = %f, width = %f\n", random_rectangle_area[0], random_rectangle_area[1], random_rectangle_area[2], random_rectangle_area[3]);
            printf("----Boxes to spawn:\n");
            for(int i=0; i<boxes.size(); i++){
                printf("%d)\n", i+1);
                printf("Box type: %c consists %d rows\n",boxes[i]->parking_type, boxes[i]->parking_box_rows);
                printf("Init pose: x = %f, y = %f\n", boxes[i]->init_xy[0],boxes[i]->init_xy[1]);
            }
             printf("------------------------------------\n");
            


        }



};

