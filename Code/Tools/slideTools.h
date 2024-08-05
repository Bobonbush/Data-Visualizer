#ifndef SLIDETOOLS_H
#define SLIDETOOLS_H

#include "Utils/Texture.h"
#include "button.h"
#include "ToolBar.h"

class SlideTools {
    private: 
        unsigned int  VAO;
        unsigned int VBO;
        unsigned int EBO;
        Shader * shader;
        Camera * camera;
        std::vector<std::vector<Button*>>slot;
        std::vector <Button*> buttons;
        unsigned int texture;
        
        
    public :
        int status = 0;
        /*
        status = 0 : Initialize        // By default -> random or input size or read from file
        status = 1 : add        // a bar
        status = 2 : remove     // a bar
        status = 3 : edit       // two bars
        status = 4 : search     // a bar
        status = 5 : getTop     // auto select
        status = 6 : algorithm   // 2 algorithms
        status = 7 : size       // auto select

        */
        SlideTools(Camera * _camera);
        ~SlideTools();

        void Draw();
        void Update(int Algo,float deltaTime, float MouseX , float MouseY);
         
};

#endif