#include "Utils/Texture.h"
#include "node.h"
#include "hashtable.h"

struct HeapInfo { 
    float  width = 0 ;
    float xOffset = 0;
};

class MinHeap {
    std::vector<Block*>blocks;
    std::vector<Block*> indexed_blocks;
    std::vector<Node*> nodes;
    int capacity;
    int heap_size;
    Camera* camera;
public:

    MinHeap(int capacity, Camera *_camera);
    ~MinHeap();
    void MinHeapify(int i);
    int parent(int i) { return (i - 1) / 2; }

    int left(int i) { return (2 * i + 1); }

    int right(int i) { return (2 * i + 2); }

    void extractMin();
    bool GetTop();

    void decreaseKey(int i, int new_val);

    int getMin() { return blocks[0] -> key; }
    int GetSize() {return heap_size;}

    void deleteKey(int i);

    void insertKey(int k);
    bool isEmpty();

    void Update(float deltaTime, float MouseX, float MouseY);
    void Draw();
    void Reset();
    void Reset(Node * node);

    HeapInfo calculateSubTreeWidth(Node* node, float offsetX);

    void RecalculatePosition(Node* node , float x , float y , float offset_x, float offset_y);
    void RecalculatePosition();
};