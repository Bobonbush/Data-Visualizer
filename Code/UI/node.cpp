#include "node.h"

Node::Node(glm::vec3 _position, glm::vec3 _size, char* path, Camera* _camera ,int _id) : Button(_position, _size, path, _camera)
{
    id = _id;
    text = new TextHandler();
    text->LoadFont("Font/AisyKhadijah.ttf", 32);
    shader = new Shader("camera.vs", "camera.fs");
    this->camera = _camera;
    textShader = new Shader("Revamped.vs", "Revamped.fs");
}

Node::~Node()
{
    delete text;
    delete shader;
    delete textShader;
}


bool Node::MouseOver(glm::vec2 hitbox[], float x, float y)
{
    glm::vec2 bottomLeft = hitbox[0];
    glm::vec2 topRight = hitbox[2];
    if(x >= bottomLeft.x && x <= topRight.x && y >= bottomLeft.y && y <= topRight.y)
    {
        return true;
    }
    return false;
}

bool Node::isMouseClicked(float x, float y)
{
    if(glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        return true;
    }
    return false;
}

int Node::Update(float deltaTime, float x, float y)
{

    Button::Update(deltaTime, x, y);

    if(Hold) {
        
        //std::cout << (x - origin.x * camera -> width) <<'\n';
        //std::cout << (y - origin.y * camera -> height) / camera -> height <<'\n';
        LastMouseX = x;
        LastMouseY = y;
        LastMouseY = camera -> height - LastMouseY;
        
        if(LastMouseX < half_x) {
            LastMouseX = -1 + LastMouseX / half_x;
        }else {
            LastMouseX = (LastMouseX - half_x) / half_x;
        }

        if(LastMouseY < half_y) {
            LastMouseY = -1 + LastMouseY / half_y;
        }else {
            LastMouseY = (LastMouseY - half_y) / half_y;
        }

        origin.x += LastMouseX - origin.x ;
        origin.y += LastMouseY - origin.y ;
    }
    
    glm::mat4 view = glm::mat4(1.0f);
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    //view = glm::lookAt(camera->cameraPos, camera->cameraPos + camera->cameraFront, camera->cameraUp);
    glm ::mat4 projection = glm::mat4(1.0f);
    //projection = glm::perspective(glm::radians(camera-> fov), 1.f, 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);

    float scale =  45.f/camera ->fov;
    
    model = glm::translate(model , glm::vec3(sin(glfwGetTime()) * minimal_up, std::max(static_cast<float>(sin(glfwGetTime() )* maximal_up), minimal_up ), 0.0f));
    
    
    shader->use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setMat4("model", model);

    textShader->use();
    textShader->setMat4("view", view);
    textShader -> setMat4("model" , model);
    textShader->setMat4("projection", projection);
    
    return Choose;

}


void Node::Draw()
{
    float x = position.x;
    float y = position.y;

    x += texture_width/2.f;
    y -= texture_height/2.f;
    float scale = 0.5f;

    name = "";
    int temp = id;
    while(temp > 0)
    {
        name += (char)(temp % 10 + '0');
        temp /= 10;
    }
    std::reverse(name.begin() , name.end());
    text -> RenderMiddleText(textShader, name, x, y, 3.f - (scale * ((int)name.size()-1)) , glm::vec3(0.0f, 0.0f, 0.0f), camera, 0.0f);
    

    Button::Draw();

    
    

    //y -= size.y / camera -> height ;

    /*
    //float offset = 0.005f;
    float offset = 0.015f;
    x -= offset  * 0.8f;         // adjust
    y -= offset * 2.4f;            // adjust
    */

    

    
}