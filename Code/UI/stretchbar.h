#include "Algorithms/Shader.h"
#include "Camera.h"
#include "Algorithms/Texture.h"


class StretchBar {
    private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    unsigned int texture;
    float texture_width;
    float texture_height;
    glm::vec3 position;
    glm::vec3 size;
    Shader * shader;
    float maximal_up = 0.015f;
    float minimal_up = -0.01f;
    public:
    float stretch = 0.0f;
    float StretchSensitive = 0.75f; 
    
    
    
    StretchBar( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera);
    ~StretchBar();
        
    void Update(float deltaTime, float x, float y);
    void Draw();
};