#include "Algorithms/shader.h"
#include "Tool/button.h"


class Node : public Button {
private:
    TextHandler * text;
    std::string name;
    Camera * camera;
    Shader* textShader;
    std::vector <Node*> adj;

    bool MouseOver(glm::vec2 hitbox [], float x, float y);
    bool isMouseClicked(float x, float y);
public:

    int id;
    
    Node(glm::vec3 _position, glm::vec3 _size, char* path, Camera* _camera, int _id);

    ~Node();

    int Update(float deltaTime, float x, float y);

    void Draw();
};