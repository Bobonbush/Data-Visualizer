#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "Utils/Texture.h"
#include "234-tree.h"

class Block {
    private : 
        unsigned int key;
        unsigned int texture;

        
    public :
        glm::vec3 position;
        glm::vec3 size;
        Shader * shader;
        Shader * textShader;

        TextHandler * text;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO; 
        Camera * camera;
        Block(glm::vec3 _position, glm::vec3 _size, unsigned int _key, Camera * _camera);
        ~Block();
        glm::vec3 getSize();
        unsigned int getKey();
        void setKey(unsigned int key);

        void Update(float deltaTime);
        void Draw();
};

class LinkedBlock {
    private :
        Block * block;
        LinkedBlock * next;
    public :
        LinkedBlock(Block * block);
        ~LinkedBlock();
        Block * getBlock();
        LinkedBlock * getNext();
        void setNext(LinkedBlock * next);
};


class HashTable {
    std::vector<LinkedBlock*> blocks;
    int size;
    int capacity;
    Camera * camera;

    HashTable(Camera * _camera);
    ~HashTable();

    void Update(float deltaTime);

    void Draw();

};

#endif