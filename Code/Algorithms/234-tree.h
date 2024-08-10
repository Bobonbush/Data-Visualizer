#include "node.h"
#include <algorithm>

class BNode {
    std::vector<int > keys;
    std::vector<BNode*> children;

    BNode* parent;
    BNode() : parent(nullptr) {}

    bool isLeaf() const {
        return children.empty();
    }

    bool isFull() const {
        return keys.size() == 3;
    }

    void insertKey(int key) {
        keys.push_back(key);
        std::sort(keys.begin(), keys.end());
    }
};

class Tree234 {
    private : 
        BNode * root;

        void split();

        void insertRecursively(BNode* node, int key);
    public:
        Tree234();
        ~Tree234();

        void insert(int key);
        void print();
        void printTree();
};