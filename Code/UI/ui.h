#ifndef UI_H
#define UI_H
#include "Tool/button.h"
#include "lightbulb.h"
#include <vector>
#include <iostream>
#include "Algorithms/AlgorithmManager.h"

class UI {
    private:
        std::vector<Button*> buttons;

        AlgorithmManager * algo;
        TextHandler * waiting;
        Shader * textShader;
        Camera * camera;
        float waitingTime = 0.0f;
        float maximumWaitingTime = 0.5f;
        std::string waitingText = "Waiting for data";
        float maximal_up = 0.01f;

        
    public:
        UI(Camera* _camera);
        ~UI();
        void Update(float deltaTime, float x, float y);
        void Draw();
        
};

#endif