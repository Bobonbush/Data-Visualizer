#include "node.h"

class BTree {
    private: 
        unsigned int  VAO;
        unsigned int VBO;
        unsigned int EBO;
        Camera * camera;
        BNode* root;
        int degree;
    public :
        BTree(glm::vec3 _position, glm::vec3 _size, Camera * _camera, int _degree);
        ~BTree();

        void Draw();
        void Update(float MouseX , float MouseY);
        
        void insert(int value);
        void traverse();
        BNode*  Search(int value);
        void Delete(int value);


        
        
};