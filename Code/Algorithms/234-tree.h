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
    BNode(glm::vec3 _postion , glm::vec3 _size, Camera * _camera) : parent(nullptr) {
        position = _postion;
        size = _size;
        
        camera = _camera;
        texture = TextureLoader::LoadTexture("box.png");
        float width = camera -> width;
        float height = camera -> height;

        float texture_width = TextureLoader::GetTextureSize("box.png").first;
        float texture_height = TextureLoader::GetTextureSize("box.png").second;
        
        text = new TextHandler();
        texture_width /= width;
        texture_height /= height;

        size.x = size.x * texture_width;
        size.y = size.y * texture_height;  
        
        shader = new Shader("Button.vs", "Button.fs");
        textShader = new Shader("textShader.vs", "textShader.fs");
        lineShader = new Shader("line.vs", "line.fs");
        text -> LoadFont("Font/Roboto-Black.ttf", 24);
        float vertices[] = {
            position.x - size.x / 2.f, position.y + size.y / 2.f, 0.0f, 1.0f, 1.0f,
            position.x + size.x / 2.f, position.y + size.y / 2.f, 0.0f, 1.0f, 0.0f,
            position.x + size.x / 2.f, position.y - size.y / 2.f, 0.0f, 0.0f, 0.0f,
            position.x - size.x / 2.f, position.y - size.y / 2.f, 0.0f, 0.0f, 1.0f
        };

        unsigned int  indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        


    }
    ~BNode() {
        for(auto child : children) {
            delete child;
        }
    }

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

    void Update(float deltaTime) {
        for (auto child : children) {
            child -> Update(deltaTime);
        }

        float offset_x = targetPosition.x - position.x;
        float offset_y = targetPosition.y - position.y;
        

        if(offset_x < 0.01f && offset_x > -0.01f) {
            offset_x = 0.f;
        }
        if(offset_y < 0.01f && offset_y > -0.01f) {
            offset_y = 0.f;
        }
        

        float speed = 3.f;

        position.x += offset_x * speed * deltaTime ;
        position.y += offset_y * speed * deltaTime ;
    }

    void Draw() {
        for(auto child : children) {
            child -> Draw();
        }
        
        
        int sz = (int) keys.size();

       glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        

        textShader -> use();
        textShader -> setMat4("projection", projection);
        glm::vec3 text_position = glm::vec3(position.x - size.x/2.f * sz, position.y, 0.0f);

        for(int i = 0 ; i < sz ; i++) {
            text_position.x += size.x/2.f;
            float offset = 0.0005f;
            text -> RenderMiddleText(textShader, std::to_string(keys[i]),text_position.x , text_position.y, 1.5f, glm::vec3(1.0f, 1.0f, 1.0f),camera, offset);
            text_position.x += size.x /2.f;
        }
        
        
        shader -> use();
        
        shader -> setMat4("projection", projection);
        shader -> setMat4("view", view);
        shader -> setMat4("model", model);

        float texutre_width = size.x;
        float texture_height = size.y;
        
        float vertices[] = {
            position.x - texutre_width /2.f * sz, position.y + texture_height/2.f, 0.0f, 1.0f, 1.0f, // top right
            position.x + texutre_width / 2.f * sz, position.y + texture_height /2.f, 0.0f, 1.0f, 0.0f, // bottom right
            position.x + texutre_width / 2.f * sz, position.y - texture_height/2.f, 0.0f, 0.0f, 0.0f, // bottom left
            position.x - texutre_width / 2.f * sz, position.y - texture_height/2.f, 0.0f, 0.0f, 1.0f // top left
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        
        
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glm::vec3 sub_position = glm::vec3(position.x - size.x / 2.f * sz, position.y, 0.0f);
        for(int i = 0 ; i < children.size(); i++) {
            sub_position.x += size.x / 2.f;
            glm::vec3 start = glm::vec3(position.x, position.y, 0.0f);
            sub_position.x += size.x / 2.f;
            glm::vec3 end = glm::vec3(children[i] -> position.x, children[i] -> position.y, 0.0f);
            lineShader -> use();

            unsigned int LINE_VAO;
            unsigned int LINE_VBO;
            glGenVertexArrays(1, &LINE_VAO);
            glGenBuffers(1, &LINE_VBO);

            glBindVertexArray(LINE_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, LINE_VBO);
            float line_vertices[] = {
                start.x, start.y, start.z,
                end.x, end.y, end.z
            };

            glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices), line_vertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glLineWidth(3.0f);
            lineShader -> setVec4("lineColor", glm::vec4(142.f/ 255.f, 162.f/255.f, 254.f/255.f, 1.0f));
            
            glDrawArrays(GL_LINES, 0, 2);
            glDeleteVertexArrays(1, &LINE_VAO);
            glDeleteBuffers(1, &LINE_VBO);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
    }
};

struct SubTreeInfo {
    float width = 0;
    float xOffset = 0;  
};

class Tree234 {
    private : 
        
        Camera * camera;

        void split(BNode * node);

        void insertRecursively(BNode* node, int key);
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
        void remove(int key);

        BNode* CopyNode(BNode* node, BNode * parent);
        void RecalculatePosition(BNode* node, float x , float y , float offset_x, float offset_y);
        void RecalculatePosition();
        SubTreeInfo calculateSubTreeWidth(BNode* node, float offsetX);
        void Update(float deltaTime);
        void Draw();
        bool isEmpty();

        
};