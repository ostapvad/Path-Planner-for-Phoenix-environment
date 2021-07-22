#include "status_receiver.cpp"

int main(int argc, char **argv){
  
    ros::init(argc, argv, "vizualize_path");
    ros::NodeHandle vizualizer;
    
    PathViewer::Trajectory new_trajectory(&vizualizer);
    ros::Rate loop_rate(10);
    
    while(ros::ok()){
        
        ros::spinOnce();
        loop_rate.sleep();
     
    }
   
    return 0;
   
 }

// Now let's implement the first step!

    /*
    TO DO!
    1) if we set listen to gazebo than call subsciber to odometry and call the client to spawn the model 
    2) else read from the file and spawn point by point
    

*/