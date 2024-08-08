#include "Utils/Texture.h"
#include "Algorithms/234-tree.h"
#include "Algorithms/AVL.h"
#include "Algorithms/Trie.h"
#include "slidebox.h"
#include "Interactive/Scroll.h"
class Manager {
    private :
    int currentAlgo = 0;
    /*
    0 is AVL
    1 is BtREE
    2 is Trie
    3 is Graph
    4 is Heap
    5 is HashTable
    */
    float AnimationSpeed = 1.f;             // Maximum 1.f
    float AnimationCountDown = 0.f;
    int currentAction = 0;
    bool Sliding = 0;

    float AnimationConst = 0.01f;
    std::vector<NodeInfo> animationNodes;
    std::vector<TrieInfo> animationTrieNodes;
    TrieNode * current;
    int index = 0;
    float AnimationTime = 0.5f;
    float AnimationTimeLimit = 0.4f;
    float AnimateStatus = 0;
    /*
    0 : traverse
    1 : change
    */


    /*
    0 is Insert
    1 is Delete
    2 is Modify
    3 is Search
    4 is Algorithm
    5 is Initialize
    */

    AVL * avl;
    BTree * bTree;
    Trie * trie;
    Camera * camera;

    TextHandler * textHandler;
    Button * scroll;
    Shader * textShader;

    std::vector<SlideBox *> algorithmsBox;
    bool noData = false;
    std::string waitingText = "Waiting for Data";
    float waitingTime = 0.f;
    float waitingTimeLimit = 1.f;

    float MouseWait = 0.f;
    float MouseWaitLimit = 0.08f;

    Scroll * animationBar;

    std::string currentData = "AVL Tree";
    
    std::string dataStructure[6] = {
        "AVL Tree",
        "B Tree",
        "Trie",
        "Graph",
        "Heap",
        "Hash Table"
    };
    
    public:
    
    /*
    
    */
   Manager(Camera * _camera);
   ~Manager();

   void Initialize(int degree);

   void Update(int & Algo, float deltaTime, float MouseX , float MouseY);

   void Draw();
   void Modify(std::string value , std::string newValue);
   void Delete(std::string value);
   void Insert(std::string value);
   void Search(std::string value);
   void Initialize();

   bool NodeAnimationManagement(float deltaTime);
   bool TrieNodeAnimationManagement(float deltaTime);

   void Reverse();

    
};