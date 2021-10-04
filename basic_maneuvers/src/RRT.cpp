#include "graph.cpp"
#include <visualization_msgs/Marker.h>


namespace PathPlanner{

    class RRT{
     
        public:
            SimpleGraph::Graph tree;
            float  SampleRate = 10;
            Parameters::RobotState start, goal;
            float dist_to_Goal = INFINITY;
            ros::Publisher vis_pub;
            RRT(ros::NodeHandle *node, Parameters::RobotState start, Parameters::RobotState goal){
                this->start = start;
                this->goal = goal;
                this->vis_pub = node->advertise<visualization_msgs::Marker>("visualization_marker", 0);
            }


           
            

           
            
            void BaseRRT(int iterations = 100){
                float dT = 0.1; // time sample 
                this->tree.addNode(this->start);
                Parameters::RobotState random_state, best_neigbour;
                SimpleGraph::Node nearest_node;
                Parameters::RobotState best_conv;
                float distToGoal = INFINITY;
                for (int i = 0; i< iterations; i++){
                    random_state = this->GetRandomState();        
                    printf("random: (x, y, theta) = (%f, %f, %f)\n",random_state.pose.x, random_state.pose.y, random_state.pose.theta);
                    nearest_node = this->GetNearestNode(random_state);
                    
                    best_neigbour = this->GetBestNeigbour(nearest_node.state, random_state, dT);
                    printf("neigb: (x, y, theta) = (%f, %f, %f)\n",best_neigbour.pose.x, best_neigbour.pose.y,best_neigbour.pose.theta);
                    this->tree.addNode(best_neigbour);
                    
                    float dx  = this->goal.pose.x - best_neigbour.pose.x;
                    float dy  = this->goal.pose.y - best_neigbour.pose.y;
                    float dtheta  = NormAngle(this->goal.pose.theta - best_neigbour.pose.theta);
                    this->tree.addEdge(nearest_node, this->tree.getNode(this->tree.nodes.size()-1), 1);
                    if(Dist(dx, dy, dtheta) < this->dist_to_Goal){
                        this->dist_to_Goal = Dist(dx, dy, dtheta);
                        best_conv = best_neigbour;
                    }

                    printf("tree_size = %d\n", this->tree.nodes.size());
                    printf("closest_dist = %f\n", this->dist_to_Goal );
                   
                   
        
                    // choose random (x, y, theta), relatively to the radius
                    // choose nearest node in graph to random
                    // create new_node v_new, psi_new, dT = 0.1, append and connect to existing nearest  
                    // for now, just if the path was founded, return the path as trajectory
                    // update search radius
                }
                printf("(x, y, theta) = (%f, %f, %f)\n", best_conv.pose.x, best_conv.pose.y, best_conv.pose.theta);   
            }
           

            void TimeBasedRRT(int iterations= 100){

                float dT = 0.1;
                float far_away = 2*Dist(this->goal.pose.x - this->start.pose.x, this->goal.pose.y - this->start.pose.y, this->goal.pose.theta - this->start.pose.theta);
              
                /*for convergation, not used now*/
                float epsilon = 0.1;
                std::vector<std::vector<int>> best_iters;
                float best_dist = 100000, best_dx = 1000, best_dtheta = 1000, best_dy = 1000;
                this->tree.addNode(this->start); 
                bool leave_iteration = false;

                    Parameters::RobotState cur_state =this->start;
                    /// k-->l, iter <  k1, k2 || iter> k2
                    for(int iter = 0; iter < iterations; iter++){
                        cur_state = this->start;
                        leave_iteration = false;
                        for(int k = 0; k < iterations; k++){
                            if(leave_iteration) break;
                            for(int l = 0; l < iterations; l++){
                                
                                if(iter < k || iter > l) cur_state = this->GenerateState(cur_state, 1, 0, dT);
                                else cur_state = this->GenerateState(cur_state, 1, M_PI/2, dT);
                                
                                this->tree.addNode(cur_state);
                                this->tree.addEdge(this->tree.getNode(this->tree.nodes.size() - 2), this->tree.getNode(this->tree.nodes.size() - 1), 1); 
                                
                                float dx = cur_state.pose.x - this->goal.pose.x;
                                float dy = cur_state.pose.y - this->goal.pose.y;
                                float dtheta = NormAngle(cur_state.pose.theta - this->goal.pose.theta);
                               
                                if(Dist(dx, dy, dtheta) < best_dist){
                                    best_dist = Dist(dx, dy, dtheta);
                                    best_dx = dx;
                                    best_dy = dy;
                                    best_dtheta = dtheta;
                                }
                                if(Dist(dx, dy, dtheta) > far_away) {
                                    leave_iteration = !leave_iteration;
                                    break;}

                                printf("current_iteration = %d, k1 = %d, k2 = %d\n", iter, k, l);
                                printf("founded: dist, dx, dy, dtheta = %f, %f, %f, %f\n" ,Dist(dx, dtheta), dx, dy, dtheta);
                                printf("best_values: dist, dx, dy, dtheta = %f, %f, %f, %f\n", best_dist, best_dx, best_dy, best_dtheta);
                            }

                        }

                    }

           }


            Parameters::RobotState GenerateState(Parameters::RobotState prevState, float v, float psi, float dT){
                Parameters::RobotState new_state;
                float dv = v - prevState.heading_speed;
                float dpsi = psi - prevState.steering_angle;
                new_state.heading_speed = Ramp(prevState.heading_speed + Ramp(dv/dT, SpeedRAMP)*dT, MAXFrwSPEED);
                new_state.steering_angle =  Ramp(prevState.steering_angle + Ramp(dpsi/dT, RotRAMP)*dT, MAXSteerANGLE);
                new_state.pose.x = prevState.pose.x +  new_state.heading_speed*cos(prevState.pose.theta +  new_state.steering_angle)*dT;
                new_state.pose.y = prevState.pose.y +  new_state.heading_speed*sin(prevState.pose.theta +  new_state.steering_angle)*dT;
                new_state.pose.theta = NormAngle(prevState.pose.theta +  new_state.heading_speed/WHEELBASE*sin(new_state.steering_angle)*dT);
                return new_state;
            }
            
            bool isGoalStateNear(Parameters::RobotState founded_state, float dT){

                float conv = 0.5;
                float tmp_dist = INFINITY, dx, dy, dtheta;
                Parameters::RobotState new_state; 
                for (float v = -1; v <= 1; v += 1){
                    for (float psi = -M_PI/2; psi <= M_PI/2; psi += M_PI/2){
                        new_state = this->GenerateState(founded_state, v, psi, dT);
                        dx = new_state.pose.x - this->goal.pose.x;
                        dy = new_state.pose.y -  this->goal.pose.y;
                        dtheta = NormAngle(new_state.pose.theta - this->goal.pose.theta);
                        tmp_dist = Dist(dx, dy, dtheta);
                        if (tmp_dist<=conv){
                            return true;
                        }
                    }
                }
                return false;

            }
            

            Parameters::RobotState GetRandomState(){
                int randomRate = rand() % 100 + 1;
                if (randomRate > SampleRate){
                    Parameters::RobotState rand_state;
                    rand_state.pose.x =  RandomDistribution<float>(std::min(this->start.pose.x, this->goal.pose.x), std::max(this->start.pose.x, this->goal.pose.x));
                    rand_state.pose.y =  RandomDistribution<float>(std::min(this->start.pose.y, this->goal.pose.y), std::max(this->start.pose.y, this->goal.pose.y));
                    rand_state.pose.theta =  RandomDistribution<float>(-M_PI, M_PI);
                    return rand_state;
                }
                return this->goal;
              

            }

            void vizualizeTree(int counter = 0){
                    visualization_msgs::Marker points, target_point;
                    points.header.frame_id = target_point.header.frame_id = "/tree";
                    points.header.stamp = target_point.header.stamp = ros::Time::now();
                    points.ns = target_point.ns = "tree_points";
                    points.action = target_point.action = visualization_msgs::Marker::ADD;
                    points.pose.orientation.w = target_point.pose.orientation.w = 1;
                    
                    points.id = 0;
                    target_point.id = 1;

                    points.type = target_point.type = visualization_msgs::Marker::POINTS;
                    points.scale.x = 0.5;
                    points.scale.y = 0.5;
                    points.scale.z = 0.5;
                    
                    target_point.scale.x = 1;
                    target_point.scale.y = 1;
                    target_point.scale.z = 1;
                    
                    points.color.g = 1.0f;
                    points.color.a = 1.0;
                    
                    target_point.color.r = 1;
                    target_point.color.a = 1;
                    
                    geometry_msgs::Point p;
                    p.x = this->goal.pose.x*10;
                    p.y = this->goal.pose.y*10;
                    p.z = this->goal.pose.theta*10;
                    target_point.points.push_back(p);
                    for(int i = 0; i< this->tree.nodes.size() && i < counter; i++){
                            p.x = this->tree.nodes[i].state.pose.x*10;
                            p.y = this->tree.nodes[i].state.pose.y*10;
                            p.z = this->tree.nodes[i].state.pose.theta*10;
                            points.points.push_back(p);
                    }
                    this->vis_pub.publish(points);
                    this->vis_pub.publish(target_point);

            }

            SimpleGraph::Node GetNearestNode(Parameters::RobotState random_state){
                    float closest_dist = INFINITY;
                    float tmp_dist, dx, dy, dtheta;
                    SimpleGraph::Node best_node; 
                    for (int j = 0; j < this->tree.nodes.size(); j++){
                        dx = this->tree.nodes[j].state.pose.x - random_state.pose.x;
                        dy = this->tree.nodes[j].state.pose.y -  random_state.pose.y;
                        dtheta =  NormAngle(this->tree.nodes[j].state.pose.theta -  random_state.pose.theta);
                        tmp_dist = Dist(dx, dy, dtheta);
                        if (tmp_dist < closest_dist){
                            closest_dist = tmp_dist; 
                            best_node = this->tree.nodes[j];    
                        }
                    }    
                    return best_node;
            }

            Parameters::RobotState GetBestNeigbour(Parameters::RobotState nearest_node_state, Parameters::RobotState random_state, float dT){
                float closest_dist = INFINITY;
                float tmp_dist, dx, dy, dtheta;
                Parameters::RobotState new_state, best_node; 
                for (float v = -1; v <= 1; v += 1){
                    for (float psi = -M_PI/2; psi <= M_PI/2; psi += M_PI/2){
                        new_state = this->GenerateState(nearest_node_state, v, psi, dT);
                        dx = new_state.pose.x - random_state.pose.x;
                        dy = new_state.pose.y -  random_state.pose.y;
                        dtheta =  NormAngle(new_state.pose.theta -  random_state.pose.theta);
                        tmp_dist = Dist(dx, dy, dtheta);
                        if (tmp_dist < closest_dist){
                            
                            closest_dist = tmp_dist;
                            best_node = new_state;
                        }
                    }
                }
                
                return best_node;
            }
    
            

};
};
