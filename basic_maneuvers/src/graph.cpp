
#include "graph.hpp"


void SimpleGraph::Graph::addNode(Parameters::RobotState state){
    Node new_node;
    new_node.state.heading_speed = state.heading_speed;
    new_node.state.heading_speed = state.heading_speed;
    new_node.state.steering_angle = state.steering_angle;
    new_node.state.pose.x = state.pose.x;
    new_node.state.pose.y = state.pose.y;
    new_node.state.pose.theta = state.pose.theta;
    new_node.index =  this->nodes.size();
    this->nodes.push_back(new_node);

}
SimpleGraph::Node SimpleGraph::Graph::getNode(int index){
    for (int i = 0; i < this->nodes.size(); i++){
        if(this->nodes[i].index == index){
            return this->nodes[i];
        }
    }
    Node empty_node;
    return empty_node;

}
void SimpleGraph::Graph::addEdge(Node node1, Node node2, float cost){
    Edge new_edge;
    new_edge.node1 = node1;
    new_edge.node2 = node2;
    new_edge.cost = cost;
    this->edges.push_back(new_edge);
}
SimpleGraph::Edge SimpleGraph::Graph::getEdge(int first_ind, int second_ind){
        for(int i=0; i<edges.size(); i++){
            if(this->edges[i].node1.index == first_ind && this->edges[i].node2.index == second_ind){
                return this->edges[i];
            }

        }
        Edge empty_edge;
        return empty_edge;

}
void SimpleGraph::Graph::emptyTree(){
    this->edges.clear();
    this->nodes.clear();
}

