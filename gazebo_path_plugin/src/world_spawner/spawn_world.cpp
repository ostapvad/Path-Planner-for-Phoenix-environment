#include "world_spawner.cpp"


std::string pathToConfig = ros::package::getPath("gazebo_path_plugin") + "/config/"; // path to Config folder with yaml files
std::vector<std::string> config_files = {"main_parking.yaml", "bottom_parking.yaml"}; // names of files in that folder

int main(int argc, char **argv){
    ros::init(argc, argv, "spawn_world_client");
    
    // -t - test the single parking of the current or default parameters
    // -f - spawn the full world, the set of planes from the yaml files
    // -c - clear the previous models in the simulation
    // Formats: {-t -c}, {-f -c}, {-t}, {-f}

    if (argc ==3){ 
        if ((strcmp(argv[1], "-t") == 0) && (strcmp(argv[2], "-c")==0)){ // -t -c
            
            bool clear_world = (argc == 3 && !(strcmp(argv[2], "-c")))?true: false;
            SpawnedWorld::World new_world(clear_world);
            new_world.RunSpawner();
            
            } 
        
<<<<<<< HEAD
<<<<<<< HEAD
        else if ((strcmp(argv[1], "-f") == 0) && (strcmp(argv[2], "-c")==0)){ // -f -c
=======
        else if ((strcmp(argv[1], "-f") == 0) && (strcmp(argv[2], "-f")==0)){ // -f -c
>>>>>>> b45cb02c4b15ab5103ac7fe4cc7e88b829370475
=======
        else if ((strcmp(argv[1], "-f") == 0) && (strcmp(argv[2], "-c")==0)){ // -f -c
>>>>>>> 8a200bd8873c353568386cc66abc309758d39615
          
            
            bool clear_world = (argc == 3 && !(strcmp(argv[2], "-c")))?true: false;
            SpawnedWorld::World new_world(clear_world);
            // spawn planes by files
            for (int i=0; i < config_files.size(); i++){
                new_world.params.setFromYaml(pathToConfig + config_files[i], new_world.spawner);
                new_world.RunSpawner("_"+std::to_string(i+1));
            }
            


        }
        else {std::cout << "Arguments weren't detected or incorrrect!\n";}
    }
    else if (argc ==2){
        SpawnedWorld::World new_world;
        if (strcmp(argv[1], "-t") == 0){ new_world.RunSpawner();} // -t 
        
        else if (strcmp(argv[1], "-f") == 0){ // -f
            // spawn planes by files
            for (int i=0; i < config_files.size(); i++){
                new_world.params.setFromYaml(pathToConfig + config_files[i], new_world.spawner);
                new_world.RunSpawner("_"+std::to_string(i+1));
            
            }
        }
        
        else {std::cout << "Arguments weren't detected or incorrrect!\n";}
    }
   
   else {std::cout << "Arguments weren't detected or incorrrect!\n";} 
   
   
   return 0;
 }