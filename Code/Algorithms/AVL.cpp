#include "AVL.h"


AVL::AVL(Camera* _camera) {
    root.clear();
    root.push_back(nullptr);
    currentVersion = 0;
    camera = _camera;
}

AVL::~AVL() {
    for(int i = 0 ; i < (int) root.size(); i++) {
        if(root[i] != nullptr) {
            delete root[i];
        }
    }
}

void AVL::Reset(Node * node) {
    node -> traverse = false;
    node -> Done = false;
    node -> status = -1;
    if(node -> left != nullptr) {
        Reset(node -> left);

    }
    if(node -> right != nullptr) {
        Reset(node -> right);
    }
}

void AVL::Reset() {
    Reset(root[currentVersion]);
}

Node* AVL::insert(Node* node ,Node* parent, int value) {
    
    if(node == nullptr) {
        totalNodes++;
        Node* temp = new Node(glm::vec3(0.0f, -0.95f, 0.0f ), glm::vec3(0.45f, 0.45f, 0.0f), camera, value);
        temp -> targetPosition = glm::vec3(0.0f, -0.95f, 0.0f);
        temp -> status = 1;
        temp -> Done = true;
        return temp ;             // Careful
    }
    node -> status = 0;

    if(value < node -> value && node -> Done == false) {
        if(node -> left != nullptr && node -> left -> traverse == false) {
            node -> left -> traverse = true;
            return node;
        } 
        if(node -> left != nullptr && node -> left -> Done == true) {
            node -> Done = true;
        }else
        {
            node -> left = insert(node -> left,node, value);
        }
    } else if(value > node -> value && node -> Done == false) {
        if(node -> right != nullptr && node -> right -> traverse == false) {
            node -> right -> traverse = true;
            return node;
        }
        if(node -> right != nullptr && node -> right -> Done == true) {
            node -> Done = true;
        } else {
            node -> right = insert(node -> right,node, value);
        }
    } else {
        node -> Done = true;
        return node;
    }
    if(node -> Done == false) {
        return node;
    }
    node -> status = -1;
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

Node * AVL::Initialize(Node* node, int value) {
    if(node == nullptr) {
        totalNodes++;
        Node* temp = new Node(glm::vec3(0.0f, -0.95f, 0.0f ), glm::vec3(0.45f, 0.45f, 0.0f), camera, value);
        temp -> targetPosition = glm::vec3(0.0f, -0.95f, 0.0f);
        temp -> status = -1;
        return temp ;             // Careful
    }

    if(value < node -> value ) {
        node -> left = Initialize(node -> left, value);
    } else if(value > node -> value) {
        node -> right = Initialize(node -> right, value);
        
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


Node* AVL :: CopyNode(Node* node) {
    if(!node) return nullptr;
    else {
        Node * newNode = new Node(node -> position, glm::vec3(0.45f, 0.45f, 0.0f), camera, node -> value);
        newNode -> left = CopyNode(node -> left);
        newNode -> right = CopyNode(node -> right);
        newNode -> targetPosition = node -> targetPosition;
        newNode -> height = node -> height;
        return newNode;
    } 
}

void AVL::insert(int value) {
    root[currentVersion] = insert(root[currentVersion],root[currentVersion], value);
}
void AVL::Initialize(int value ) {
    root[currentVersion] = Initialize(root[currentVersion], value);
}

Info AVL::calculateSubTreeWidth(Node* node, float offsetX) {
    Info info = {0, 0};
    if(node == nullptr) {
        return info;
    }
    float totalWidth = 0;
    float maxOffset = 0.f;

    Info leftInfo = calculateSubTreeWidth(node -> left, offsetX);
    totalWidth += leftInfo.width + offsetX;
    maxOffset = std::max(maxOffset, leftInfo.xOffset);
    Info rightInfo = calculateSubTreeWidth(node -> right, offsetX);

    totalWidth += rightInfo.width + offsetX;
    maxOffset = std::max(maxOffset, rightInfo.xOffset);

    totalWidth = std::max(totalWidth, offsetX);

    float xOffset = totalWidth / 2;

    return {totalWidth, xOffset};
}

void AVL::RecalculatePosition(Node* node , float x , float y , float offset_x, float offset_y) {
    if(!node)return ;
    Info info = calculateSubTreeWidth(node, offset_x);
    node -> targetPosition = glm::vec3(x, y, 0.0f);

    float childX = x - info.width / 2.0f;
    if(node -> left != nullptr) {
        Info childInfo = calculateSubTreeWidth(node -> left, offset_x);
        RecalculatePosition(node -> left, childX + childInfo.xOffset, y - offset_y, offset_x, offset_y);
        childX += childInfo.width + offset_x;
    }

    if(node -> right != nullptr) {
        Info childInfo = calculateSubTreeWidth(node -> right, offset_x);
        RecalculatePosition(node -> right, childX + childInfo.xOffset, y - offset_y, offset_x, offset_y);
        
    }
}
void AVL :: RecalculatePosition() {
    RecalculatePosition(root[currentVersion], 0.0f, 0.75f, offsetx, offsety);
}

Node* AVL::deleteNode(Node* node, int value) {
    if(node == nullptr) {
        return node;
    }
    node -> status = 0;
    if(value < node -> value) {
        if(node -> left != nullptr && node -> left -> traverse == false) {
            node -> left -> traverse = true;
            return node;
        }
        if((node -> left != nullptr && node -> left -> Done == true) || node -> left == nullptr) {
            node -> Done = true;
        } else {
            node -> left = deleteNode(node -> left, value);
        }
    } else if(value > node -> value) {
        if(node -> right != nullptr && node -> right -> traverse == false) {
            node -> right -> traverse = true;
            return node;
        }
        if((node -> right != nullptr && node -> right -> Done == true) || node -> right == nullptr) {
            node -> Done = true;
            
        } else {
            node -> right = deleteNode(node -> right, value);
        }
    } else {
        
        if(node -> left == nullptr || node -> right == nullptr) {
            Node* temp = node -> left ? node -> left : node -> right;
            if(temp == nullptr) {
                temp = node;
                node = nullptr;
                temp -> Done = true;
                //node -> text = "";
                //node -> value = 0;
            } else {
                *node = *temp;
                node -> status = 3;
            }
            deleted = true;
            delete temp;
        } else {
            
            Node* temp = minValueNode(node -> right);
            int value = temp -> value;
            std::string text = temp -> text;
            temp -> status = 2;
            
              
            
            //node -> right -> traverse = true;
            node -> right = deleteNode(node -> right, temp -> value);
            if(  node-> right == nullptr || node -> right -> Done || deleted) {
                node -> Done = true;
                node -> value = value;
                node -> text = text;
            }
            
        }
        //if(node != nullptr) node -> Done = true;
    }
    if(node == nullptr) {
        return node;
    }
    if(node -> Done == false) {
        return node;
    }
    node -> status = -1;

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
    root[currentVersion] = deleteNode(root[currentVersion], value);
}

Node* AVL::minValueNode(Node* node) {
    Node* current = node;
    while(current -> left != nullptr) {
        current = current -> left;
    }
    return current;
}

void AVL::Draw() {
    if(root[currentVersion] != nullptr) {
        root[currentVersion] -> Draw();
    }
}

void AVL::Update(float deltaTime, float mouseX, float mouseY) {
    
    if(root[currentVersion]) {
        Node::isOver = false;
        root[currentVersion] -> Update(deltaTime, mouseX, mouseY);
    }
}

Node* AVL::search(Node* node, int value) {
    if(node == nullptr) {
        return node;
    }
    if(node -> value == value) {
        node -> Done = true;
        node -> status = 3;
        return node;
    }
    node -> status = 0;
    if(node -> value < value && node -> Done == false) {
        if(node -> right != nullptr && node -> right -> traverse == false) {
            node -> right -> traverse = true;
            return node;
        }
        if(node -> right != nullptr && node -> right -> Done == true) {
            node -> Done = true;
        } else {
            return search(node -> right, value);
        }
    }

    if(node -> value > value && node -> Done == false) {
        if(node -> left != nullptr && node -> left -> traverse == false) {
            node -> left -> traverse = true;
            return node;
        }
        if(node -> left != nullptr && node -> left -> Done == true) {
            node -> Done = true;
        } else {
            Node *lmao = search(node -> left, value);
            if(lmao == nullptr) {
                node -> Done = true;
                return node;
            }
            return lmao;
        }
    }
    return node;
}


void AVL::search(int value) {
    Node* node = search(root[currentVersion], value);
    
}

void AVL::PreOrder(Node* node) {
    if(node != nullptr) {
        node -> Draw();
        PreOrder(node -> left);
        PreOrder(node -> right);
    }
}

bool AVL::Empty() {
    return root.empty()  || root[currentVersion] == nullptr;
}

void AVL::PreOrder() {
    PreOrder(root[currentVersion]);
}


void AVL::InOrder(Node* node) {
    if(node != nullptr) {
        InOrder(node -> left);
        node -> Draw();
        InOrder(node -> right);
    }
}

void AVL::InOrder() {
    InOrder(root[currentVersion]);
}

void AVL::PostOrder(Node* node) {
    if(node != nullptr) {
        PostOrder(node -> left);
        PostOrder(node -> right);
        node -> Draw();
    }
}

void AVL::PostOrder() {
    PostOrder(root[currentVersion]);
}
