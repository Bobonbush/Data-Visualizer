#include "node.h"

Node::Node(glm::vec3 _position, glm::vec3 _size, Camera * _camera, int _value) {
    value = _value;
    left = nullptr;
    right = nullptr;
    height = 1;

    position = _position;
    size = _size;
    camera = _camera;
    shader = new Shader("button.vs", "button.fs");

    float width = camera -> width;
    float height = camera -> height;
    float texture_width = TextureLoader::GetTextureSize("node.png").first;
    float texture_height = TextureLoader::GetTextureSize("node.png").second;
    
}

Node::~Node() {
    delete left;
    delete right;
}


int Node::getHeight(Node* node) {
    if(node == nullptr) {
        return 0;
    }
    return node -> height;
}


int Node::getBalance(Node* node) {
    if(node == nullptr) {
        return 0;
    }
    return getHeight(node -> left) - getHeight(node -> right);
}

Node* Node::rightRotate(Node* y) {
    Node* x = y -> left;
    Node* T2 = x -> right;

    x -> right = y;
    y -> left = T2;

    y -> height = 1 + std::max(getHeight(y -> left), getHeight(y -> right));
    x -> height = 1 + std::max(getHeight(x -> left), getHeight(x -> right));

    return x;
}

Node* Node::leftRotate(Node* x) {
    Node* y = x -> right;
    Node* T2 = y -> left;

    y -> left = x;
    x -> right = T2;

    x -> height = 1 + std::max(getHeight(x -> left), getHeight(x -> right));
    y -> height = 1 + std::max(getHeight(y -> left), getHeight(y -> right));

    return y;
}


void Node::Update(float mouseX, float mouseY) {
    isHovered = isInside(mouseX, mouseY);
}

void Node::Draw() {

    shader -> use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    shader -> setMat4("projection", projection);
    shader -> setMat4("view", view);
    shader -> setMat4("model", glm::mat4(1.0f));
    glBindVertexArray(VAO);

    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

bool Node::isInside(float mouseX, float mouseY) {
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

    if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height) {
        return true;
    }
    return false;
}


BNode::BNode(glm::vec3 _position, glm::vec3 _size, Camera * _camera ,int _degree) {
    position = _position;
    size = _size;
    camera = _camera;
    shader = new Shader("button.vs", "button.fs");

    float width = camera -> width;
    float height = camera -> height;
    float texture_width = TextureLoader::GetTextureSize("node.png").first;
    float texture_height = TextureLoader::GetTextureSize("node.png").second;
    texture = TextureLoader::LoadTexture("node.png");
    key.assign(_degree * 2 - 1, -1);
    child.resize(_degree * 2);
    degree = _degree;
}

BNode::~BNode() {
    for (int i = 0; i < (int) child.size(); i++) {
        delete child[i];
    }
    delete shader;
}

void BNode::Draw() {
    shader -> use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    shader -> setMat4("projection", projection);
    shader -> setMat4("view", view);
    shader -> setMat4("model", model);
    glBindVertexArray(VAO);

    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    for(int i = 0 ; i < (int) child.size() ; i++) {
        child[i] -> Draw();
    }
}

void BNode::Update(float mouseX, float mouseY) {
    isHovered = isInside(mouseX, mouseY);
}

bool BNode::isInside(float mouseX, float mouseY) {
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

    if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height) {
        return true;
    }
    return false;
}

void BNode::traverse() { 
    int i; 
    for (i = 0; i < n; i++) { 
        if (isLeaf == false) 
            child[i] -> traverse(); 
        std::cout << " " << key[i]; 
    } 
    if (isLeaf == false) 
        child[i] -> traverse(); 
}

BNode* BNode::search (int value) {
    int i = 0;
    while (i < n && value > key[i]) {
        i++;
    }
    if (key[i] == value) {
        return this;
    }
    if (isLeaf == true) {
        return nullptr;
    }
    return child[i] -> search(value);
}

void BNode::insertNonFull(int value) {

    int i = n - 1;
    if (isLeaf == true) {
        while (i >= 0 && key[i] > value) {
            key[i + 1] = key[i];
            i--;
        }
        key[i + 1] = value;
        n = n + 1;
    } else {
        while (i >= 0 && key[i] > value) {
            i--;
        }
        if (child[i + 1] -> n == 2 * degree - 1) {
            splitChild(i + 1, child[i + 1]);
            if (key[i + 1] < value) {
                i++;
            }
        }
        child[i + 1] -> insertNonFull(value);
    }

}

void BNode::splitChild(int i, BNode* y) {
    BNode* z = new BNode(glm::vec3(0.f), glm::vec3(0.f), camera, degree);
    z -> isLeaf = y -> isLeaf;
    z -> n = degree - 1;
    for (int j = 0; j < degree - 1; j++) {
        z -> key[j] = y -> key[j + degree];
    }
    if (y -> isLeaf == false) {
        for (int j = 0; j < degree; j++) {
            z -> child[j] = y -> child[j + degree];
        }
    }
    y -> n = degree - 1;
    for (int j = n; j >= i + 1; j--) {
        child[j + 1] = child[j];
    }
    child[i + 1] = z;
    for (int j = n - 1; j >= i; j--) {
        key[j + 1] = key[j];
    }
    key[i] = y -> key[degree - 1];
    n = n + 1;
}

int BNode::findKey(int value) {
    int idx = 0;
    while (idx < n && key[idx] < value) {
        ++idx;
    }
    return idx;
}

void BNode:: remove(int value) {
    int idx = findKey(value);
    if (idx < n && key[idx] == value) {
        if (isLeaf) {
            removeFromLeaf(idx);
        } else {
            removeFromNonLeaf(idx);
        }
    } else {
        if (isLeaf) {
            std::cout << "The value " << value << " is not present in the tree\n";
            return;
        }
        bool flag = ((idx == n) ? true : false);
        if (child[idx] -> n < degree) {
            fill(idx);
        }
        if (flag && idx > n) {
            child[idx - 1] -> remove(value);
        } else {
            child[idx] -> remove(value);
        }
    }
}

void BNode::removeFromLeaf(int idx) {
    for (int i = idx + 1; i < n; ++i) {
        key[i - 1] = key[i];
    }
    n--;
}

void BNode::removeFromNonLeaf(int idx) {
    int value = key[idx];
    if (child[idx] -> n >= degree) {
        int pred = getPred(idx);
        key[idx] = pred;
        child[idx] -> remove(pred);
    } else if (child[idx + 1] -> n >= degree) {
        int succ = getSucc(idx);
        key[idx] = succ;
        child[idx + 1] -> remove(succ);
    } else {
        merge(idx);
        child[idx] -> remove(value);
    }
}

int BNode::getPred(int idx) {
    BNode* cur = child[idx];
    while (!cur -> isLeaf) {
        cur = cur -> child[cur -> n];
    }
    return cur -> key[cur -> n - 1];
}

int BNode::getSucc(int idx) {
    BNode* cur = child[idx + 1];
    while (!cur -> isLeaf) {
        cur = cur -> child[0];
    }
    return cur -> key[0];
}

void BNode::fill(int idx) {
    if (idx != 0 && child[idx - 1] -> n >= degree) {
        borrowFromPrev(idx);
    } else if (idx != n && child[idx + 1] -> n >= degree) {
        borrowFromNext(idx);
    } else {
        if (idx != n) {
            merge(idx);
        } else {
            merge(idx - 1);
        }
    }
}

void BNode::borrowFromPrev(int idx) {
    BNode* childs = child[idx];
    BNode* childr = child[idx - 1];
    for (int i = childs -> n - 1; i >= 0; --i) {
        childs -> key[i + 1] = childs -> key[i];
    }
    if (!childs -> isLeaf) {
        for (int i = childs -> n; i >= 0; --i) {
            childs -> child[i + 1] = childs -> child[i];
        }
    }
    childs -> key[0] = key[idx - 1];
    if (!childs -> isLeaf) {
        childs -> child[0] = childr -> child[childr -> n];
    }
    key[idx - 1] = childr -> key[childr -> n - 1];
    
    childs -> n += 1;
    childr -> n -= 1;

}

void BNode::borrowFromNext(int idx) {
    BNode* childs = child[idx];
    BNode* childr = child[idx + 1];
    childs -> key[(childs -> n)] = key[idx];
    if (!(childs -> isLeaf)) {
        childs -> child[(childs -> n) + 1] = childr -> child[0];
    }
    key[idx] = childr -> key[0];
    for (int i = 1; i < childr -> n; ++i) {
        childr -> key[i - 1] = childr -> key[i];
    }
    if (!childr -> isLeaf) {
        for (int i = 1; i <= childr -> n; ++i) {
            childr -> child[i - 1] = childr -> child[i];
        }
    }
    childs -> n += 1;
    childr -> n -= 1;
}

void BNode::merge(int idx) {
    BNode* childs = child[idx];
    BNode* childr = child[idx + 1];
    childs -> key[degree - 1] = key[idx];
    for (int i = 0; i < childr -> n; ++i) {
        childs -> key[i + degree] = childr -> key[i];
    }
    if (!childs -> isLeaf) {
        for (int i = 0; i <= childr -> n; ++i) {
            childs -> child[i + degree] = childr -> child[i];
        }
    }
    for (int i = idx + 1; i < n; ++i) {
        key[i - 1] = key[i];
    }
    for (int i = idx + 2; i <= n; ++i) {
        child[i - 1] = child[i];
    }
    childs -> n += childr -> n + 1;
    n--;
    delete childr;
}
