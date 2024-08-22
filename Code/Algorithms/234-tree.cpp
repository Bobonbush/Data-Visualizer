#include "234-tree.h"

BNode:: BNode(glm::vec3 _position, glm::vec3 _size , Camera * _camera)  : parent(nullptr) {
        position = _position;
        size = _size;
        
        camera = _camera;
        texture = TextureLoader::LoadTexture("box.png");
        textureDel = TextureLoader::LoadTexture("box_red.png");
        textureNew = TextureLoader::LoadTexture("box_new.png");
        textureRelated = TextureLoader::LoadTexture("box_traverse.png");
        textureFound = TextureLoader::LoadTexture("box_found.png");
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

BNode::~BNode() {
    for(auto child : children) {
        delete child;
    }
}

void BNode::Update(float deltaTime) {
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

void BNode::Draw()  {
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
        if(status == 0) {
            glBindTexture(GL_TEXTURE_2D, textureRelated);
        }else if(status == 1) {
            glBindTexture(GL_TEXTURE_2D, textureNew);
        }else if(status == 2) {
            glBindTexture(GL_TEXTURE_2D, textureDel);
        }else if(status == 3) {
            glBindTexture(GL_TEXTURE_2D, textureFound);
        }else {
            glBindTexture(GL_TEXTURE_2D, texture);
        }
        
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


void Tree234::split(BNode * node) {
    if(node -> traverse == true) {
        return ;
    }
    node -> traverse = true;
    node -> status = 1;
    BNode * parent = node->parent;

    BNode * leftChild = new BNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.45f, 0.45f, 0.45f), camera);
    BNode * rightChild = new BNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.45f, 0.45f, 0.45f) , camera);
    leftChild->keys.push_back(node->keys[0]);
    rightChild->keys.push_back(node->keys[2]);

    if(!node->isLeaf()) {
        leftChild->children.push_back(node->children[0]);
        leftChild->children.push_back(node->children[1]);

        rightChild->children.push_back(node->children[2]);
        rightChild->children.push_back(node->children[3]);

        leftChild->children[0]->parent = leftChild;
        leftChild->children[1]->parent = leftChild;

        rightChild->children[0]->parent = rightChild;
        rightChild->children[1]->parent = rightChild;
    }

    if(parent == nullptr) {
        root[currentVersion] = new BNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.45f, 0.45f, 0.45f), camera);
        root[currentVersion] -> insertKey(node->keys[1]);
        root[currentVersion] -> children.push_back(leftChild);

        root[currentVersion] -> children.push_back(rightChild);

        leftChild->parent = root[currentVersion];

        rightChild->parent = root[currentVersion];
    }else {
        parent -> insertKey(node->keys[1]);
        auto pos = std::find(parent -> children.begin(), parent -> children.end() , node);
        if(pos != parent -> children.end()) {
            *pos = leftChild;
            parent -> children.insert(pos + 1, rightChild);
        }
        leftChild -> parent = parent;
        rightChild -> parent = parent;
    }
}

void Tree234::instantSplit(BNode * node) {
    BNode * parent = node->parent;

    BNode * leftChild = new BNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.45f, 0.45f, 0.45f), camera);
    BNode * rightChild = new BNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.45f, 0.45f, 0.45f) , camera);
    leftChild->keys.push_back(node->keys[0]);
    rightChild->keys.push_back(node->keys[2]);

    if(!node->isLeaf()) {
        leftChild->children.push_back(node->children[0]);
        leftChild->children.push_back(node->children[1]);

        rightChild->children.push_back(node->children[2]);
        rightChild->children.push_back(node->children[3]);

        leftChild->children[0]->parent = leftChild;
        leftChild->children[1]->parent = leftChild;

        rightChild->children[0]->parent = rightChild;
        rightChild->children[1]->parent = rightChild;
    }

    if(parent == nullptr) {
        root[currentVersion] = new BNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.45f, 0.45f, 0.45f), camera);
        root[currentVersion] -> insertKey(node->keys[1]);
        root[currentVersion] -> children.push_back(leftChild);

        root[currentVersion] -> children.push_back(rightChild);

        leftChild->parent = root[currentVersion];

        rightChild->parent = root[currentVersion];
    }else {
        parent -> insertKey(node->keys[1]);
        auto pos = std::find(parent -> children.begin(), parent -> children.end() , node);
        if(pos != parent -> children.end()) {
            *pos = leftChild;
            parent -> children.insert(pos + 1, rightChild);
        }
        leftChild -> parent = parent;
        rightChild -> parent = parent;
    }
}

void Tree234 :: insertRecursively(BNode* node, int key) {
    if(node -> Done ) {
        node -> status = -1;
        return ;
    } 
    if(node -> traverse == false) {
        node -> traverse = true;
        node -> status = 0;
        return ;
    }

    if(node->isLeaf()) {
        node -> Done = true;
        node -> status = 1;
        node->insertKey(key);
        
    } else {
        for(int i = 0 ; i <  (int)node -> children.size() ; i++ ) {
            if(node -> children[i] -> isFull()) {
                split(node -> children[i]);

                if(key < node -> keys[0]) {
                    i = 0;
                }else if((int)node -> keys.size() == 2 && key >= node -> keys[1] ) {
                    i = 2;
                }else {
                    i = 1;
                }
            }
        }

        if(key < node -> keys[0]) {
            if(node -> children[0] -> Done == true) {
                node -> Done = true;
                node -> status = -1;
            }
            else
            {
                insertRecursively(node -> children[0], key);
            }
        }else if ( (int) node -> keys.size() == 1 || key < node -> keys[1]) {
            if(node -> children[1] -> Done == true) {
                node -> Done = true;
                node -> status = -1;
            }else {

                insertRecursively(node -> children[1], key);
            }
        }else if((int)node -> keys.size() == 2 || key < node -> keys[2]) {
            if(node -> children[2] -> Done == true) {
                node -> Done = true;
                node -> status = -1;
            }else {

            
                insertRecursively(node -> children[2], key);
            }
        }
        else {
            if(node -> children[3] -> Done == true) {
                node -> Done = true;
                node -> status = -1;
            }else {
                insertRecursively(node -> children[3], key);
            }
        }
    }
}

void Tree234 :: initializeRecursively(BNode* node, int key) {
    if(node->isLeaf()) {
        node->insertKey(key);
    } else {
        for(int i = 0 ; i <  (int)node -> children.size() ; i++ ) {
            if(node -> children[i] -> isFull()) {
                instantSplit(node -> children[i]);

                if(key < node -> keys[0]) {
                    i = 0;
                }else if((int)node -> keys.size() == 2 && key >= node -> keys[1] ) {
                    i = 2;
                }else {
                    i = 1;
                }
            }
        }

        if(key < node -> keys[0]) {
            initializeRecursively(node -> children[0], key);
        }else if ( (int) node -> keys.size() == 1 || key < node -> keys[1]) {
            initializeRecursively(node -> children[1], key);
        }else if((int)node -> keys.size() == 2 || key < node -> keys[2]) {
            initializeRecursively(node -> children[2], key);
        }
        else {

            initializeRecursively(node -> children[3], key);
        }
    }
}

void Tree234::Initialize(int key) {
    if(root[currentVersion] == nullptr) {
        root[currentVersion] = new BNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.45f, 0.45f, 0.45f), camera);
        root[currentVersion] -> targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        root[currentVersion] -> insertKey(key);
        
    }else {
        
        if(root[currentVersion] -> isFull()) {
            instantSplit(root[currentVersion]);
        }
        initializeRecursively(root[currentVersion], key);
    }
}

void Tree234:: insert(int key) {
    if(root[currentVersion] != nullptr && root[currentVersion] -> Done == true) {
        return ;
    }
    if(root[currentVersion] == nullptr) {
        root[currentVersion] = new BNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.45f, 0.45f, 0.45f), camera);
        root[currentVersion] -> status = 0;
        root[currentVersion] -> targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        root[currentVersion] -> insertKey(key);
        root[currentVersion] -> Done = true;
    }else {
        if(root[currentVersion] -> isFull()) {
            split(root[currentVersion]);
        }
        
        insertRecursively(root[currentVersion], key);
    }
}

void Tree234::deleteRecursively(BNode* node, int key) {
    if(node == nullptr) return ;
    auto it = std::find (node -> keys.begin() , node -> keys.end() , key);

    if(it != node -> keys.end()) {
        if(node -> isLeaf()) {
            node -> RemoveKey(key);
        }else {
            int index = it - node -> keys.begin();
            BNode * predecessor = getPredecessor(node, index);
            int predKey = predecessor -> keys.back();
            node -> keys[index] = predKey;
            deleteRecursively(predecessor, predKey);
        }
    }else {
        int childIndex = 0;
        while(childIndex < (int)node -> keys.size() && key > node -> keys[childIndex]) {
            childIndex++;
        }

        BNode* child = node -> children[childIndex];
        if((int)child -> keys.size() == 1) {
            handleUnderflow(node, childIndex);
        }

        if( childIndex < (int)node -> children.size()) {
            deleteRecursively(node -> children[childIndex], key);
        }
        
    }
}

BNode* Tree234::getPredecessor(BNode* node, int index) {
    BNode* current = node -> children[index];
    while(!current -> isLeaf()) {
        current = current -> children.back();
    }
    return current;
}

void Tree234::handleUnderflow(BNode* parent , int childIndex) {
    BNode * child = parent -> children[childIndex];

    if(childIndex > 0 && (int) parent -> children[childIndex - 1] -> keys.size() > 1) {
        BNode * leftSibling = parent -> children[childIndex - 1];
        child -> keys.insert(child -> keys.begin(), parent -> keys[childIndex - 1]);
        parent -> keys[childIndex - 1] = leftSibling -> keys.back();

        leftSibling -> keys.pop_back();

        if(!leftSibling -> isLeaf()) {
            child -> children.insert(child -> children.begin(), leftSibling -> children.back());
            leftSibling -> children.pop_back();
        }
    }else if (childIndex < parent -> keys.size() && parent -> children[childIndex + 1] -> keys.size() > 1 ) {
        BNode* rightSibling = parent->children[childIndex + 1];
        child->keys.push_back(parent->keys[childIndex]);
        parent->keys[childIndex] = rightSibling->keys.front();
        rightSibling->keys.erase(rightSibling->keys.begin());

        if (!rightSibling->isLeaf()) {
            child->children.push_back(rightSibling->children.front());
            rightSibling->children.erase(rightSibling->children.begin());
        }
    }else {
        if(childIndex > 0) {
            MergeNodes(parent, childIndex - 1);
        }else {
            MergeNodes(parent, childIndex);
        }
    }
}

void Tree234::MergeNodes(BNode* parent, int index) {
     BNode* leftChild = parent->children[index];
     BNode* rightChild = parent->children[index + 1]; 

     leftChild->keys.push_back(parent->keys[index]);
     leftChild->keys.insert(leftChild->keys.end(), rightChild->keys.begin(), rightChild->keys.end()); 
     if (!rightChild->isLeaf()) {
         leftChild->children.insert(leftChild->children.end(), rightChild->children.begin(), rightChild->children.end());
     } 
     
     parent->keys.erase(parent->keys.begin() + index);
     parent->children.erase(parent->children.begin() + index + 1 );
     delete rightChild;
        
     
     
     if (parent->keys.empty()) {
         root[currentVersion] = leftChild;
         root[currentVersion]->parent = nullptr;
         delete parent;
      }
      
     
}

void Tree234::remove(int key) {
    if(root[currentVersion] == nullptr) return ;

    deleteRecursively(root[currentVersion], key);

    if(root[currentVersion]->keys.empty()) {
        BNode* temp = root[currentVersion];
        if(root[currentVersion]->isLeaf()) {
            root[currentVersion] = nullptr;
        }else {
            
            root[currentVersion] = root[currentVersion]->children[0];
            root[currentVersion]->parent = nullptr;
        }
        //delete temp;
    }
}



Tree234::Tree234 (Camera * _camera) {
    camera = _camera;
    root.push_back(nullptr);
    
}

Tree234::~Tree234() {
    for(auto node : root) {
        delete node;
    }
}

BNode* Tree234::CopyNode(BNode * node, BNode* parent) {
    if(node == nullptr) return nullptr;
    BNode * newNode = new BNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.45f, 0.45f, 0.45f), camera);
    newNode -> keys = node -> keys;
    newNode -> targetPosition = node -> targetPosition;
    newNode -> position = node -> position;
    newNode -> parent = parent;
    
    for(auto child : node -> children) {
        BNode * newChild = CopyNode(child, newNode);
        newNode -> children.push_back(newChild);

    }
    return newNode;
}

void Tree234::Update(float deltaTime) {
    if(root[currentVersion] != nullptr) {
        root[currentVersion] -> Update(deltaTime);
    }
}

void Tree234::Draw() {
    if(root[currentVersion] != nullptr) {
        root[currentVersion] -> Draw();
    }
}

SubTreeInfo Tree234::calculateSubTreeWidth(BNode * node, float offsetX) {
    if(node == nullptr) return {0.f, 0.f};

    float totalWidth = 0.0f;
    float maxOffset = 0.0f;

    for(auto child : node -> children) {
        SubTreeInfo childInfo = calculateSubTreeWidth(child, offsetX);
        totalWidth += childInfo.width + offsetX;
        maxOffset = std::max(maxOffset, childInfo.xOffset);
    }

    totalWidth = std::max(totalWidth - offsetX , offsetX);

    float xOffset = totalWidth / 2.0f;

    return {totalWidth, xOffset};
}

void Tree234::RecalculatePosition(BNode * node, float x , float y , float offset_x , float offset_y) {
    if(!node) return ;

    SubTreeInfo info = calculateSubTreeWidth(node, offset_x);
    node -> targetPosition = glm::vec3(x, y, 0.0f);
    
    float childX = x - info.width / 2.0f;
    
    for (auto child : node -> children) {
       SubTreeInfo childInfo = calculateSubTreeWidth(child, offset_x);
       RecalculatePosition(child, childX + childInfo.xOffset, y - offset_y, offset_x, offset_y);
       childX += childInfo.width + offset_x; 
    }
}

void Tree234::RecalculatePosition() {
    if(root[currentVersion] != nullptr) {
        RecalculatePosition(root[currentVersion], 0.0f, 0.3f, 0.06f, 0.3f);
    }
}
\
bool Tree234::isEmpty() {
    return root.empty() || root[currentVersion] == nullptr;
}



void Tree234::FindNode(BNode * node , int key) {
    if(node == nullptr) return ;
    if(node -> traverse == false || node -> Done == true) {
        node -> traverse = true;
        node -> status = 0;
        return ;
    }
    auto it = std::find(node -> keys.begin(), node -> keys.end(), key);
    if(it != node -> keys.end()) {
        node -> Done = true;
        node -> status = 3;
        return ;
    }else {
        int childIndex = 0;
        while(childIndex < (int)node -> keys.size() && key > node -> keys[childIndex]) {
            childIndex++;
        }

        BNode * child = node -> children[childIndex];
        if(child == nullptr){
            node -> Done = true;
            return ;
        }
        if(child -> Done == true) {
            node -> Done = true;
        }else
        {
            FindNode(child, key);
        }
    }
}

void Tree234::Reset(BNode * node) {
    if(node == nullptr) return ;
    node -> Done = false;
    node -> traverse = false;
    node -> status = -1;
    for(auto child : node -> children) {
        Reset(child);
    }
}

void Tree234::Reset() {
    if(root[currentVersion] != nullptr) {
        Reset(root[currentVersion]);
    }
}