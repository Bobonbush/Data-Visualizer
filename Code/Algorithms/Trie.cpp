#include "Trie.h"

bool TrieNode::isDragging = false;
bool TrieNode::isOver = false;

TrieNode::TrieNode(glm::vec3 _position, glm::vec3 _size, Camera* _camera, char _word) {
    for (int i = 0; i < 256; i++) {
        _next[i] = nullptr;
        cnt[i] = 0;
        cnt_Leaf[i] = 0;
    }
   
    shader = new Shader("button.vs", "button.fs");
    camera = _camera;
    position = _position;
    size = _size;
    word = "";
    word = _word;

    
    float texture_width = TextureLoader::GetTextureSize("node.png").first;
    float texture_height = TextureLoader::GetTextureSize("node.png").second;
    texture = TextureLoader::LoadTexture("node.png");
    textureHovered = TextureLoader::LoadTexture("on_node.png");
    textureDel = TextureLoader::LoadTexture("node_del.png");
    textureNew = TextureLoader::LoadTexture("new_node.png");
    textureRelated = TextureLoader::LoadTexture("traverse.png");
    textureFound = TextureLoader::LoadTexture("node_found.png");
    textureLeaf = TextureLoader::LoadTexture("leaf_node.png");
    lineShader = new Shader("line.vs", "line.fs");
    textHandler = new TextHandler();
    textHandler->LoadFont("Font/Revamped.ttf", 24);
    textShader = new Shader("textShader.vs", "textShader.fs");
    float width = camera -> width;
    float height = camera -> height;
    texture_width = texture_width / width;
    texture_height = texture_height / height;
    texture_width = texture_width * size.x;
    texture_height = texture_height * size.y;

    float vertices[] = {
        position.x - texture_width/2.f, position.y + texture_height/2.f, 0.f, 1.f, 1.f, // top right
        position.x - texture_width/2.f, position.y - texture_height/2.f, 0.f, 1.f, 0.f, // bottom right
        position.x + texture_width/2.f, position.y - texture_height/2.f, 0.f, 0.f, 0.f, // bottom left
        position.x + texture_width/2.f, position.y + texture_height/2.f, 0.f, 0.f, 1.f // top left
    };

    size.x = texture_width;
    size.y = texture_height;

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

TrieNode::~TrieNode() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete shader;
    delete camera;
    for (int i = 0; i < 256; i++) {
        if(_next[i] != nullptr) {
            delete _next[i];
        }
    }
}

bool TrieNode::isInside(float mouseX , float mouseY) {
    float width = camera -> width * size.x;
    float height = camera -> height * size.y;
    float x = position.x;
    float y = position.y;
    float half_x = camera -> width /2.f;
    float half_y = camera -> height /2.f;

    x = half_x + x * half_x;
    y = half_y - y * half_y;

    width /= 2.f;
    height /= 2.f;

    if (mouseX >= x -width && mouseX <= x + width /2 && mouseY >= y - height && mouseY <= y + height/4.f) {
        return true;
    }
    return false;
}


void TrieNode::Update(float deltaTime, float MouseX , float MouseY) {
    if(!TrieNode::isDragging) {
        isHovered = isInside(MouseX, MouseY);
        if(isHovered) {
            TrieNode::isDragging = true;
        }
    }
    if(!isOver) {
        isOver = isInside(MouseX , MouseY);
    }

    textShader -> use();
    textShader -> setMat4("projection", glm::mat4(1.0f));

    for(int i = 0 ; i < 256; i++) {
        if(_next[i] != nullptr) {
            _next[i] -> Update(deltaTime, MouseX, MouseY);
        }
    }

    if(isOver) {
        Cursor::CurrentCursor = Cursor::ReadyCursor;
    }

    if(isHovered) {
        Cursor::CurrentCursor = Cursor::ReadyCursor;
        if(glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            
        
            float half_x = camera -> width /2.f;
            float half_y = camera -> height /2.f;
            MouseX = (MouseX - half_x) / half_x;
            MouseY = (half_y - MouseY) / half_y;
            targetPosition.x = MouseX;
            position.x = MouseX;
            targetPosition.y = MouseY;
            position.y = MouseY;
            Cursor::CurrentCursor =  Cursor::HoldCursor;
        } else {
            isDragging = false;
            isHovered = false;
        }
    }

    if(!isOver) Cursor::CurrentCursor = Cursor::normalCursor;

    Cursor::SetCursor(glfwGetCurrentContext());

    if(status != -1 && status != 1) {
        return ;
    }

    float offset_x = targetPosition.x - position.x;
    float offset_y = targetPosition.y - position.y;

    if(offset_x < 0.01f && offset_x > -0.01f) {
        offset_x = 0.f;
    }
    if(offset_y < 0.01f && offset_y > -0.01f) {
        offset_y = 0.f;
    }
    if(offset_x == 0.f && offset_y == 0.f) {
        Animating = false;
    } else {
        Animating = true;
    }

    position.x += offset_x * deltaTime * speed;
    position.y += offset_y * deltaTime * speed;
}

void TrieNode::Draw() {
    float offset = 0.0005f;
    textHandler -> RenderMiddleText(textShader, word, position.x, position.y, 1.75f, glm::vec3(0.1f, 0.2f, 0.6f), camera, offset);
    shader -> use();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    shader -> setMat4("projection", projection);
    shader -> setMat4("view", view);
    shader -> setMat4("model", model);
    float texture_width = size.x;
    float texture_height = size.y;

    float vertices[] = {
        position.x - texture_width/2.f, position.y + texture_height/2.f, 0.f, 1.f, 1.f, // top right
        position.x - texture_width/2.f, position.y - texture_height/2.f, 0.f, 1.f, 0.f, // bottom right
        position.x + texture_width/2.f, position.y - texture_height/2.f, 0.f, 0.f, 0.f, // bottom left
        position.x + texture_width/2.f, position.y + texture_height/2.f, 0.f, 0.f, 1.f // top left
    };
    /*
    if(word != "") {
        for(int i = 0 ; i < 4 ; i ++) {
            for(int j = 0 ; j < 3 ; j ++) {
                std::cout << vertices[i * 5 + j] << " ";
            }
            std::cout << '\n';
        }
    }
    */

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    if(status == 3) {
        glBindTexture(GL_TEXTURE_2D, textureFound);
    }
    else if(cnt_Leaf[word[0]] > 0) {
        glBindTexture(GL_TEXTURE_2D, textureLeaf);
    }
    else if(Animating && status == -1) {
        glBindTexture(GL_TEXTURE_2D, textureHovered);
    }else if(status == -1) {
        glBindTexture(GL_TEXTURE_2D, texture);
    }else if(status == 2) {
        glBindTexture(GL_TEXTURE_2D, textureDel);
    }else if (status == 1) {
        glBindTexture(GL_TEXTURE_2D, textureNew);
    }else if(status == 0) {
        glBindTexture(GL_TEXTURE_2D, textureRelated);
    }

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    for(int i = 0 ; i < 256; i++) {
        if(_next[i] != nullptr) {
            _next[i] -> Draw();
            float lineVertices[] = {
                position.x, position.y, 0.0f,
                _next[i]->position.x, _next[i]->position.y, 0.0f
            };
            unsigned int lineVAO, lineVBO;
            glGenVertexArrays(1, &lineVAO);
            glGenBuffers(1, &lineVBO);
            glBindVertexArray(lineVAO);
            glBindBuffer(GL_ARRAY_BUFFER, lineVBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glLineWidth(3.0f);
            
           
            lineShader -> use();
            lineShader -> setVec4("lineColor",glm::vec4(142.f/ 255.f, 162.f/255.f, 254.f/255.f, 1.0f));
            glDrawArrays(GL_LINES, 0, 2);
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

        }
    }

}

float TrieNode::RecalculatePosition(TrieNode* node, int depth, float offsetx = 0) {
    if(node == nullptr) return 0.f;
    float totalWidth = 0;
    int ChildCnt = 0;
    for(int i = 0 ; i < 256; i++) {
        if(node->_next[i] != nullptr) {
            ChildCnt++;
        }
    }

    float currentXOffset =  offsetx;
    float offset = 0.15f;
    float verticaloffset = 0.15f;
    for(int i = 0 ; i < 256; i++) {
        if(node -> _next[i] != nullptr) {
            float width = RecalculatePosition(node->_next[i], depth + 1, currentXOffset);
            totalWidth += width;
            
            currentXOffset += width + offset;
        }
    }

    if(ChildCnt == 0) {
        node -> targetPosition.x = -0.5 + offsetx;
        node -> targetPosition.y = 0.75f -depth * verticaloffset;
        return offset;
    }

    node->targetPosition.x =  -0.5+ offsetx + totalWidth / 2.f;
    node->targetPosition.y = 0.75f -depth * verticaloffset;

    return totalWidth;
    
}
Trie::Trie(Camera * _camera) {
    camera = _camera;
}

Trie :: ~Trie() {
    for(int i = 0 ; i < (int) root.size(); i++) {
        if(root[i] != nullptr) {
            delete root[i];
        }
    }
}

TrieNode* Trie::CopyNode(TrieNode * node) {
    if(node == nullptr) return nullptr;
    TrieNode* newNode = new TrieNode(node->position, glm::vec3(0.6f, 0.6f, 0.6f), camera, ( (int)node->word.size()) ? node->word[0] : ' ');
    for(int i = 0 ; i < 256; i++) {
        newNode -> _next[i] = CopyNode(node->_next[i]);
        newNode ->cnt_Leaf[i] = node->cnt_Leaf[i];
        newNode -> cnt[i] = node->cnt[i];

    }
    newNode -> targetPosition = node -> targetPosition;
    return newNode;
}
void Trie::insert(std::string word, std::vector<TrieInfo> &nodes) {
    if(root.empty() || root[currentVersion] == nullptr) {
        root.push_back(new TrieNode(glm::vec3(0.0f , 0.0f, 0.0f), glm::vec3(0.6f, 0.6f, 0.0f), camera, ' '));
        root.back() -> targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    else {
        root.push_back(nullptr);
        root[++currentVersion] = CopyNode(root[currentVersion - 1]);
    }
    TrieNode* current = root[currentVersion];
    
    for(int i = 0 ; i <= (int) word.size()-1 ; i++ ) {
        if(current -> _next[word[i]] == nullptr) {
            nodes.push_back(TrieInfo(current, 1));
            nodes.back().newed = true;
            nodes.back().word = word[i];
            //current -> _next[word[i]] = new TrieNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.6f, 0.0f), new Camera(),word[i]);
            continue;
        }
        
        nodes.push_back(TrieInfo(current, 0));
        nodes.back().newed = true;
        nodes.back().word = word[i];
        
        current = current->_next[word[i]];
        current -> cnt[word[i]]++;
    }
    
}

bool Trie::subInsert(char word, TrieNode* &current, bool isLeaf) {
    bool newed = false;

    if(current -> _next[word] == nullptr) {
        current -> _next[word] = new TrieNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.6f, 0.0f), new Camera(),word);
        newed =  true;
    }
    current = current->_next[word];
    current -> cnt[word]++;
    
    current->cnt_Leaf[word]+= isLeaf;
    return newed;
}

void Trie::Initialize(std::string word) {
    if(root.empty() || root[currentVersion] == nullptr) {
        root.push_back(new TrieNode(glm::vec3(0.0f , 0.0f, 0.0f), glm::vec3(0.6f, 0.6f, 0.0f), camera, ' '));
        root.back() -> targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    TrieNode* current = root[currentVersion];
    
    for(int i = 0 ; i <= (int) word.size()-1 ; i++ ) {
        if(current -> _next[word[i]] == nullptr) {
            current -> _next[word[i]] = new TrieNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.6f, 0.0f), new Camera(),word[i]);
        }
        current = current->_next[word[i]];
        current -> cnt[word[i]]++;
    }
    current->cnt_Leaf[word[(int)word.size()-1]]++;
}

void Trie::Delete(std::string word , std::vector<TrieInfo> &subnodes) {
    if(root.empty() || root[currentVersion] == nullptr) {
        return;
    }else {
        root.push_back(nullptr);
        root[++currentVersion] = CopyNode(root[currentVersion - 1]);
    }
    TrieNode* current = root[currentVersion];
    std::vector<TrieNode*> nodes;
    for(int i = 0 ; i <= (int) word.size()-1 ; i++ ) {
        if(!current) return;
        current = current->_next[word[i]];
    }

    current = root[currentVersion];

    for(int i = 0 ; i <= (int)word.size()-1 ; i++) {
        subnodes.push_back(TrieInfo(current, 0));
        current = current -> _next[word[i]];
    }
    subnodes.push_back(TrieInfo(current, 2));

    
    if(current -> cnt_Leaf[word[(int)word.size()-1]] == 0) {
        return;
    }
    current -> cnt_Leaf[word[(int)word.size()-1]]--;
    
    for(int i = (int)nodes.size()-2 ; i >= 0 ; i--) {
        nodes[i+1] -> cnt[word[i+1]]--;
        if(nodes[i+1] -> cnt[word[i+1]] == 0) {
            //delete nodes[i+1];
            nodes[i] -> _next[word[i+1]] = nullptr;
        }
    }
    nodes[0] -> cnt[word[0]]--;
    current = root[currentVersion];
    if(current -> _next[word[0]] -> cnt[word[0]] == 0) {
        //delete current -> _next[word[0]];
        current -> _next[word[0]] = nullptr;
    }
    
}

void Trie::subDelete(char word, TrieNode* &current, bool isLeaf) {
    current -> cnt[word]--;
    current -> cnt_Leaf[word] -= isLeaf;
}

bool Trie::search(std::string word , std::vector<TrieInfo> &nodes) {
    TrieNode* current = root[currentVersion];
    for(int i = 0 ; i <= (int) word.size()-1 ; i++ ) {
        if(current->_next[word[i]] == nullptr) {
            return false;
        }
        nodes.push_back(TrieInfo(current, 0));
        nodes.back().found = true;
        nodes.back().word = word[i];
        current = current->_next[word[i]];
    }
    nodes.push_back(TrieInfo(current, (current->cnt_Leaf[word[(int)word.size()-1]] > 0) ? 3 : 0));
    nodes.back().found = true;
    return current->cnt_Leaf[word[(int)word.size()-1]] > 0;
}

void Trie::RecalculatePosition() {
    root[currentVersion] -> targetPosition = glm::vec3(0.0f, 0.94f, 0.0f);
    root[currentVersion] -> RecalculatePosition(root[currentVersion], 0, 0);
}

void Trie::Update(float deltaTime, float MouseX, float MouseY) {
    if(!root.empty() && root[currentVersion] != nullptr) {
        TrieNode::isOver = false;
        root[currentVersion] -> Update(deltaTime, MouseX, MouseY);
    }
}

void Trie::Draw() {
    if( !root.empty() && root[currentVersion] != nullptr) {
        root[currentVersion] -> Draw();
    }
}

bool Trie::Empty() {
    return root.empty() || root[currentVersion] == nullptr;
}