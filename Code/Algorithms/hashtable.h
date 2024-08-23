#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "Utils/Texture.h"
#include "234-tree.h"

class Block {
    private : 
        
        unsigned int texture;
        
        unsigned int texture_Found;
        unsigned int texture_New;
        unsigned int texture_Delete;
        unsigned int texture_Related;
        bool nullTexture = false;

        
    public :
        int key;
        int status = -1;
        bool traverse = false;
        glm::vec3 position;
        glm::vec3 targetPosition;
        glm::vec3 size;
        Shader * shader;
        Shader * textShader;

        TextHandler * text;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO; 
        Camera * camera;
        Block(glm::vec3 _position, glm::vec3 _size,  int _key, Camera * _camera, char * path, bool isNull = false);
        ~Block();
        glm::vec3 getSize();
        unsigned int getKey();
        void setKey(unsigned int key);

        void Update(float deltaTime);
        void Draw();
};

class LinkedBlock {
    private :
        
        LinkedBlock * next;
    public :
        Block * block;
        LinkedBlock(Block * block);
        ~LinkedBlock();
        Block * getBlock();
        LinkedBlock * getNext();
        void setNext(LinkedBlock * next);
};


class HashTable {
    std::vector<LinkedBlock*> blocks;
    Shader * lineShader;
    int size;
    int capacity;
    Camera * camera;

    TextHandler* text;
    Shader * textShader;

    std::string HashText = "";
    public:
    HashTable(Camera * _camera);
    ~HashTable();

    int HashFunction(int key);

    bool insert(int key);
    void Initialize(int key);

    bool Delete(int key);

    bool Find( int key);

    void Update(float deltaTime);

    void Reset();

    void Draw();

    void Recalculate();

    bool isEmpty() {
        return size == 0;
    }

};

#endif