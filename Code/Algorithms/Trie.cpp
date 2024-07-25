#include "Trie.h"


TrieNode::TrieNode(glm::vec3 _position, glm::vec3 _size, Camera* _camera, std::string _word) {
    for (int i = 0; i < 256; i++) {
        _next[i] = nullptr;
        cnt[i] = 0;
        cnt_Leaf[i] = 0;
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    shader = new Shader("bugton.vs", "button.fs");
    camera = new Camera();
    texture = TextureLoader::LoadTexture("node.png");

    position = _position;
    size = _size;
    word = "";
    word = _word;
}

TrieNode::~TrieNode() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete shader;
    delete camera;
}

Trie::Trie(Camera * _camera) {
    camera = _camera;
    root = new TrieNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), camera, "");
}

void Trie::insert(std::string word) {
    TrieNode* current = root;
    for(int i = 0 ; i <= (int) word.size()-1 ; i++ ) {
        if(current -> _next[word[i]] == nullptr) {
            current -> _next[word[i]] = new TrieNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), new Camera(), word.substr(0, i+1));
        }
        current = current->_next[word[i]];
        current -> cnt[word[i]]++;
    }
    current->cnt_Leaf[word[(int)word.size()-1]]= true;
}

void Trie::Delete(std::string word) {
    TrieNode* current = root;
    std::vector<TrieNode*> nodes;
    for(int i = 0 ; i <= (int) word.size()-1 ; i++ ) {
        if(!current) return;
        current = current->_next[word[i]];
        nodes.push_back(current);
    }

    
    if(current -> cnt_Leaf[word[(int)word.size()-1]] == 0) {
        return;
    }
    current -> cnt_Leaf[word[(int)word.size()-1]]--;;
    

    for(int i = (int) word.size()-1 ; i >= 0 ; i--) {
        nodes[i]->cnt[word[i]]--;
        if(nodes[i]->cnt[word[i]] == 0) {
            delete nodes[i];
        }
    }
}

bool Trie::search(std::string word) {
    TrieNode* current = root;
    for(int i = 0 ; i <= (int) word.size()-1 ; i++ ) {
        if(current->_next[word[i]] == nullptr) {
            return false;
        }
        current = current->_next[word[i]];
    }
    return current->cnt_Leaf[word[(int)word.size()-1]] > 0;
}