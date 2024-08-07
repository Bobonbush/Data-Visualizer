#include "AVL.h"


AVL::AVL(Camera* _camera) {
    root = nullptr;
    camera = _camera;
}

AVL::~AVL() {
    delete root;
}

Node* AVL::insert(Node* node ,Node* parent, int value, std::vector<NodeInfo> & nodes) {
    if(node == nullptr) {
        totalNodes++;
        Node* temp = new Node(glm::vec3(0.0f, -0.95f, 0.0f ), glm::vec3(0.6f , 0.6f, 0.f), camera, value);
        temp -> targetPosition = glm::vec3(0.0f, -0.95f, 0.0f);
        temp -> status = -1;
        nodes.push_back(NodeInfo(temp, 1));
        return temp ;             // Careful
    }
    nodes.push_back(NodeInfo(node, 0));

    if(value < node -> value) {
        node -> left = insert(node -> left,node, value, nodes);
    } else if(value > node -> value) {
        node -> right = insert(node -> right,node, value, nodes);
    } else {
        return node;
    }
    node -> height = 1 + std::max(node -> getHeight(node -> left), node -> getHeight(node -> right));
    int balance = node -> getBalance(node);
    if(balance > 1 && value < node -> left -> value) {
        return node -> rightRotate(node);
        
        return node;
    }
    if(balance < -1 && value > node -> right -> value) {
        
        return node -> leftRotate(node);
        return node;
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

void AVL::insert(int value, std::vector<NodeInfo> & nodes) {
    root = insert(root,root, value, nodes);
}

void AVL :: RecalculatePosition() {
    if(root != nullptr) {
        root -> targetPosition = glm::vec3(0.0f, 0.74f, 0.0f);
        root -> RecalculatePosition(offsetx  * totalNodes, offsety * totalNodes);
    }
}

Node* AVL::deleteNode(Node* node, int value, std::vector<NodeInfo> & nodes) {
    if(node == nullptr) {
        return node;
    }
    if(value < node -> value) {
        node -> left = deleteNode(node -> left, value, nodes);
        
    } else if(value > node -> value) {
        node -> right = deleteNode(node -> right, value, nodes);
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
            node ->text = temp -> text;
            node -> right = deleteNode(node -> right, temp -> value, nodes);
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

void AVL::deleteNode(int value, std::vector<NodeInfo> & nodes) {
    root = deleteNode(root, value, nodes);
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

void AVL::Update(float deltaTime, float mouseX, float mouseY) {
    
    if(root) {
        Node::isOver = false;
        root -> Update(deltaTime, mouseX, mouseY);
    }
}

Node* AVL::search(Node* node, int value, std::vector<NodeInfo> & nodes) {
    if(node == nullptr) return node;
    if(node -> value == value) {
        nodes.push_back(NodeInfo(node, 3));

        return node;
    }
    nodes.push_back(NodeInfo(node, 0));
    if(node -> value < value) {
        return search(node -> right, value, nodes);
    }
    return search(node -> left, value , nodes);
}


void AVL::search(int value, std::vector<NodeInfo> & nodes) {
    Node* node = search(root, value, nodes);
    
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
