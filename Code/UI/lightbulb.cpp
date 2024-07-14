#include "lightbulb.h"


LightBulb::LightBulb( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera) : Button(_position , _size , path , camera)
{
    
}

LightBulb::~LightBulb()
{
}

void LightBulb::Update(float deltaTime)
{
    Button::Update(deltaTime);
}


void LightBulb::Draw()
{
    Button::Draw();
}