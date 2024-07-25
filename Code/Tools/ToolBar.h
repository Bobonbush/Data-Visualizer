#ifndef TOOLBAR_H
#define TOOLBAR_H
#include "Utils/Texture.h"
#include <vector>
#include <iostream>
#include "Button.h"


class ToolBar {
    private: 
        unsigned int  VAO;
        unsigned int VBO;
        unsigned int EBO;
        unsigned int outline_Texture;
        Shader * shader;
        Camera * camera;
        std::vector <Button*> buttons;
    public : 
        ToolBar(Camera * _camera);
        ~ToolBar();

        void Draw();
        void Update();

};

#endif