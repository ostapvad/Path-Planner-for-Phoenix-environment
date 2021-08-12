<<<<<<< HEAD
#include "maneuvers.cpp"


#define SleepRate 10 // 10 Hz
<<<<<<< HEAD
enum TestModes {GoalToGoal, TrajectoryFollower};
=======

>>>>>>> b45cb02c4b15ab5103ac7fe4cc7e88b829370475
=======
#include "robot.hpp"


#define SleepRate 10 // 10 Hz
#define REF_POSE_X 10
#define REF_POSE_Y 0
#define REF_POSE_YAW 0 



enum TestModes {GoalToGoal, TrajectoryFollower, InitManeuver, TestController, TestTurn};
>>>>>>> 8a200bd8873c353568386cc66abc309758d39615

int main(int argc, char **argv){
    ros::init(argc, argv, "test_maneuvers");
    ros::NodeHandle tester;
<<<<<<< HEAD
   
    SteerBot:: Robot new_move(&tester);
    ros::Rate loop_rate(SleepRate);
    
    float final_speed = 3;
    float turn_angle = M_PI/2; // 90 deg
    float deviation = 0.001;
<<<<<<< HEAD

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
=======
    ros::Rate loop_rate(SleepRate);
 
    int TestMode = InitManeuver;
    int counter_state = 0;

    
    Paths::Generator trajectory;
    SteerBot::Robot robot(&tester);;
    RobotManeuvers::Maneuvers maneuver;

    while(ros::ok()){

        switch (TestMode)
        {
        case GoalToGoal:
            geometry_msgs::Pose2D ref_pose; ref_pose.x = REF_POSE_X; ref_pose.y = REF_POSE_Y; ref_pose.theta = REF_POSE_YAW;
            
            break;
        
        case TrajectoryFollower:
            
            break;
        default:
            break;
        }


        
        ros::spinOnce();
        loop_rate.sleep();
    
    }   
   

   
    /*

>>>>>>> 8a200bd8873c353568386cc66abc309758d39615
    while(ros::ok()){
       
       switch (TestMode)
       {
       case GoalToGoal:
<<<<<<< HEAD
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
=======
    int counter = 0;
    while(ros::ok()){
>>>>>>> b45cb02c4b15ab5103ac7fe4cc7e88b829370475
        if (counter < 30){ new_move.SetVelocity(1, 0);}
        else if(counter >= 30 && counter < 60) { new_move.SetVelocity(1, 1);}
        else if (counter >= 60){new_move.SetVelocity(0, 0);}
   
        ros::spinOnce();
        loop_rate.sleep();
        counter ++;
        std::cout << "Counter" << counter <<std::endl;
<<<<<<< HEAD
        */
       
       ros::spinOnce();
        loop_rate.sleep();
    }
    
=======

    }
>>>>>>> b45cb02c4b15ab5103ac7fe4cc7e88b829370475
=======
            new_move.PathController(new_move.ref_pose, new_move.cur_state); 
            break;
       case TrajectoryFollower:
                         
            //printf("(x_goal, y_goal) = (%f, %f), (x_cur, y_cur) = (%f, %f), yaw = %f\n", new_move.trajectory[counter_state].pose.x, new_move.trajectory[counter_state].pose.y,  new_move.cur_state.pose.x,  new_move.cur_state.pose.y, new_move.cur_state.pose.theta); 
            //counter_state = (new_move.PathController(new_move.trajectory[counter_state].pose, new_move.cur_state))? counter_state+1: counter_state;
            break;
       
       case InitManeuver:
            new_move.SetVelocity(0,0.5);
            printf("turn_angle = %f\n", new_move.cur_state.steering.angle);
            break;
        
        case TestController:
            new_move.NaiveController(new_move.ref_pose, new_move.cur_state); 
            break;
        case TestTurn:
            new_move.TurnAngle(1.57, new_move.cur_state);
            break;
       default:

           break;
       }
      
        ros::spinOnce();
        loop_rate.sleep();
    }
    */

>>>>>>> 8a200bd8873c353568386cc66abc309758d39615

    

}