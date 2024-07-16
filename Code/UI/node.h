#include "Algorithms/shader.h"
#include "Tool/button.h"
#include "Utils/Cursor.h"


class Node : public Button {
private:
    TextHandler * text;
    std::string name;
    Camera * camera;
    Shader* textShader;
    std::vector <Node*> adj;

    bool MouseOver(float x, float y);
    bool isMouseClicked(float x, float y);
    bool Hold = false;
    bool isMouseRelease(float x, float y);

public:

    int id;
    static bool Chosen;
    static bool Over;
    
    Node(glm::vec3 _position, glm::vec3 _size, char* path, Camera* _camera, int _id);

    ~Node();

    int Update(float deltaTime, float x, float y);

    void Draw();
};