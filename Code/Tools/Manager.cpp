#include "Manager.h"


Manager::Manager(Camera * _camera) {
    avl = nullptr;
    bTree = nullptr;
    trie = nullptr;
    glm::vec3 position = glm::vec3(0.5, -0.97f, 0.0f);
    float offset = 0.15f;
    position.x -= offset ;
    SlideBox * AVL_Slide = new SlideBox(position, glm::vec3(0.01f, 0.05f, 0.0f), _camera , dataStructure[0]);
    algorithmsBox.push_back(AVL_Slide);

    scroll = new Button(glm::vec3(0.95f, -0.9, 0.f), glm::vec3(0.3f, 0.3f, 0.f),  "greater.png", _camera, "");

    animationBar = new Scroll(_camera);

    //randomButton = new Button(glm::vec3(0.95f, 0.9f, 0.f), glm::vec3(0.3f, 0.3f, 0.f), "node.png", _camera, "Random");

    
    position.y += offset;
    

    SlideBox * BTree_Slide = new SlideBox(position, glm::vec3(0.01f, 0.05f, 0.0f), _camera , dataStructure[1]);
    algorithmsBox.push_back(BTree_Slide);
    
    position.y += offset;
    SlideBox * Trie_Slide = new SlideBox(position, glm::vec3(0.01f, 0.05f, 0.0f), _camera , dataStructure[2]);
    algorithmsBox.push_back(Trie_Slide);
    
    position.y += offset;
    SlideBox * Graph_Slide = new SlideBox(position, glm::vec3(0.01f, 0.05f, 0.0f), _camera , dataStructure[3]);
    algorithmsBox.push_back(Graph_Slide);
    
    position.y += offset;
    SlideBox * Heap_Slide = new SlideBox(position, glm::vec3(0.01f, 0.05f, 0.0f), _camera , dataStructure[4]);
    algorithmsBox.push_back(Heap_Slide);
    
    position.y += offset;
    SlideBox * HashTable_Slide = new SlideBox(position, glm::vec3(0.01f, 0.05f, 0.0f), _camera , dataStructure[5]);
    algorithmsBox.push_back(HashTable_Slide);

    algorithmsBox[0] -> isChosen = true;


    
    camera = _camera;

    textHandler = new TextHandler();
    textShader = new Shader("textShader.vs", "textShader.fs");
    textHandler->LoadFont("Font/Revamped.ttf", 48);
    
    Initialize(2);
}

Manager::~Manager() {
    if (avl) {
        delete avl;
    }
    if (bTree) {
        delete bTree;
    }
    if (trie) {
        delete trie;
    }
    delete textHandler;
    delete textShader;
    for(int i = 0; i < algorithmsBox.size(); i++) {
        delete algorithmsBox[i];
    }
}

void Manager::Initialize(int degree) {
    avl = new AVL(camera);
        //bTree = new BTree(camera, degree);
    trie = new Trie(camera);
}

void Manager::Update(int &Algo, float deltaTime , float MouseX , float MouseY) {
    waitingTime -= deltaTime;
    MouseWait -= deltaTime;
    if(waitingTime < 0.f) {
        waitingText.push_back('.');
        if(waitingText.size() == 20) {
            waitingText = "Waiting for Data";
        }
        waitingTime = waitingTimeLimit;
    }

    currentData = dataStructure[Algo];
    
    if(currentAlgo == 0) {
        
        noData = avl -> Empty();
        avl -> Update(deltaTime, MouseX , MouseY);
        //randomButton -> Update(deltaTime, MouseX, MouseY);
        
    } else if(currentAlgo == 1) {
        //bTree -> Update(MouseX, MouseY);
    } else if(currentAlgo == 2) {
        noData = trie -> Empty();
        trie -> Update(deltaTime, MouseX , MouseY);
    }
    AnimationConst =  animationBar -> Update(MouseX, MouseY);
    Node::speed = AnimationConst * 1.0f;


    for(int i = 0; i < algorithmsBox.size(); i++) {
        
        if(!Sliding &&  algorithmsBox[i] -> isChosen == false) continue;
        bool ok = algorithmsBox[i] -> Update(deltaTime, MouseX, MouseY);
        if(ok && MouseWait < 0.f && algorithmsBox[i] -> isChosen == false) {
            MouseWait = MouseWaitLimit;
            currentAlgo = i;
            Algo = i ;
            //Initialize(2);
            
            SlideBox * current = algorithmsBox[i];
            for(int j = 0; j < algorithmsBox.size(); j++) {
                if(algorithmsBox[j]->isChosen == true) {
                    algorithmsBox[j] -> isChosen = false;
                    std::swap(algorithmsBox[j] -> position, current -> position);
                    break;
                }
            }

            Sliding = 0;
            
            algorithmsBox[i] -> isChosen = true;
            break;
        }
    } 
    bool slide = scroll -> Update(deltaTime, MouseX, MouseY);
    if(slide ) {
        Sliding ^= 1;
    }
    
    if(!animationNodes.empty() && NodeAnimationManagement(deltaTime)) {
        return ;
    } else if(animationNodes.size() > 0) {
        
        if(currentAlgo == 0) {
            for(int i = 0 ; i<= (int)animationNodes.size()-1 ; i++) {
                NodeInfo info = animationNodes[i];
                info. node -> status = -1;
            }
            avl -> RecalculatePosition();
        }

        animationNodes.clear();
    }

    if(!animationTrieNodes.empty() && TrieNodeAnimationManagement(deltaTime)) {
        return ;
    }else if(animationTrieNodes.size() > 0 ) {
        if(currentAlgo == 2) {
            current = animationTrieNodes[0].node;
            for(int i = 0 ; i<= (int)animationTrieNodes.size()-1 ; i++) {
                TrieInfo info = animationTrieNodes[i];
                current ->  status = -1;
                current = current -> _next[info.word];
            }
            trie -> RecalculatePosition();
        }
        animationTrieNodes.clear();
    }

}

void Manager::Draw() {
    if(currentAlgo == 0) {
        avl -> Draw();
        //randomButton -> Draw();
    } else if(currentAlgo == 1) {
        //bTree -> Draw();
    } else if(currentAlgo == 2) {
        trie -> Draw();
    }

    for(int i = 0; i < algorithmsBox.size(); i++) {
        
        if(!Sliding &&  algorithmsBox[i] -> isChosen == false) continue;
        algorithmsBox[i] -> Draw();
    } 
    if(noData) {
        float offset = camera -> height / camera -> width * 0.009f;

        textShader -> use();       
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.05f * cos(glfwGetTime()), 0.05f * sin(glfwGetTime()), 0.0f));
        textShader -> setMat4("projection", model);


        textHandler -> RenderText(textShader, waitingText, -0.5f, -0.25f, 2.5f, glm::vec3(43.f/255.f, 157.f/255.f, 168.f/255.f) , camera, offset);
        textShader -> setMat4("projection", glm::mat4(1.0f));
        //textHandler -> RenderText(textShader, currentData, 1.f, -0.97f, 2.f, glm::vec3(75.f/255.f, 213.f/255.f, 226.f/255.f) , camera, offset);
    }

    animationBar -> Draw();

    
    
    scroll -> Draw();
}

bool Manager::NodeAnimationManagement(float deltaTime) {
    if(AnimationTime > 0.f) {
        AnimationTime -= deltaTime;
    }else if(index < (int)animationNodes.size()) {
        AnimationTime = AnimationTimeLimit * AnimationConst;
        NodeInfo info = animationNodes[index];
        info.node -> status = info.status;
        
        
        index++;

    }else {
        AnimationTime = AnimationTimeLimit * AnimationConst;
        index++;
    }
    return index <= (int) animationNodes.size();
}

bool Manager::TrieNodeAnimationManagement(float deltaTime) {
    if(AnimationTime > 0.f) {
        AnimationTime -= deltaTime;
    }else if(index < (int)animationTrieNodes.size()) {
        if(index ==0 ) {
            current = animationTrieNodes[index].node;
        }
        AnimationTime = AnimationTimeLimit * AnimationConst;
        TrieInfo info = animationTrieNodes[index];
        //current -> status = info.status;
        if(info.found) {
            current -> status = info.status;
            current = current -> _next[info.word];
        }
        if(info.newed) {

            current -> status = info.status;
            bool isLeaf = (index == (int)animationTrieNodes.size()-1);
            bool created = trie->subInsert(info.word, current, isLeaf);
            if(created) {
                trie -> RecalculatePosition();
            }
            
        }
        index++;
    }else {
        AnimationTime = AnimationTimeLimit * AnimationConst;
        index++;
    }
    return index <= (int) animationTrieNodes.size();
}

void Manager::Modify(std::string value , std::string newValue) {

}

void Manager::Delete(std::string input) {
    if((int) animationNodes.size() > 0 && currentAlgo != 2) return;
    if((int) animationTrieNodes.size() > 0 && currentAlgo == 2) return;
    index = 0;
    if(currentAlgo == 2) {
        trie -> Delete(input, animationTrieNodes);
        trie -> RecalculatePosition();
        return ;
    }
    int value = 0;
    bool valid = true;
    for(int i = 0 ; i < input.size(); i++) {
        if(input[i] <'0' || input[i] > '9') {
            if(input[i] !=',') {
                valid = false;
                break;    
            }
            
        }
    }

    valid &= (input[0] != ',');
    if(!valid) return;

    for(int i = 0; i < input.size(); i++) {
        if(input[i] == ',') {
            avl -> deleteNode(value, animationNodes) ;
            value = 0; 
            continue;
        } 
        value = value * 10 + input[i] - '0';
    }
    avl -> deleteNode(value, animationNodes);
    avl -> RecalculatePosition();

}

void Manager::Insert(std::string input) {
    if((int) animationNodes.size() > 0 && currentAlgo != 2) return;
    if((int) animationTrieNodes.size() > 0 && currentAlgo == 2) return;
    index = 0;
    if(currentAlgo == 2) {
        trie -> insert(input, animationTrieNodes);
        //trie -> RecalculatePosition();
        return ;
    }
    int value = 0;
    bool valid = true;
    
    for(int i = 0 ; i < input.size(); i++) {
        if(input[i] <'0' || input[i] > '9') {
            if(input[i] !=',') {
                valid = false;
                break;    
            }
            
        }
    }
    
    valid &= (input[0] != ',');
    if(!valid) return;
    for(int i = 0; i < input.size(); i++) {
        if(input[i] == ',') {
            avl -> insert(value, animationNodes) ;
            value = 0; 
            continue;
        } 
        value = value * 10 + input[i] - '0';
    }
    if(currentAlgo == 0)
        avl -> insert(value, animationNodes);
    
    //avl -> RecalculatePosition();


}

void Manager::Search(std::string value) {
    if(value.size() == 0) return;
    if((int) animationNodes.size() > 0 && currentAlgo != 2) return;
    if((int) animationTrieNodes.size() > 0 && currentAlgo == 2) return;
    index = 0;
    if(currentAlgo == 2) {
        trie -> search(value, animationTrieNodes);
        return ;
    }
    bool valid = true;
    
    
    for(int i = 0; i < value.size(); i++) {
        if(value[i] < '0' || value[i] > '9') {
            valid = false;
            break;
        }
    }

    if(!valid) return;
    int val = 0;
    for(int i = 0; i < value.size(); i++) {
        val = val * 10 + value[i] - '0';
    }
    avl -> search(val, animationNodes);

}

int Rand(int a, int b) {
    return a + rand() % (b - a + 1);
}

void Manager::Initialize() {
    if(currentAlgo == 0) {
        avl = new AVL(camera);
        int n = Rand(10, 20);
        avl -> currentVersion++;
        avl -> root.push_back(nullptr);
        for(int i = 0; i < n; i++) {
            int value = Rand(1, 100);
            avl -> Initialize(value, animationNodes);
            for(int i = 0 ; i <= (int)animationNodes.size()-1 ; i++) {
            NodeInfo info = animationNodes[i];
            info. node -> status = -1;
            }
        animationNodes.clear();
        }
        
        avl -> RecalculatePosition();

    } else if(currentAlgo == 1) {
        //bTree = new BTree(camera, 2);
    } else if(currentAlgo == 2) {
        int numWord = Rand(2 , 7);
        trie = new Trie(camera);
        while(numWord--) {
            std::string word = "";
            int len = Rand(1, 6);
            for(int i = 0; i < len; i++) {
                word.push_back('A' + Rand(0, 3));
            }
            trie -> Initialize(word);
        }
        trie -> RecalculatePosition();
    }
}

void Manager::Reverse() {
    if(currentAlgo == 0) {
        avl -> currentVersion--;
        if(avl -> currentVersion < 0) {
            avl -> currentVersion = 0;
        }else avl -> root.pop_back();
    } else if(currentAlgo == 1) {
        //bTree -> PreOrder();
    } else if(currentAlgo == 2) {
       trie -> currentVersion--;
       if(trie -> currentVersion < 0) {
           trie -> currentVersion = 0;
           trie -> root.clear();
       }else trie -> root.pop_back();
    }
}