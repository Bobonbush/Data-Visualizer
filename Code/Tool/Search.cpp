#include "Search.h"


Search::Search( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera) : Button(_position , _size , path , camera)
{
    
}


Search::~Search()
{
}

int Search::Update(float deltaTime, float x, float y )
{
    return Button::Update(deltaTime, x, y);
}


void Search::Draw()
{
    Button::Draw();
}