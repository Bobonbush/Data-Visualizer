#ifndef AVL_H
#define AVL_H
#include "Node.h"
struct Info { 
    float  width = 0 ;
    float xOffset = 0;
};

class AVL {
    private : 
    
    Camera * camera;
    int totalNodes = 0;
    float offsetx = 0.025f;
    float offsety = 0.3f;
    
    public :
    bool deleted = false;
    int currentVersion = 0;
    std::vector<Node *> root;
    AVL(Camera* _camera);
    ~AVL();
    void restart();
    Node* insert(Node* node, Node* parent, int value);
    void insert(int value);
    Node * Initialize(Node* node, int value);
    void Initialize(int value);
    Node* deleteNode(Node* node, int value);
    void deleteNode(int value);
    Node* minValueNode(Node* node);
    void Draw();
    void Update(float deltaTime, float MouseX , float MouseY);
    Node* search(Node* node, int values);
    void search(int value);

    void Reset(Node * node);
    void Reset();
    
    void RecalculatePosition(Node* node , float x , float y , float offset_x, float offset_y);
    void RecalculatePosition();
    Info calculateSubTreeWidth(Node* node, float offsetX);
    void PreOrder(Node* node);
    void PreOrder();
    void InOrder(Node* node);
    void InOrder();
    void PostOrder(Node* node);
    void PostOrder();
    bool Empty();

    Node* CopyNode(Node* node);
};

#endif