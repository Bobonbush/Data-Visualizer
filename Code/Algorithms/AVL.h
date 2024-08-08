#include "Node.h"


class AVL {
    private : 
    
    Camera * camera;
    int totalNodes = 0;
    float offsetx = 0.0025f;
    float offsety = 0.02f;
    public :
    int currentVersion = 0;
    std::vector<Node *> root;
    AVL(Camera* _camera);
    ~AVL();
    Node* insert(Node* node, Node* parent, int value, std::vector<NodeInfo> & nodes);
    void insert(int value, std::vector<NodeInfo> & nodes);
    void Initialize(int value, std::vector<NodeInfo> & nodes);
    Node* deleteNode(Node* node, int value , std::vector<NodeInfo> & nodes);
    void deleteNode(int value, std::vector<NodeInfo> & nodes);
    Node* minValueNode(Node* node);
    void Draw();
    void Update(float deltaTime, float MouseX , float MouseY);
    Node* search(Node* node, int value, std::vector<NodeInfo> & nodes);
    void search(int value, std::vector<NodeInfo> & nodes);
    
    void RecalculatePosition();
    void PreOrder(Node* node);
    void PreOrder();
    void InOrder(Node* node);
    void InOrder();
    void PostOrder(Node* node);
    void PostOrder();
    bool Empty();

    Node* CopyNode(Node* node);
};