#include "spawn_world.hpp"



int main(int argc, char **argv){
    
    SpawnedWorld::World new_world(argc, argv);
    new_world.SpawnParkingBox(1);
    return 0;
 }