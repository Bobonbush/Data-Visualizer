#include "Utils/Texture.h"
#include <algorithm>

class BNode {
    public:
    std::vector<int > keys;
    std::vector<BNode*> children;
    Camera * camera;
    BNode* parent;
    glm :: vec3 position;
    glm::vec3 targetPosition;
    glm ::vec3 size;
    Shader * shader;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int texture;
    Shader * textShader;
    Shader * lineShader;
    TextHandler * text;

    bool traverse = false;
    bool Done = false;

    BNode(glm::vec3 _postion , glm::vec3 _size, Camera * _camera);
    ~BNode();

    bool isLeaf() const {
        return children.empty();
    }

    bool isFull() const {
        return keys.size() == 3;
    }

    bool isUnderflow() const {
        return keys.size() < 1;
    }

    void insertKey(int key) {
        keys.push_back(key);
        std::sort(keys.begin(), keys.end());
    }

    void RemoveKey(int key) {
        auto it = std::find(keys.begin(), keys.end(), key);
        if (it != keys.end()) {
            keys.erase(it);
        }
    }

    void Update(float deltaTime);

    void Draw();
};

struct SubTreeInfo {
    float width = 0;
    float xOffset = 0;  
};

class Tree234 {
    private : 
        
        Camera * camera;

        void split(BNode * node);
        void instantSplit(BNode * node);

        void insertRecursively(BNode* node, int key);
        void initializeRecursively(BNode* node, int key);
        void deleteRecursively(BNode* node, int key); 
        BNode* getPredecessor(BNode* node, int index);

        void handleUnderflow(BNode* parent, int childIndex);

        void MergeNodes(BNode* parent, int index);
    public:
        int currentVersion = 0;
        //std::vector<BNode * > root;
        std::vector<BNode *> root;
        Tree234(Camera * _camera);
        ~Tree234();

        void insert(int key);
        void Initialize(int key);
        void remove(int key);
        

        BNode* CopyNode(BNode* node, BNode * parent);
        void RecalculatePosition(BNode* node, float x , float y , float offset_x, float offset_y);
        void RecalculatePosition();
        SubTreeInfo calculateSubTreeWidth(BNode* node, float offsetX);
        void Update(float deltaTime);
        void Draw();
        bool isEmpty();

        void FindNode(BNode* node, int key);

        void Reset();
        void Reset(BNode * node);

        
};