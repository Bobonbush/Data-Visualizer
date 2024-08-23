#include "heap.h"


MinHeap::MinHeap(int capacity, Camera * _camera) {
    heap_size = 0;
    this->capacity = capacity;
    camera = _camera;
    blocks.resize(capacity);
    indexed_blocks.resize(capacity);
    textHandler = new TextHandler();
    textShader = new Shader("textShader.vs", "textShader.fs");
    textHandler -> LoadFont("Font/Roboto-Black.ttf", 24);

    glm::vec3 position = glm::vec3(-0.4, -0.73, 0.0f);
    for(int i = 0 ; i < capacity ; i++) {
        blocks[i] = new Block(position, glm::vec3(0.75f, 0.75f, 0.1f), INT_MAX, camera, "box.png");
        indexed_blocks[i] = new Block(glm::vec3(position.x , position.y - blocks[i] -> getSize().y /1.5f , position.z), blocks[i] -> getSize(), i+1, camera, "", true);
        position.x +=  blocks[i] -> getSize().x + 0.005f;
    }

    nodes.resize(capacity);

    for(int i = 0; i < capacity; i++) {
        nodes[i] = nullptr;
    }
}

MinHeap::~MinHeap() {
    for(int i = 0 ; i < capacity ; i++) {
        delete blocks[i];
        delete indexed_blocks[i];
        if(nodes[i] != nullptr) {
            delete nodes[i];
        }
    }
}

void MinHeap::GetSize() {
    textShader -> use();
    textShader -> setMat4("projection", glm::mat4(1.0f));
    textHandler -> RenderText(textShader, "Heap Size: " + std::to_string(heap_size), 0.0f, -0.95f, 2.f, glm::vec3(1.0f, 0.0f, 0.0f),camera, 0.0005f);

}

void MinHeap::MinHeapify(int i) {
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && blocks[l] -> key < blocks[smallest] -> key)
        smallest = l;
    if (r < heap_size && blocks[r] -> key < blocks[smallest] -> key)
        smallest = r;
    if (smallest != i) {
        std::swap(blocks[i] -> key, blocks[smallest] -> key);
        MinHeapify(smallest);
    }
}

bool MinHeap::isEmpty() {
    return heap_size == 0;
}


void MinHeap::extractMin() {
    if (heap_size <= 0)
        return ;
    if (heap_size == 1) {
        heap_size--;
        blocks[0] -> key = INT_MAX;
        nodes[0] = nullptr; 
        return ;
    }
    int root = blocks[0] -> key;
    blocks[0] -> key = blocks[heap_size - 1] -> key;
    blocks[heap_size -1] -> key = INT_MAX;
    nodes[heap_size - 1] = nullptr;
    heap_size--;
    MinHeapify(0);
    Reset();
    return ;
}

bool MinHeap::GetTop() {
    if(heap_size <= 0) {
        blocks[0] -> status = -1;
        return true;
    }
    if(heap_size == 1) {
        heap_size--;
        blocks[0] -> key = INT_MAX;
        nodes[0] = nullptr;
        return true;
    }
    int root = blocks[0] -> key;
    blocks[0] -> key = blocks[heap_size - 1] -> key;
    blocks[heap_size - 1] -> key = INT_MAX;
    nodes[heap_size - 1] = nullptr;
    heap_size--;
    MinHeapify(0);
    Reset();
    return  true ;
} 

void MinHeap::decreaseKey(int i, int new_val) {
    blocks[i] -> key = new_val;
    while (i != 0 && blocks[parent(i)] -> key > blocks[i] -> key) {
        std::swap(blocks[i] -> key, blocks[parent(i)] -> key);
        i = parent(i);
    }
    Reset();
}

void MinHeap::deleteKey(int key) {
    for(int i = 0 ; i < capacity ; i++) {
        if(blocks[i] -> key == key) {
            decreaseKey(i, INT_MIN);
            extractMin();
            Reset();
            return;
        }
    }
}

void MinHeap::insertKey(int k) {
    if (heap_size == capacity) {
        std::cout << "\nOverflow: Could not insertKey\n";
        return;
    }
    heap_size++;
    int i = heap_size - 1;
    blocks[i] -> key = k;
    nodes[i] = new Node(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), camera, k);
    nodes[i] -> targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    while (i != 0 && blocks[parent(i)] -> key > blocks[i] -> key) {
        std::swap(blocks[i] -> key, blocks[parent(i)] -> key);
        i = parent(i);
    }

    Reset();
}

void MinHeap::Update(float deltaTime, float MouseX, float MouseY) {
    if(blocks[0] != nullptr && nodes[0] != nullptr) {
        if(abs(nodes[0] -> targetPosition.x - nodes[0] -> position.x) < 0.01f && abs(nodes[0] -> targetPosition.y - nodes[0] -> position.y) < 0.01f) {
           nodes[0] -> status = 2;
        }
        
        blocks[0] -> status = 1;
    }
    
    for(int i = 0; i < capacity; i++) {
        blocks[i]->Update(deltaTime);
        indexed_blocks[i]->Update(deltaTime);
    }

    if(nodes[0] != nullptr) {
        Node::isOver = false;
        nodes[0] -> Update(deltaTime, MouseX, MouseY);
        //nodes[0]->Update(deltaTime, MouseX, MouseY);
    }
}

void MinHeap::Draw() {
    GetSize();
    for(int i = 0; i < capacity; i++) {
        blocks[i]->Draw();
        indexed_blocks[i]->Draw();
    }

    if(nodes[0] != nullptr) {
        nodes[0]->Draw();
    }
}

void MinHeap::Reset() {
    for(int i =  0 ; i < capacity ; i++) {
        if(nodes[i] != nullptr) {
            nodes[i] -> left = nullptr;
            nodes[i] -> right = nullptr;
            if(i * 2 + 1 < capacity) {
                nodes[i]->left = nodes[i * 2 + 1];
            }

            if(i * 2 + 2 < capacity) {
                nodes[i] -> right = nodes[i * 2 + 2];
            }

            nodes[i] -> value = blocks[i] -> key;
            nodes[i] -> text = std::to_string(blocks[i] -> key);
        }
    }
}

void MinHeap::Reset(Node * node) {
}

HeapInfo MinHeap::calculateSubTreeWidth(Node * node, float offsetX) {
    HeapInfo info = {0, 0};
    if(!node) return info ;

    float totalWidth = 0;
    float maxOffset = 0.f;

    HeapInfo leftInfo = calculateSubTreeWidth(node -> left, offsetX);
    totalWidth += leftInfo.width + offsetX;
    maxOffset = std::max(maxOffset, leftInfo.xOffset);
    HeapInfo rightInfo = calculateSubTreeWidth(node -> right, offsetX);

    totalWidth += rightInfo.width + offsetX;
    maxOffset = std::max(maxOffset, rightInfo.xOffset);

    totalWidth = std::max(totalWidth, offsetX);

    float xOffset = totalWidth / 2;

    return {totalWidth, xOffset};

}

void MinHeap::RecalculatePosition(Node * node, float x, float y, float offset_x, float offset_y) {
    if(!node) return ;

    HeapInfo info = calculateSubTreeWidth(node, offset_x);
    node -> targetPosition = glm::vec3(x, y, 0.0f);

    float childX = x - info.width / 2.0f;
    if(node -> left != nullptr) {
        HeapInfo childInfo = calculateSubTreeWidth(node -> left, offset_x);
        RecalculatePosition(node -> left, childX + childInfo.xOffset, y - offset_y, offset_x, offset_y);
        childX += childInfo.width + offset_x;
    }

    if(node -> right != nullptr) {
        HeapInfo childInfo = calculateSubTreeWidth(node -> right, offset_x);
        RecalculatePosition(node -> right, childX + childInfo.xOffset, y - offset_y, offset_x, offset_y);
        
    }

}

void MinHeap::RecalculatePosition() {
    float offsetx  = 0.025f;
    float offsety = 0.2f;
    RecalculatePosition(nodes[0], 0.0f, 0.75f, offsetx, offsety);
}

