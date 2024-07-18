#ifndef GRAPH_H
#define GRAPH_H
#include "UI/edge.h"
#include <algorithm>
#include <queue>
#include <vector>
#include <map>
class GRAPH {
private:
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    std::vector<int>degree;
    bool Directed = false;
    std::vector<int>dist;
    std::map<int ,int > dict;
    Camera * camera;
public:
    GRAPH(bool _Directed, Camera * _camera);
    ~GRAPH();
    std::vector<std::vector<std::pair<int,int >>>label;
    std::vector<int>rank;
    
    void AddEdge(int u ,int v);
    void Dijkstra(int s);
    void PrunedLabelHub();
    void PushHub(int hub , int lab ,int weight);
    int ShortestPathByLabelHub(int s, int t);
    void HubLabeling(int s);

    void Update(float deltaTime, float x , float y);
    void Draw();


    


};

#endif