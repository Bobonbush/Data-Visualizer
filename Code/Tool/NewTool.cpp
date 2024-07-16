#include "NewTool.h"




NewTool::NewTool( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera) : Button(_position , _size , path , camera)
{
    float sizebetween = 0.2f;
    glm::vec3 subsize = _size * 0.85f;
    glm::vec3 subposition = _position + glm::vec3(sizebetween, 0.0f , 0.0f);
    Button * randoms = new Button(subposition, subsize, "surprise-box.png", camera);
    button.push_back(randoms);
    subposition.x += sizebetween;
    
    Button * fromFile = new Button(subposition, subsize, "computer.png", camera);
    button.push_back(fromFile);
    subposition.x += sizebetween;

    Button * TextBox = new Button(subposition, subsize, "typing.png", camera);
    button.push_back(TextBox);
    subposition.x += sizebetween;
    

}

NewTool::~NewTool()
{
    for(Button *but : button)
    {
        delete but;
    }
}

int NewTool::Update(float deltaTime, float x, float y)
{
    int index = 0;
    if(Choose) {
        for(Button *but : button)
        {
            index++;
            int decide = but -> Update(deltaTime, x, y);
            if(decide)
            {
                Choose = false; 
                return index;
            }
        }
    }
    return Button::Update(deltaTime, x, y);
}


void NewTool::Draw()
{
    if(Choose) {
    for(Button *but : button){
        but -> Draw();
        }
    }
    Button::Draw();
}


