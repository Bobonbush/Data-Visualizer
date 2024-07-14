#include "NewTool.h"




NewTool::NewTool( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera) : Button(_position , _size , path , camera)
{
    
}

NewTool::~NewTool()
{
}

void NewTool::Update(float deltaTime, float x, float y)
{
    Button::Update(deltaTime, x, y);
}


void NewTool::Draw()
{
    Button::Draw();
}

