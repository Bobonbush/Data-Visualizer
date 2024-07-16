#include "UpdateTool.h"


UpdateTool::UpdateTool(glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera) : Button(_position , _size , path , camera)
{
    
}

UpdateTool::~UpdateTool()
{
}

int UpdateTool::Update(float deltaTime, float x, float y)
{
    return Button::Update(deltaTime , x , y);
}


void UpdateTool::Draw()
{
    Button::Draw();
}






