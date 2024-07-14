#include "Search.h"


Search::Search( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera) : Button(_position , _size , path , camera)
{
    
}


Search::~Search()
{
}

void Search::Update(float deltaTime)
{
    Button::Update(deltaTime);
}


void Search::Draw()
{
    Button::Draw();
}