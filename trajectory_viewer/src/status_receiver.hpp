#include "ros/ros.h"
#include "ros/package.h"

#include "additional.hpp"

std::string path_prefix = ros::package::getPath("gazebo_path_plugin") + "/models";
std::vector<std::string> created_models = {path_prefix + "/single_point/model.sdf"}; // {parking_slot, car, asphalt, ground_plane}

namespace PathViewer{

    typedef struct deviations{
        float distance = 0.1; // 0.1 [m]
        float orientation_difference = 1; // [deg] 

    }Deviations;

    namespace States{
        typedef struct {
            struct DOF{
                struct Coord{
                float x, y;
            }coord;
            float yaw; 

            }dof;
            struct WheelsSpeed{
                float forward = 0;
                float turning = 0;
            }wheels_speed;
            
            
        }RobotState;
    };
    
    class Trajectory
    {   
        private:
            ros::Subscriber status, odometry;
            ros::ServiceClient client_spawner;
            gazebo_msgs::SpawnModel srv_spawner;
        public:
              
                std::vector <States::RobotState> robot_states;
                States::RobotState prev_state;
                Deviations devs; 
                Trajectory(ros::NodeHandle *node);
                void initSubsribers(ros::NodeHandle *node);
                void initClientsServices(ros::NodeHandle *node);
                void initRobotStates();
                void statusCallback(const phoenix_msgs::Status::ConstPtr &data);
                void odomCallback(const nav_msgs::Odometry::ConstPtr &data);
                void spawnState(States::RobotState state, std::string pathToModel, std::string model_name);
                bool PushToStates();

    
    };

};

