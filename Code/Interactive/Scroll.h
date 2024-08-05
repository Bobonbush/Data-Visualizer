#include "Tools/button.h"




class StaticButton : public Button {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    Shader *shader;
    Camera *camera;
    unsigned int texture;
    std::string name;

    glm::vec3 pivot;
    bool isHovered = false;
    public:
    glm::vec3 position;
    glm::vec3 size;
    StaticButton(glm::vec3 _position, glm::vec3 _size, char* path, Camera * _camera, std::string _name);
    ~StaticButton();
    void Draw();
    void Update(float MouseX, float MouseY);
    bool isClicked();
    bool isMouseHovered(float MouseX, float MouseY);
};

class Scroll {
    float max_value = 2.f;
    float min_value = 1.f;
    float current_value = min_value;
    float step = 0.1f;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    Shader *shader;
    Camera *camera;
    unsigned int texture;
    
    StaticButton * iter;
    public:
    glm::vec3 position;
    glm::vec3 size;

    Scroll(Camera * _camera);
    ~Scroll();
    void Draw();
    float Update(float MouseX, float MouseY);

};