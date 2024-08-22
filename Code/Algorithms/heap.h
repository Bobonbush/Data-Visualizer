#include "Utils/Texture.h"
#include "node.h"
#include "hashtable.h"

class MinHeap {
    std::vector<int>harr;
    int capacity;
    int heap_size;
public:

    MinHeap(int capacity);
    void MinHeapify(int i);
    int parent(int i) { return (i - 1) / 2; }

    int left(int i) { return (2 * i + 1); }

    int right(int i) { return (2 * i + 2); }

    int extractMin();

    void decreaseKey(int i, int new_val);

    int getMin() { return harr[0]; }

    void deleteKey(int i);

    void insertKey(int k);
};