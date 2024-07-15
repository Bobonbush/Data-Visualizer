#include "ui.h"

UI::UI(Camera* _camera)
{
    camera = _camera;
    LightBulb * lightBulb = new LightBulb(glm::vec3(0.87f, -0.8f, 0.0f), glm::vec3(0.4f, 0.4f, 0.0f), "light-mode.png", camera);
    buttons.push_back(lightBulb);

    waiting = new TextHandler();
    waiting->LoadFont("Font/wheaton capitals.otf", 48);

    textShader = new Shader("Revamped.vs", "Revamped.fs");
}


UI::~UI()
{
    for (int i = 0; i < buttons.size(); i++)
    {
        delete buttons[i];
    }
    for(int i = 0; i < nodes.size(); i++)
    {
        delete nodes[i];
    }

    delete waiting;
}

void UI::Update(float deltaTime, float x, float y)
{



    for (int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->Update(deltaTime, x, y);
    }

    for (int i = 0; i < nodes.size(); i++)
    {
        nodes[i]->Update(deltaTime, x, y);
    }

    waitingTime -= deltaTime;
    if(waitingTime < 0)
    {
        waitingTime = maximumWaitingTime;
        waitingText.push_back('.');
        if(waitingText.size() > 19)
        {
            waitingText = "Waiting for data";
        }
    }

      
    
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(sin(glfwGetTime()) * maximal_up, cos(glfwGetTime()) * maximal_up, 0.0f));
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    textShader->use();
    textShader->setMat4("view", view);
    textShader->setMat4("projection", projection);
    textShader->setMat4("model", model);
    
}


void UI::Draw()
{
    for (int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->Draw();
    }

    for (int i = 0; i < nodes.size(); i++)
    {
        nodes[i]->Draw();
    }

    if(nodes.empty()) waiting -> RenderText(textShader, waitingText,-0.5f, -0.05f, 3.f, glm::vec3(0.2f, 0.4f, 0.4f), camera, 0.03f);

}



