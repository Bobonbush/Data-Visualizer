#ifndef NODE_H
#define NODE_H
#include "Utils/Texture.h"
#include <vector>

class Node {
    private:
        

        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
        Shader* shader;
        Camera* camera;
        unsigned int texture;
        Shader * textShader; 
        TextHandler * textHandler;
        std::string text;
    public :

        Node* left;
        Node* right;
        
        int value;
        int height;

        glm::vec3 position;
        glm::vec3 targetPosition;
        glm::vec3 size;
        bool isHovered = false;
        Node(glm::vec3 _position, glm::vec3 _size, Camera* _camera , int _value);
        ~Node();
        
        void RecalculatePosition(float offsetx , float offsety);
        void Draw();
        void Update(float deltaTime, float MouseX, float MouseY);
        bool isInside(float MouseX, float MouseY);
        int getHeight(Node* node);
        int getBalance(Node* node);
        Node* rightRotate(Node* y);
        Node* leftRotate(Node* x);
};


class BNode {
    private: 
        unsigned int  VAO;
        unsigned int VBO;
        unsigned int EBO;
        Shader * shader;
        Camera * camera;
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