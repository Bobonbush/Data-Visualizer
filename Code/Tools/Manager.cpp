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
    if(currentAlgo == 0) {
        avl = new AVL();
    } else if(currentAlgo == 1) {
        //bTree = new BTree(camera, degree);
    } else if(currentAlgo == 2) {
        //trie = new Trie(camera);
    }
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
        avl -> Update(MouseX , MouseY);
        
    } else if(currentAlgo == 1) {
        //bTree -> Update(MouseX, MouseY);
    } else if(currentAlgo == 2) {
        //trie -> Update();
    }

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
}

void Manager::Draw() {
    if(currentAlgo == 0) {
        avl -> Draw();
    } else if(currentAlgo == 1) {
        //bTree -> Draw();
    } else if(currentAlgo == 2) {
        //trie -> Draw();
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

    

    scroll -> Draw();
}

void Manager::Modify(float value , float newValue) {
}

void Manager::Delete(float value) {
    
}

void Manager::Insert(float value) {
    
}

void Manager::Search(float value) {
    
}