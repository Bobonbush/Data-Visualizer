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
        SlideTools(Camera * _camera);
        ~SlideTools();

        void Draw();
        void Update(int Algo,float deltaTime, float MouseX , float MouseY);
         
};

#endif