#include "node.h"

class Edge {
    public : 
        Node * start;
        Node * end;
        float weight;
        bool Directed;
        Edge(Node * _start, Node * _end, float _weight);
        ~Edge();
        void Draw();
        void Update();
};


class GRAPH {
    std::vector<Edge*> edges;
    std::vector<Node*> nodes;
    public:
    GRAPH();
    ~GRAPH();
    /*
    void Draw();
    void Update();
    void AddNode(Node * node);
    void AddEdge(Edge * edge);
    void RemoveNode(Node * node);
    void RemoveEdge(Edge * edge);
    void RemoveEdge(Node * start, Node * end);
    */
};