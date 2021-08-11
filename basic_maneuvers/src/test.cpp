#include "robot.hpp"


#define SleepRate 10 // 10 Hz
#define REF_POSE_X 10
#define REF_POSE_Y 0
#define REF_POSE_YAW 0 



enum TestModes {GoalToGoal, TrajectoryFollower, InitManeuver, TestController, TestTurn};

int main(int argc, char **argv){
    ros::init(argc, argv, "test_maneuvers");
    ros::NodeHandle tester;
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

    while(ros::ok()){
       
       switch (TestMode)
       {
       case GoalToGoal:
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


    

}