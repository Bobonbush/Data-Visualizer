#include "UI/edge.h"
#include <algorithm>
#include <queue>
#include <vector>
class GRAPH {
private:
    std::vector<Node*> nodes;
    std::vector<Edge*> edges;
    std::vector<int>degree;
    bool Directed = false;
    std::vector<int>dist;
    GRAPH(std::vector<Node*> _nodes, std::vector<Edge*> _edges, bool _Directed);
    ~GRAPH();
    std::vector<std::vector<std::pair<int,int >>>label;
    std::vector<int>rank;

    void Dijkstra(int s);
    void PrunedLabelHub();
    void PushHub(int hub , int lab ,int weight);
    int ShortestPathByLabelHub(int s, int t);
    void HubLabeling(int s);


    


};