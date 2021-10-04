//#include "robot.cpp"

#include  "robot.cpp"

#define SleepRate 10 // 10 Hz



//enum TestModes {GoalToGoal, TrajectoryFollower, InitManeuver, TestController, TestTurn};

int main(int argc, char **argv){
    
    ros::init(argc, argv, "test_maneuvers");
    ros::NodeHandle tester;
    ros::Rate loop_rate(SleepRate);
 


    SteerBot::Robot new_robot(&tester);
    Parameters::RobotState goal_state; 
    goal_state.pose.x = 10;
    goal_state.pose.y = 10;
    goal_state.pose.theta = M_PI/2;

    PathPlanner::RRT new_search(&tester, new_robot.cur_state, goal_state);

  
    bool isBuilding = true;
    int counter = 0;;
    
    while(ros::ok()){
        if(isBuilding){
            //new_search.BaseRRT(20000);
            new_search.TimeBasedRRT(80);
            counter = new_search.tree.nodes.size();
            isBuilding = !isBuilding;
        }
        else{
           new_search.vizualizeTree(counter);
           counter ++;
        }
        ros::spinOnce();
        loop_rate.sleep();
        
    }   
    
    
   
   

    return 0;

}
