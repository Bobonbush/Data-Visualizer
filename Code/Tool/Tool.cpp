#include "Tool.h"



Tools::Tools(Camera * camera)
{
    Delete * del = new Delete(glm::vec3(-0.92f , -0.75f , 0.0f) , glm::vec3(0.2f , 0.2f , 0.0f) , "delete.png" , camera);
    DataStructure * dataStructure = new DataStructure(glm::vec3(-0.92f , -0.45f , 0.0f) , glm::vec3(0.2f , 0.2f , 0.0f) , "data.png" , camera);
   
    Search * search = new Search(glm::vec3(-0.92f , -0.15f , 0.0f) , glm::vec3(0.2f , 0.2f , 0.0f) , "search.png" , camera);
    UpdateTool* updateTool = new UpdateTool(glm::vec3(-0.93f , 0.2f , 0.0f) , glm::vec3(0.4f , 0.4f , 0.0f) , "update.png" , camera); 
    NewTool * newTool = new NewTool(glm::vec3(-0.93f , 0.5f , 0.0f) , glm::vec3(0.25f , 0.25f , 0.0f) , "new.png" , camera);
    Algorithm * AlgorithmTool = new Algorithm(glm::vec3(-0.95f , 0.9f , 0.0f) , glm::vec3(0.5f , 0.5f , 0.0f) , "algorithm.png" , camera);
    button.push_back(updateTool);
    button.push_back(search);
    button.push_back(del);
    button.push_back(newTool);
    button.push_back(AlgorithmTool);
    button.push_back(dataStructure);

}

Tools::~Tools()
{
    for(Button * b : button)
    {
        delete b;
    }
}

void Tools::Update(float deltaTime, float x, float y)
{
    
    for(Button * b : button){
        b -> Update(deltaTime, x, y);
    }
}

void Tools::Draw()
{
    for(Button * b : button)
    {
        
        b -> Draw();
    }
}