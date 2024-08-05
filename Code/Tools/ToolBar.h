#ifndef TOOLBAR_H
#define TOOLBAR_H
#include "Utils/Texture.h"
#include <vector>
#include <iostream>
#include "Button.h"
#include "Interactive/Bar.h"
#include "Manager.h"

class ToolBar {
    private: 
        unsigned int  VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int outline_Texture;

        int status = 0;
        
        Shader * shader;
        Camera * camera;
        std::vector<std::vector<Bar*>>Barslot;
        std::vector  <Bar*> bars;
        std::vector <Button*> button;
        std::vector <std::vector <Button*>> buttonSlot;
        Manager * manager;
    public : 
        int Algo;
        ToolBar(Camera * _camera);
        ~ToolBar();

        void Draw();
        void Update(int _status, float deltaTime, float MouseX , float MouseY);

};

#endif