#include "ui.h"

UI::UI(Camera* camera)
{
    LightBulb * lightBulb = new LightBulb(glm::vec3(0.87f, -0.8f, 0.0f), glm::vec3(0.4f, 0.4f, 0.0f), "light-mode.png", camera);
    buttons.push_back(lightBulb);
}


UI::~UI()
{
    for (int i = 0; i < buttons.size(); i++)
    {
        delete buttons[i];
    }
}

void UI::Update(float deltaTime)
{
    for (int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->Update(deltaTime);
    }
}


void UI::Draw()
{
    for (int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->Draw();
    }
}



