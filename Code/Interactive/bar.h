#include "Utils/Texture.h"

class Bar {
    unsigned int VAO, VBO, EBO;
    Shader * shader;
    unsigned int texture;
    Camera * camera;
    glm::vec3 position;
    glm::vec3 size;
    std::string input;
    

    public:
    Bar(Camera * _camera, glm::vec3 _position, glm::vec3 _size);
    ~Bar();

    void Draw();
    void Update();

};