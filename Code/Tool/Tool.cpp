#include "Tool.h"



Tools::Tools(Camera * camera)
{
    float sizeConst = camera -> height/camera -> width * 0.5f ;
    Delete * del = new Delete(glm::vec3(-0.928f , -0.75f , 0.0f) , glm::vec3(sizeConst , sizeConst , 0.0f) , "delete.png" , camera);
    DataStructure * dataStructure = new DataStructure(glm::vec3(-0.918f , -0.45f , 0.0f) , glm::vec3(sizeConst , sizeConst , 0.0f) , "data.png" , camera);
   
    Search * search = new Search(glm::vec3(-0.918f , -0.15f , 0.0f) , glm::vec3(sizeConst , sizeConst , 0.0f) , "search.png" , camera);
    sizeConst *= 2.f;
    UpdateTool* updateTool = new UpdateTool(glm::vec3(-0.93f , 0.2f , 0.0f) , glm::vec3(sizeConst , sizeConst , 0.0f) , "update.png" , camera); 
    sizeConst *= 0.25/0.4;
    NewTool * newTool = new NewTool(glm::vec3(-0.928f , 0.5f , 0.0f) , glm::vec3(sizeConst , sizeConst , 0.0f) , "new.png" , camera);
    sizeConst *= 2;
    Algorithm * AlgorithmTool = new Algorithm(glm::vec3(-0.95f , 0.9f , 0.0f) , glm::vec3(sizeConst , sizeConst , 0.0f) , "algorithm.png" , camera);
    button.push_back(updateTool);
    button.push_back(search);
    button.push_back(del);
    button.push_back(newTool);
    button.push_back(AlgorithmTool);
    button.push_back(dataStructure);

    textBox = new TextBox(camera);

}

Tools::~Tools()
{
    for(Button * b : button)
    {
        delete b;
    }
    delete textBox;
}

void Tools::Update(float deltaTime, float x, float y)
{
    for(Button * b : button){
        if(status != 0) break;
        status = b -> Update(deltaTime, x, y);
    }

    if(status != 0) {
        for(Button * b : button){
            //b -> Update(deltaTime, x , y);
        }
    }

    if(status == 3) {
        textBox -> Update(deltaTime, x, y);
    }
}

void Tools::Draw()
{
    if(status == 3) {
        textBox -> Draw();
    }
    
    for(Button * b : button)
    {
        
        b -> Draw();
    }
    
}