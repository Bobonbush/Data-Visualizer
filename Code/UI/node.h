#include "Algorithms/shader.h"
#include "Tool/button.h"


class Node : public Button {
private:
    TextHandler * text;
    std::string name;
    Camera * camera;
    Shader* textShader;
public:
    
    Node(glm::vec3 _position, glm::vec3 _size, char* path, Camera* _camera);

    ~Node();

    void Update(float deltaTime, float x, float y);

    void Draw();
};