#include "maneuvers.cpp"


#define SleepRate 10 // 10 Hz
enum TestModes {GoalToGoal, TrajectoryFollower};

int main(int argc, char **argv){
    ros::init(argc, argv, "test_maneuvers");
    ros::NodeHandle tester;
   
    SteerBot:: Robot new_move(&tester);
    ros::Rate loop_rate(SleepRate);
    
    float final_speed = 3;
    float turn_angle = M_PI/2; // 90 deg
    float deviation = 0.001;

    /*

    State init_state;
    init_state.pose.x = 0;
    init_state.pose.y = 0;
    init_state.pose.yaw = 0;
    init_state.speed.linear = 0;
    init_state.speed.angular = 0;
    State goal_state;
    goal_state.pose.x = 5;
    goal_state.pose.y = 15;
    goal_state.pose.yaw = 0;    
    int counter = 0;
    std::vector <float> speeds = {0, 0};
    while (ros::ok()){
        speeds = PathController(goal_state, init_state);
     
       
        init_state = System(init_state, speeds[0], speeds[1]); 
        if (counter % 100 ==0){
        printf("(x, y) = (%f, %f), yaw = %f, frw_speed = %f, rot_speed = %f, it=%d\n", init_state.pose.x, init_state.pose.y, init_state.pose.yaw, speeds[0], speeds[1], counter);
        
        }
         counter ++;
        ros::spinOnce();
        loop_rate.sleep();
    }
    */
    int TestMode = TrajectoryFollower;
    int counter_state = 0;
    while(ros::ok()){
       
       switch (TestMode)
       {
       case GoalToGoal:
            new_move.PathController(new_move.ref_pose, new_move.cur_state);
            break;
       case TrajectoryFollower:
     
            printf("(x_goal, y_goal) = (%f, %f), (x_cur, y_cur) = (%f, %f), yaw = %f\n", new_move.trajectory[counter_state].pose.x, new_move.trajectory[counter_state].pose.y,  new_move.cur_state.pose.x,  new_move.cur_state.pose.y, new_move.cur_state.pose.theta); 
            counter_state = (new_move.PathController(new_move.trajectory[counter_state].pose, new_move.cur_state))? counter_state+1: counter_state;

            break;
       
       default:
           break;
       }
       /*
        if (counter < 30){ new_move.SetVelocity(1, 0);}
        else if(counter >= 30 && counter < 60) { new_move.SetVelocity(1, 1);}
        else if (counter >= 60){new_move.SetVelocity(0, 0);}
   
        ros::spinOnce();
        loop_rate.sleep();
        counter ++;
        std::cout << "Counter" << counter <<std::endl;
        */
       
       ros::spinOnce();
        loop_rate.sleep();
    }
    

    

}