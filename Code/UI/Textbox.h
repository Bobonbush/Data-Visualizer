#ifndef TEXTBOX_H
#define TEXTBOX_H
#include "Algorithms/shader.h"
#include "Algorithms/graph.h"


class TextBox {
    private :
    Shader* shader;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    
    unsigned int box_texture;
    unsigned int box_texture_width;
    unsigned int box_texture_height;

    glm::vec3 position;
    glm::vec3 size;

    static int cursorPos;

    static std::string textBoxContent;
    Shader * textShader;
    TextHandler * textHandler;

    Camera * camera;

    float TimeWait = 0.5f;
    float TimeWaitMaximum = 0.5f;


    public :
    TextBox(Camera * _camera);
    ~TextBox();
    void Draw();
    void Update(float deltaTime, float x, float y);

    static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    
};

#endif