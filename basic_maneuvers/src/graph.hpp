#include "libraries.hpp"


#ifndef _GRAPH_
#define _GRAPH_


namespace SimpleGraph{
    
   
    
    typedef struct{
        Parameters::RobotState state;
        int index = -1;
    }Node;

     typedef struct{
        Node node1, node2;  
        float cost = -1;
    }Edge;

    
    class Graph{
        public:
            
            std::vector<Node> nodes;
            std::vector<Edge> edges;  
            void addNode(Parameters::RobotState state);
            Node getNode(int index);
            void addEdge(Node node1, Node node2, float cost);
            Edge getEdge(int first_ind, int second_ind); 
            void emptyTree();
            ///float NodeDist(Parameters::RobotState state1, Parameters::RobotState state2);
            //int findByState(Parameters::RobotState state);
    };

};
#endif
