//
// Created by rodrigo on 10/05/2021.
//

#ifndef VACCINEROUTER_NODETONODE_H
#define VACCINEROUTER_NODETONODE_H


#include "../graph/Graph.h"

class NodeToNode {
private:
    Graph * graph;
    std::unordered_map<Node *, double> distForward;
    std::unordered_map<Node *, double> distBackward;
    std::unordered_map<Node *, Node *> path;
    std::unordered_map<Node*, bool> visitedForward;
    std::unordered_map<Node*, bool> visitedBackward;
    void setup();
public:
    NodeToNode(Graph * graph);
    double dijkstra(Node * orig, Node* dest);
};


#endif //VACCINEROUTER_NODETONODE_H