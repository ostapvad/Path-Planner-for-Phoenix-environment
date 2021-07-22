#include "maneuvers.cpp"


#define SleepRate 10 // 10 Hz


int main(int argc, char **argv){
    ros::init(argc, argv, "test_maneuvers");
    ros::NodeHandle tester;
   
    SteerBot:: Robot new_move(&tester);
    ros::Rate loop_rate(SleepRate);
    
    float final_speed = 3;
    float turn_angle = M_PI/2; // 90 deg
    float deviation = 0.001;
    int counter = 0;
    while(ros::ok()){
        if (counter < 30){ new_move.SetVelocity(1, 0);}
        else if(counter >= 30 && counter < 60) { new_move.SetVelocity(1, 1);}
        else if (counter >= 60){new_move.SetVelocity(0, 0);}
   
        ros::spinOnce();
        loop_rate.sleep();
        counter ++;
        std::cout << "Counter" << counter <<std::endl;

    }

    

}