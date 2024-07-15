#ifndef BUTTON_H
#define BUTTON_H

#include "Algorithms/Texture.h"
#include "Algorithms/shader.h"
#include "Camera.h"
#include "UI/stretchbar.h"
#include <iostream>
#include <algorithm>




class Button {
    private:
        
        
        //float * vertices;
        //float * indices;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        unsigned int outline_VAO;
        unsigned int outline_VBO;
        unsigned int outline_EBO;
    
        // animation
        float maximal_up = 0.015f;
        float minimal_up = -0.01f;

        glm::vec3 pivot;
        StretchBar * stretchbar;

        bool MouseOver(float x, float y);

    protected:
        unsigned int texture;
        float texture_width;
        float texture_height;
        glm::vec3 position;
        glm::vec3 size;
        Shader * shader;
        float sizeConst = 3.f;
        



    public:
    
        
        
        Button(glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera);
        ~Button();
        
        virtual void Update(float deltaTime, float x, float y);
        virtual void Draw();

        
};

#endif