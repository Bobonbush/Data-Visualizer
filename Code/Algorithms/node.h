#ifndef NODE_H
#define NODE_H
#include "Utils/Texture.h"
#include <vector>
#include "Utils/Cursor.h"

class Node {
    protected:
        

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

        bool traverse = false;
        bool Done = false;


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
        virtual void Draw();
        virtual void Update(float deltaTime, float MouseX, float MouseY);
        bool isInside(float MouseX, float MouseY);
        int getHeight(Node* node);
        int getBalance(Node* node);
        int SubTreeHeight(Node* node);
        Node* rightRotate(Node* y);
        Node* leftRotate(Node* x);
};



#endif