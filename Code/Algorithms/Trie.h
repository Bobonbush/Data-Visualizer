#include "Utils/Texture.h"

#include <iostream>
#include <string>


class TrieNode {
    
    
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    Shader * shader;
    Camera * camera;
    unsigned int texture;
    std::string word;
    glm::vec3 position;
    glm::vec3 size;
    public : 
    TrieNode* _next[256];
    int cnt[256];
    int cnt_Leaf[256];
    
    TrieNode(glm::vec3 _position, glm::vec3 _size, Camera* _camera, std::string _word);
    ~TrieNode();
};

class Trie {
    TrieNode * root;
    Camera * camera;
    public : 
    Trie(Camera *_camera);
    ~Trie();
    void insert(std::string word);
    bool search(std::string word);
    void Delete(std::string word);

};
