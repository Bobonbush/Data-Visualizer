#include "Utils/Texture.h"
#include "node.h"
#include <queue>

class GNode : Node {
    private :
    TextHandler * textHandler;
    Shader * subTextShader;
    
    public:
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f ,0.0f);
    glm::vec3 force = glm::vec3(0.0f, 0.0f, 0.0f);
    
    
    std::vector<std::pair<GNode *, int> > neighbours;

    GNode(glm::vec3 _position, glm::vec3 _size, int data , Camera * _camera);
    ~GNode();

    void Draw();
    void DrawLine(glm::vec3 start, glm::vec3 end, glm::vec4 color, int weight);
    void Update(float deltaTime, float MouseX, float MouseY);
    void AddNeighbour(GNode * node, int weight);

    int GetStatus () {
        return status;
    }

    void changeStatus(int _status) {
        status = _status;
    }

    void changeTargetPosition(glm::vec3 _targetPosition) {
        targetPosition = _targetPosition;
    }

    void SetPosition(glm::vec3 _position) {
        position = _position;
        targetPosition = _position;
    }

    glm::vec3 GetPosition() {
        return position;
    }

    void UndoAnimation() {
        Done = false;
        traverse = false;
    }

    void changeText(std::string _text) {
        text = _text;
    }

    void changeValue(int _value) {
        value = _value;
    }

    int GetValue() {
        return value;
    }

    glm::vec3 GetSize() {
        return size;
    }
};

class Edges {
    public:
    GNode * start;
    GNode * end;
    int weight;
    bool MST = false;
    Edges(GNode * _start, GNode * _end, int _weight) {
        start = _start;
        end = _end;
        weight = _weight;
    }

};

class DisJointSet {
    private:
    std::vector<int > lab;
    public:
    DisJointSet(int n) {
        lab.resize(n+1);
        for(int i = 1 ; i <= n ; i++) {
            lab[i] = -1;
        }
    }

    int Find(int u) {
        return (lab[u] < 0 ) ? u : lab[u] = Find(lab[u]);
    }

    bool Join(int u, int v) {
        u = Find(u);
        v = Find(v);
        if(u == v) return false;
        if(lab[u] > lab[v]) std::swap(u, v);

        lab[u] += lab[v];
        lab[v] = u ;
        return true;
    }
};

class GRAPH {

    private:
    const float REPULSION_CONSTANT = 0.05f;
    const float ATTRACTION_CONSTANT = 1.f;
    const float DAMPING = 0.85f;
    const float MAX_DISPLACEMENT = 0.1f;
    std::vector<GNode *> nodes;
    std::vector<Edges *> edges;
    Camera * camera;
    std::queue<GNode *> q;
    DisJointSet * dsu;
    std::vector<int>space;

    int component = 0 ;
    bool MST = false;
    bool CC = false;


    int debugConst = 0;
    std::vector<glm::vec4> edgecolor;
    public:
    GRAPH(Camera * _camera);
    ~GRAPH();

    void restart();
    void AddNode(GNode * node);
    void AddEdge(GNode * start, GNode * end, int weight);
    void FileInitialize(int n ,int m , std::ifstream & file);
    void Draw();
    void Update(float deltaTime, float MouseX, float MouseY);
    void Reset();
    bool isEmpty() {
        return nodes.empty();
    }

    void Initialize(int numNode ,int numEdge);

    bool MinimumSpanningTree();

    bool ConnectedComponents();

    void calculateForces();

    void updatePositions(float deltaTime);
    
    void seperateOverlappingNodes(GNode* a , GNode * b);
    
    void preventNodeEdgeOverLap(GNode * node, GNode * start , GNode * end);
};