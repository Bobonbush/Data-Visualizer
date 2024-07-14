#include "animation.h"


Animation::Animation(glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera)
{
    play = new Button(_position , _size , "play.png" , camera);

}

Animation::~Animation()
{
    delete play;
}

void Animation::Update(float deltaTime)
{
    play->Update(deltaTime);
}


void Animation::Draw()
{
    play->Draw();
}

