#include "Tools/button.h"




class StaticButton{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    Shader *shader;
    Camera *camera;
    unsigned int texture;
    glm::vec3 position;
    glm::vec3 size;
    std::string name;

    glm::vec3 pivot;

    Outline * outline;

    public:
    StaticButton(glm::vec3 _position, glm::vec3 _size, char* path, Camera * _camera, std::string _name);
    ~StaticButton();
    void Draw();
    void Update(float MouseX, float MouseY);
    bool isClicked();
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
    glm::vec3 position;
    glm::vec3 size;

    Button * iter;
    public:
    Scroll(Camera * _camera);
    ~Scroll();
    void Draw();
    void Update(float MouseX, float MouseY);

};