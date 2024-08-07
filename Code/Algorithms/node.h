#ifndef NODE_H
#define NODE_H
#include "Utils/Texture.h"
#include <vector>
#include "Utils/Cursor.h"

class Node {
    private:
        

        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        Shader* shader;
        Camera* camera;
        unsigned int texture;
        unsigned int textureHovered;
        unsigned int textureDel;
        unsigned int textureNew;
        unsigned int textureRelated;
        unsigned int textureFound;
        Shader * textShader; 
        TextHandler * textHandler;

        
        /*
            status = -1 nothing happens
            status = 0 just traverse
            status = 1 new
            status = 2 deletation

        */
        

        Shader * lineShader;
    public :
        std::string text;

        Node* left;
        Node* right;
        
        int value;
        int height;
        static float speed;

        glm::vec3 position;
        glm::vec3 targetPosition;
        glm::vec3 size;
        glm::vec3 Forces = glm::vec3(0.0f, 0.0f, 0.0f);
        bool isHovered = false;
        bool Animating = false;
        static bool isDragging;
        static bool isOver;
        int status = -1;
        Node(glm::vec3 _position, glm::vec3 _size, Camera* _camera , int _value);
        ~Node();
        
        void RecalculatePosition(float offsetx , float offsety);
        void Draw();
        void Update(float deltaTime, float MouseX, float MouseY);
        bool isInside(float MouseX, float MouseY);
        int getHeight(Node* node);
        int getBalance(Node* node);
        int SubTreeHeight(Node* node);
        Node* rightRotate(Node* y);
        Node* leftRotate(Node* x);
};

struct NodeInfo {
    Node* node;
    int status = 0; // 0. just traverse , 1. new, 2. deletation
    float AnimationTime = 0.0f;
    int rotate = 0; 
    Node* node2 = nullptr;
    /*
    0 right
    1 left
    2 left right
    3 right left
    */
    NodeInfo(Node* _node, int _status) : node(_node), status(_status) {}
};

class BNode {
    private: 
        unsigned int  VAO;
        unsigned int VBO;
        unsigned int EBO;
        Shader * shader;
        Camera * camera;
        Shader * lineShader;
        unsigned int texture;
        


    public : 
        glm::vec3 position;
        glm::vec3 size;
        std::vector <BNode*> child;
        std::vector<int>key;
        bool isLeaf = true;

        BNode * parent;
        bool isHovered = false;

        int n; // number of key
        int degree; // degree of the tree
        BNode(glm::vec3 _position, glm::vec3 _size, Camera * _camera, int _degree);

        ~BNode();

        void Draw();
        void Update(float deltaTime, float MouseX , float MouseY);
        bool isInside(float MouseX , float MouseY);
        void traverse();
        BNode* search(int value);
        int findKey(int value);
        void insertNonFull(int value);
        void splitChild(int i, BNode* y);
        void remove(int value);
        void removeFromLeaf(int idx);
        void removeFromNonLeaf(int idx);
        int getPred(int idx);
        int getSucc(int idx);
        void fill(int idx);
        void borrowFromPrev(int idx);
        void borrowFromNext(int idx);
        void merge(int idx);
        
        
};

#endif