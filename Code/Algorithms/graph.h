#include "node.h"

class Edge {
    public : 
        Node * start;
        Node * end;
        float weight;
        bool Directed = false;
        Edge(Node * _start, Node * _end, float _weight);
        ~Edge();
        void Draw();
        void Update(float deltaTime, float MouseX , float MouseY);
};


class GRAPH {
    std::vector<Edge*> edges;
    std::vector<Node*> nodes;
    bool Directed = false;
    Camera * camera;
    public:
    GRAPH(Camera* _camera, bool _Directed);
    ~GRAPH();
    
    void Draw();
    void Update(float deltaTime, float MouseX , float MouseY);
    void AddNode(Node * node);
    void AddEdge(Edge * edge);
    void RemoveNode(Node * node);
    void RemoveEdge(Edge * edge);
    void RemoveEdge(Node * start, Node * end);
};