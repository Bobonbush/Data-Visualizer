#include "algorithmTool.h"

Algorithm::Algorithm( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera) : Button(_position , _size , path , camera)
{
    
}

Algorithm::~Algorithm()
{
}

void Algorithm::Update(float deltaTime)
{
    Button::Update(deltaTime);
}


void Algorithm::Draw()
{
    Button::Draw();
}