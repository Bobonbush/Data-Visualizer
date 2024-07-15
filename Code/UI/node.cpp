#include "node.h"

Node::Node(glm::vec3 _position, glm::vec3 _size, char* path, Camera* _camera) : Button(_position, _size, path, _camera)
{
    text = new TextHandler();
    text->LoadFont("Font/AisyKhadijah.ttf", 32);
    shader = new Shader("camera.vs", "camera.fs");
    this->camera = _camera;
    name = "321";
    textShader = new Shader("Revamped.vs", "Revamped.fs");
}

Node::~Node()
{
    delete text;
    delete shader;
    delete textShader;
}

void Node::Update(float deltaTime, float x, float y)
{





    //Button::Update(deltaTime, x, y);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    //glm::mat4 view = glm::lookAt(camera->cameraPos, camera->cameraPos + camera->cameraFront, camera->cameraUp);
    glm ::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(camera-> fov), 1.f, 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);
    shader->use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    shader->setMat4("model", model);

    textShader->use();
    textShader->setMat4("view", view);
    textShader->setMat4("projection", projection);
    textShader->setMat4("model", model);


 
}


void Node::Draw()
{
    Button::Draw();
    float x = position.x;
    float y = position.y;
    float half_x = camera->width / 2;
    float half_y = camera->height / 2;
    if(x > half_x)
    {
        x = (x - half_x) / half_x;
    }else
    {
        x = -1 + x / half_x;
    }

    if(y > half_y)
    {
        y = (half_y - y) / half_y;
    }else
    {
        y = -1 + y / half_y;
    }

    x += size.x / camera->width * 2.f ;
    y -= size.y / camera->height * 2.f;
    y += size.y / camera->height /1.4f ; 

    //y -= size.y / camera -> height ;

    /*
    //float offset = 0.005f;
    float offset = 0.015f;
    x -= offset  * 0.8f;         // adjust
    y -= offset * 2.4f;            // adjust
    */
    float offset =  0.00f;
       
    float scale = 0.5f;
    y += scale * 0.01f *(int)(name.size()-1);
    x -= scale * 0.005f * (int)(name.size());
    //x -= offset * ((int)name.size()-1) ;
    //y += scale *((int)name.size()-1) * offset * 1.5f ;


    text->RenderMiddleText(textShader ,name, x, y, 3.f -(scale * ((int)name.size()-1)), glm::vec3(0.0f, 0.0f, 0.0f), camera , offset/2.f);
}