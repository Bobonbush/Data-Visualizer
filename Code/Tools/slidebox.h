#include "Utils/Texture.h"
#include "button.h"


class OutlineSlideBox {
    unsigned int VAO, VBO;
    unsigned int EBO;
    Shader * shader;
    Camera * camera;
    unsigned int texture;

    

    glm::vec3 size;
    public :
    bool isHovered = false;
    glm::vec3 position;
    
    OutlineSlideBox( glm::vec3 _position, glm::vec3 _size, Camera * _camera);
    ~OutlineSlideBox();

    void Draw(bool isChosen);

    void Update(float MouseX , float MouseY);

    bool isInside(float MouseX , float MouseY);

};

class SlideBox {
    std::string data;

    OutlineSlideBox * outline;
    
    TextHandler * text;
    Shader * shader;
    Camera * camera;

    float MouseWait = 0.f;
    float MouseWaitLimit = 0.05f;

    


    public : 
    glm::vec3 position;
    glm::vec3 size;
    bool isHovered = false;
    bool isChosen = false;
    SlideBox(glm::vec3 _position, glm::vec3 _size, Camera * _camera, std::string _data);
    ~SlideBox();
    bool Update(float deltaTime,float MouseX , float MouseY);
    void Draw();
    bool isClicked();
};

