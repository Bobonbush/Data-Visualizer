#include "data.h"


DataStructure::DataStructure( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera) : Button(_position , _size , path , camera)
{
    data = new TextHandler();
    //data->LoadFont("Font/Merriweather-Regular.ttf" , 48);
    data -> LoadFont("Font/Revamped.ttf" , 48);
    current = "GRAPH";                             // Default data structure
    textShader = new Shader("Revamped.vs" , "Revamped.fs");
    

    _camera = camera;

    
}

DataStructure::~DataStructure()
{
    delete data;
    delete textShader;
    delete _camera;
}

void DataStructure::Update(float deltaTime, float x, float y)
{
    text = current;
    Button::Update(deltaTime, x, y);
}


void DataStructure::Draw(){
    data -> RenderText(this -> textShader, current, 1.0f, 1.0f, 3.5f, glm::vec3(0.5f, 1.f, 0.85f) , _camera);
    
    Button::Draw();
}