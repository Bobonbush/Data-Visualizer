#include "algorithmTool.h"

Algorithm::Algorithm( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera) : Button(_position , _size , path , camera)
{
    
}

Algorithm::~Algorithm()
{
}

int Algorithm::Update(float deltaTime, float x, float y)
{
    return Button::Update(deltaTime, x, y);
}


void Algorithm::Draw()
{
    Button::Draw();
}