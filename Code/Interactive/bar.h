#include "Utils/Texture.h"

class Bar {
    unsigned int VAO, VBO, EBO;
    Shader * shader;
    unsigned int texture;
    Camera * camera;
    glm::vec3 position;
    glm::vec3 size;
    static std::string input;
    static int cursor;

    bool isChosen = false;
    

    TextHandler * text;
    Shader* textShader;

    float waitingTime = 0.f;
    float waitingTimeLimit = 0.5f;

    bool IsHovered(float MouseX, float MouseY);
    bool IsClicked(float MouseX, float MouseY);

    

    public:
    static bool CanHaveComma;
    static bool CanHaveSpace;
    static bool Enter;

    Bar(Camera * _camera, glm::vec3 _position, glm::vec3 _size);
    ~Bar();

    void Draw();
    std::string Update(float deltaTime , float MouseX , float MouseY);

    static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
    


};