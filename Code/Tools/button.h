#ifndef BUTTON_H
#define BUTTON_H
#include "Utils/Texture.h"
#include "string"
#include <iostream>


class Outline {
    private :
        unsigned int  VAO;
        unsigned int VBO;
        unsigned int EBO;
        Shader * shader;
        Camera * camera;
        
        unsigned int texture;


        

        


    public :
        glm::vec3 position;
        glm::vec3 size;
        bool isHovered = false;
        Outline(glm::vec3 _position, glm::vec3 _size, Camera * _camera);
        ~Outline();
        void Draw();
        void Update(float MouseX , float MouseY);
        bool isInside(float MouseX , float MouseY);
        
        
};



class Button {
    private : 
        unsigned int VBO;
        unsigned int EBO;
        TextHandler * text;
        bool clicked = false;
        std::string name;
        Shader* textShader;
        
    protected :
        unsigned int  VAO;
        float MouseWait = 0.f;
        float MouseWaitLimit = 0.09f;

        Shader * shader;
        
        Camera * camera;
        Outline * outline;
        glm::vec3 position;
        glm::vec3 size;
        glm::vec3 pivot;
        unsigned int texture;

        
        
        

    public:
        bool isChosen = false;
        

        

        
        Button(glm::vec3 _position, glm::vec3 _size, char *path,  Camera * _camera, std::string _text);
        ~Button();

        virtual void Draw();
        virtual bool Update(float deltaTime, float MouseX , float MouseY);
        glm::vec3 GetBoxSize();
        bool isClicked(float MouseX , float MouseY);
    
};





#endif