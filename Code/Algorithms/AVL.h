#include "Node.h"


class AVL {
    private : 
    Node * root;
    Camera * camera;
    int totalNodes = 0;
    float offsetx = 0.05f;
    float offsety = 0.05;
    public :
    AVL(Camera* _camera);
    ~AVL();
    Node* insert(Node* node, int value);
    void insert(int value);
    Node* deleteNode(Node* node, int value);
    void deleteNode(int value);
    Node* minValueNode(Node* node);
    void Draw();
    void Update(float deltaTime, float MouseX , float MouseY);
    Node* search(Node* node, int value);
    void search(int value);

    void PreOrder(Node* node);
    void PreOrder();
    void InOrder(Node* node);
    void InOrder();
    void PostOrder(Node* node);
    void PostOrder();
    bool Empty();
};