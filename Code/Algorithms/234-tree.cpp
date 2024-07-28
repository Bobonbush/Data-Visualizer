#include "234-tree.h"

BTree::BTree(Camera * _camera, int _degree) {
    root = nullptr;
    camera = _camera;
    degree = _degree;
}

BTree::~BTree() {
    delete root;
}


void BTree::Draw() {
    if(root != nullptr) {
        root -> Draw();
    }
}

void BTree::Update(float MouseX , float MouseY) {
    if(root != nullptr) {
        root -> Update(MouseX, MouseY);
    }
}

void BTree::traverse() {
    if(root != nullptr) {
        root -> traverse();
    }
}

BNode* BTree::Search(int value) {
    if(root != nullptr) {
        return root -> search(value);
    }
    return nullptr;
}

void BTree::insert(int value) { 
    if(root == nullptr) {
        root = new BNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), camera, degree);
        root -> key.push_back(value);
        root -> n = 1;
    } else {
        if(root -> n == 2 * degree - 1) {
            BNode* s = new BNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), camera, degree);
            s -> child.push_back(root);
            s -> splitChild(0, root);
            int i = 0;
            if(s -> key[0] < value) {
                i++;
            }
            s -> child[i] -> insertNonFull(value);
            root = s;
        } else {
            root -> insertNonFull(value);
        }
    }
}

void BTree::Delete(int value) {
    if(root == nullptr) {
        return;
    }
    root -> remove(value);
    if(root -> n == 0) {
        BNode* temp = root;
        if(root -> isLeaf) {
            root = nullptr;
        } else {
            root = root -> child[0];
        }
        delete temp;
    }
}

