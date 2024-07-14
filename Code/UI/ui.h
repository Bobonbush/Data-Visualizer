#ifndef UI_H
#define UI_H
#include "Tool/button.h"
#include "lightbulb.h"
#include <vector>
#include <iostream>

class UI {
    private:
        std::vector<Button*> buttons;
    public:
        UI(Camera* camera);
        ~UI();
        void Update(float deltaTime);
        void Draw();
        
};

#endif