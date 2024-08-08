#ifndef TRIE_H
#define TRIE_H
#include "Utils/Texture.h"
#include "Utils/Cursor.h"
#include <iostream>
#include <string>


class TrieNode {
    
    
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    Shader * shader;
    Shader *lineShader;
    Shader * textShader;
    Camera * camera;
    unsigned int texture;
    unsigned int textureHovered;
    unsigned int textureDel;
    unsigned int textureNew;
    unsigned int textureRelated;
    unsigned int textureFound;
    unsigned int textureLeaf;
    TextHandler * textHandler;
    public : 
    std::string word;
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 targetPosition;

    bool isHovered = false;
    bool Animating = false;
    static bool isDragging;
    static bool isOver;
    int status = -1;
    TrieNode* _next[256];
    int cnt[256];
    int cnt_Leaf[256];
    float speed = 3.f;
    
    TrieNode(glm::vec3 _position, glm::vec3 _size, Camera* _camera, char _word);
    ~TrieNode();
    float RecalculatePosition(TrieNode* node, int depth, float offsetx);
    void Update(float deltaTime, float MouseX, float MouseY);
    void Draw();

    bool isInside(float MouseX, float MouseY);
};

struct TrieInfo {
    TrieNode* node;
    int status;
    bool deleted = false;
    bool newed = false;
    bool found = false;
    char word;
    TrieInfo(TrieNode* _node, int _status) {
        node = _node;
        status = _status;
    }
};

class Trie {
   
    Camera * camera;
    public : 
    std::vector<TrieNode*> root;
    int currentVersion = 0;
    Trie(Camera *_camera);
    ~Trie();
    TrieNode* CopyNode(TrieNode * node);
    void Update(float deltaTime, float MouseX, float MouseY);
    void Draw();
    void RecalculatePosition();
    void insert(std::string word, std::vector<TrieInfo> & nodes);
    bool subInsert(char word, TrieNode* &current, bool isLeaf);
    bool search(std::string word, std::vector<TrieInfo> & nodes);
    void Delete(std::string word, std::vector<TrieInfo> & nodes);
    void subDelete(char word, TrieNode* &current, bool isLeaf);
    bool Empty();
    void Initialize(std::string word);

};

#endif