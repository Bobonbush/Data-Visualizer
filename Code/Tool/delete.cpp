#include "delete.h"


Delete::Delete(glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera) : Button(_position , _size , path , camera)
{
    
}

Delete::~Delete()
{
}

int Delete::Update(float deltaTime, float x, float y)
{
    return Button::Update(deltaTime, x, y);
}

void Delete::Draw()
{
    Button::Draw();
}