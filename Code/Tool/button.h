#ifndef BUTTON_H
#define BUTTON_H

#include "Algorithms/Texture.h"
#include "Algorithms/shader.h"
#include "Camera.h"




class Button {
    private:
        Shader * shader;
        
        //float * vertices;
        //float * indices;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        unsigned int outline_VAO;
        unsigned int outline_VBO;
        unsigned int outline_EBO;
    protected:
        unsigned int texture;
        float texture_width;
        float texture_height;
        glm::vec3 position;
        glm::vec3 size;
        Shader * outline;

    public:
        
        
        Button(glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera);
        ~Button();
        
        virtual void Update(float deltaTime);
        virtual void Draw();

        
};

#endif