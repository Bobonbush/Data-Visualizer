#ifndef BUTTON_H
#define BUTTON_H

#include "Algorithms/Texture.h"
#include "Algorithms/shader.h"
#include "Camera.h"
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
        

        
        

        

       

    protected:
        float maximal_up = 0.015f;
        float minimal_up = -0.01f;
        glm::vec3 pivot;
        bool MouseOver(float x, float y);
        bool isMouseClicked(float x, float y);
        bool isMouseRelease(float x, float y);
        bool Choose = false;
        unsigned int texture;
        float texture_width;
        float texture_height;
        glm::vec3 position;
        glm::vec3 size;
        Shader * shader;
        float sizeConst = 3.f;
        std::vector<glm::vec3> HitBox;
        float LastMouseX = -1.f;
        float LastMouseY = -1.f;

        float half_x;
        float half_y;
        
        



    public:
        glm::vec3 origin;
    
        
        
        Button(glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera);
        ~Button();
        
        virtual int Update(float deltaTime, float x, float y);
        virtual void Draw();

        
};

#endif