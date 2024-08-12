#include "Manager.h"


Manager::Manager(Camera * _camera) {
    avl = nullptr;
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
    tree234 = new Tree234(camera);
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
        noData = tree234 -> isEmpty();
        tree234 -> Update(deltaTime);
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

    
    if(mask) Animator(deltaTime);

}

void Manager::Draw() {
    if(currentAlgo == 0) {
        avl -> Draw();
        //randomButton -> Draw();
    } else if(currentAlgo == 1) {
        tree234 -> Draw();
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

void Manager::Animator(float deltaTime) {
    AnimationTime -= deltaTime;
    if(AnimationTime > 0.f) return ;
    AnimationTime = AnimationTimeLimit * AnimationConst;
    if(currentAlgo == 0) {
        if(mask & 1) {
            
            avl -> insert(value1);
            avl -> RecalculatePosition();
            if(avl -> root[avl -> currentVersion] -> Done) {
                mask = 0;
                avl -> Reset();
            }
        }

        if(mask & (1 << 2)) {
            avl -> search(value1);
            if(avl -> root[avl -> currentVersion] -> Done) {
                mask = 0;
                avl -> Reset();
            }
        }

        if(mask & (1 << 1)) {
            avl -> deleteNode(value1);
            
            avl -> RecalculatePosition();
            if(avl -> root[avl -> currentVersion] -> Done) {
                mask = 0;
                avl -> Reset();
            }
        }
    }

    if(currentAlgo == 2) {
        if(mask & 1) {
            trie -> insert(value2);
            trie -> RecalculatePosition();
            if(trie -> root[trie -> currentVersion] -> Done) {
                mask = 0;
                trie -> Reset();
            }
        }

        if(mask & (1 << 2)) {
            trie -> search(value2);
            if(trie -> root[trie -> currentVersion] -> Done) {
                mask = 0;
                trie -> Reset();
            }
        }
        if(mask & (1 << 1)) {
            trie -> Delete(value2);
            trie -> RecalculatePosition();
            if(trie -> root[trie -> currentVersion] -> Done) {
                mask = 0;
                trie -> Reset();
            }
        }
    }
}

void Manager::Modify(std::string value , std::string newValue) {

}

void Manager::Delete(std::string input) {
    if(mask) return ;
    index = 0;
    if(currentAlgo == 2) {
        if(trie -> root.empty() || trie -> root[trie -> currentVersion] == nullptr) {
            mask = 0;   
        }
        else {
            trie -> Deleted = false;
            trie -> root.push_back(nullptr);
            trie -> root[++trie -> currentVersion] = trie ->CopyNode(trie -> root[trie -> currentVersion - 1]);
            mask = 2;
            value2 = input;
        }
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
            value = 0; 
            continue;
        } 
        value = value * 10 + input[i] - '0';
    }

    if(currentAlgo == 0) {
        avl->root.push_back(nullptr);
        avl -> currentVersion++;
        avl -> root[avl -> currentVersion] = avl -> CopyNode(avl -> root[avl -> currentVersion - 1]);
        avl -> deleted = false;
        mask = (1 << 1);
        value1 = value;
    }

    if(currentAlgo == 1) {
        tree234 -> remove(value);
        tree234 -> RecalculatePosition();
    }


    

}


void Manager::Insert(std::string input) {
    if(mask) return ;
    index = 0;
    if(currentAlgo == 2) {
        if(trie -> root.empty() || trie -> root[trie -> currentVersion] == nullptr) {
            trie -> root.push_back(new TrieNode(glm::vec3(0.0f , 0.0f, 0.0f), glm::vec3(0.6f, 0.6f, 0.0f), camera, ' '));
            trie -> root.back() -> targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        }
        else {
            trie -> root.push_back(nullptr);
            trie -> root[++trie -> currentVersion] = trie ->CopyNode(trie -> root[trie -> currentVersion - 1]);
        }
        //trie -> RecalculatePosition();
        mask = 1;
        value2 = input;
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
            value = 0; 
            continue;
        } 
        value = value * 10 + input[i] - '0';
    }

    if(currentAlgo == 0) {
        avl->root.push_back(nullptr);
        avl -> currentVersion++;
        avl -> root[avl -> currentVersion] = avl -> CopyNode(avl -> root[avl -> currentVersion - 1]);
        value1 = value;
        mask = 1;
    }
    if(currentAlgo == 1) {
        tree234 -> root.push_back(nullptr);
        tree234 -> currentVersion++;
        tree234 -> root[tree234 -> currentVersion] = tree234 -> CopyNode(tree234 -> root[tree234 -> currentVersion - 1], nullptr);
        tree234 -> insert(value);
        tree234 -> RecalculatePosition();
    }

    
}

void Manager::Search(std::string value) {
    if(mask) return ;
    if(value.size() == 0) return;
    index = 0;
    if(currentAlgo == 2) {
        mask = (1 << 2);
        value2 = value;
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

    value1 = val;
    mask = (1 << 2);

}

int Rand(int a, int b) {
    return a + rand() % (b - a + 1);
}

void Manager::Initialize() {
    if(currentAlgo == 0) {
        avl = new AVL(camera);
        int n = Rand(10, 50);
        avl -> currentVersion++;
        avl -> root.push_back(nullptr);
        for(int i = 0; i < n; i++) {
            int value = Rand(1, 100);
            
            avl -> Initialize(value);
            avl -> Reset();
        }
        
        avl -> RecalculatePosition();

    } else if(currentAlgo == 1) {
        tree234 = new Tree234(camera);
        int n = Rand(10, 40);
        tree234 -> currentVersion++;
        tree234 -> root.push_back(nullptr);
        for(int i = 0; i < n; i++) {
            int value = Rand(1, 100);
            tree234 -> Initialize(value);
        }
        tree234 -> RecalculatePosition();
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
        tree234 -> currentVersion--;
        if(tree234 -> currentVersion < 0) {
            tree234 -> currentVersion = 0;
            tree234 -> root.clear();
        }else tree234 -> root.pop_back();
    } else if(currentAlgo == 2) {
       trie -> currentVersion--;
       if(trie -> currentVersion < 0) {
           trie -> currentVersion = 0;
           trie -> root.clear();
       }else trie -> root.pop_back();
    }
}