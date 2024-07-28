#include "Tools/button.h"


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

    unsigned int outline_Texture;
    unsigned int texture;
    glm::vec3 position;
    glm::vec3 size;
    public:
    Scroll();
    ~Scroll();
    void Draw();
    void Update(float MouseX, float MouseY);
    bool isInside(float MouseX, float MouseY);


};

class StaticButton : Button {
    public:
    StaticButton(glm::vec3 _position, glm::vec3 _size, Camera * _camera, std::string _name);
    ~StaticButton();
    void Draw();
    void Update(float MouseX, float MouseY);
    bool isInside(float MouseX, float MouseY);
};