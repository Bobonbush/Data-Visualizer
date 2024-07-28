#include "AVL.h"


AVL::AVL() {
    root = nullptr;
}

AVL::~AVL() {
    delete root;
}

Node* AVL::insert(Node* node , int value) {
    if(node == nullptr) {
        return new Node(glm::vec3(0.0f), glm::vec3(0.0f), nullptr, value);             // Careful
    }
    if(value < node -> value) {
        node -> left = insert(node -> left, value);
    } else if(value > node -> value) {
        node -> right = insert(node -> right, value);
    } else {
        return node;
    }
    node -> height = 1 + std::max(node -> getHeight(node -> left), node -> getHeight(node -> right));
    int balance = node -> getBalance(node);
    if(balance > 1 && value < node -> left -> value) {
        return node -> rightRotate(node);
    }
    if(balance < -1 && value > node -> right -> value) {
        return node -> leftRotate(node);
    }
    if(balance > 1 && value > node -> left -> value) {
        node -> left = node -> leftRotate(node -> left);
        return node -> rightRotate(node);
    }
    if(balance < -1 && value < node -> right -> value) {
        node -> right = node -> rightRotate(node -> right);
        return node -> leftRotate(node);
    }
    return node;
}

void AVL::insert(int value) {
    root = insert(root, value);
}

Node* AVL::deleteNode(Node* node, int value) {
    if(node == nullptr) {
        return node;
    }
    if(value < node -> value) {
        node -> left = deleteNode(node -> left, value);
    } else if(value > node -> value) {
        node -> right = deleteNode(node -> right, value);
    } else {
        if(node -> left == nullptr || node -> right == nullptr) {
            Node* temp = node -> left ? node -> left : node -> right;
            if(temp == nullptr) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            delete temp;
        } else {
            Node* temp = minValueNode(node -> right);
            node -> value = temp -> value;
            node -> right = deleteNode(node -> right, temp -> value);
        }
    }
    if(node == nullptr) {
        return node;
    }
    node -> height = 1 + std::max(node -> getHeight(node -> left), node -> getHeight(node -> right));
    int balance = node -> getBalance(node);
    if(balance > 1 && node -> getBalance(node -> left) >= 0) {
        return node -> rightRotate(node);
    }
    if(balance > 1 && node -> getBalance(node -> left) < 0) {
        node -> left = node -> leftRotate(node -> left);
        return node -> rightRotate(node);
    }
    if(balance < -1 && node -> getBalance(node -> right) <= 0) {
        return node -> leftRotate(node);
    }
    if(balance < -1 && node -> getBalance(node -> right) > 0) {
        node -> right = node -> rightRotate(node -> right);
        return node -> leftRotate(node);
    }
    return node;
}

void AVL::deleteNode(int value) {
    root = deleteNode(root, value);
}

Node* AVL::minValueNode(Node* node) {
    Node* current = node;
    while(current -> left != nullptr) {
        current = current -> left;
    }
    return current;
}

void AVL::Draw() {
    if(root != nullptr) {
        root -> Draw();
    }
}

void AVL::Update(float mouseX, float mouseY) {
    if(root) {
        root -> Update(mouseX, mouseY);
    }
}

Node* AVL::search(Node* node, int value) {
    if(node == nullptr || node -> value == value) {
        return node;
    }
    if(node -> value < value) {
        return search(node -> right, value);
    }
    return search(node -> left, value);
}


void AVL::search(int value) {
    Node* node = search(root, value);
    
}

void AVL::PreOrder(Node* node) {
    if(node != nullptr) {
        node -> Draw();
        PreOrder(node -> left);
        PreOrder(node -> right);
    }
}

bool AVL::Empty() {
    return root == nullptr;
}

void AVL::PreOrder() {
    PreOrder(root);
}


void AVL::InOrder(Node* node) {
    if(node != nullptr) {
        InOrder(node -> left);
        node -> Draw();
        InOrder(node -> right);
    }
}

void AVL::InOrder() {
    InOrder(root);
}

void AVL::PostOrder(Node* node) {
    if(node != nullptr) {
        PostOrder(node -> left);
        PostOrder(node -> right);
        node -> Draw();
    }
}

void AVL::PostOrder() {
    PostOrder(root);
}
