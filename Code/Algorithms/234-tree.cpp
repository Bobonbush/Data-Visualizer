#include "234-tree.h"

void Tree234::split(BNode * node) {
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
    if(node->isLeaf()) {
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
            insertRecursively(node -> children[0], key);
        }else if ( (int) node -> keys.size() == 1 || key < node -> keys[1]) {
            insertRecursively(node -> children[1], key);
        }else if((int)node -> keys.size() == 2 || key < node -> keys[2]) {
            insertRecursively(node -> children[2], key);
        }
        else {

            insertRecursively(node -> children[3], key);
        }
    }
}

void Tree234:: insert(int key) {
    if(root[currentVersion] == nullptr) {
        root[currentVersion] = new BNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.45f, 0.45f, 0.45f), camera);
        root[currentVersion] -> targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        root[currentVersion] -> insertKey(key);
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

        if(childIndex == (int)node -> keys.size()) {
            childIndex--;
        }

        BNode* child = node -> children[childIndex];
        if(child == nullptr) return ;
        if((int)child -> keys.size() == 1) {
            handleUnderflow(node, childIndex);
        }

        deleteRecursively(child, key);
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
     parent->children.erase(parent->children.begin() + index + 1); 
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
        delete temp;
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
        RecalculatePosition(root[currentVersion], 0.0f, 0.3f, 0.06f, 0.2f);
    }
}

