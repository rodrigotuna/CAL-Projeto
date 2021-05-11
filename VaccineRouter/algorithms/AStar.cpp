//
// Created by rodrigo on 11/05/2021.
//

#include "AStar.h"

#include <queue>
#include <cmath>

using namespace std;

AStar::AStar(Graph *graph) {
    this->graph = graph;
}

void AStar::setup(Node * dest) {
    for(auto &node : graph->nodes){
        dist[node] = Graph::INF;
        visited[node] = false;
        path[node] = nullptr;
        heuristic[node] = sqrt(pow(node->getX() - dest->getX(),2) + pow(node->getY() - dest->getY(),2));
    }
}

double AStar::run(Node *orig, Node *dest) {
    setup(dest);
    priority_queue<pair<double, Node*>> q;
    q.push(make_pair(0 - heuristic[orig], orig));
    dist[orig] = 0;
    path[orig] = nullptr;
    while(!q.empty()){
        Node * out = q.top().second; q.pop();
        if(visited[out]) continue;
        if(out == dest) return dist[dest];
        visited[out] = true;
        for(auto &edge : graph->adjList[out]){
            Node * in = edge->getEnd();
            double w = edge->getWeight();
            if(dist[in] > dist[out] + w){
                dist[in] = dist[out] + w;
                q.push({-dist[in] - heuristic[in], in});
                path[in] = out;
            }
        }
    }

    return Graph::INF;
}