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
    glm:: mat4 view = glm::mat4(1.0f);
  
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    text = current;
    Button::Update(deltaTime, x, y);

    textShader -> use();
    textShader -> setMat4("view", view);
    textShader -> setMat4("projection", projection);
    textShader -> setMat4("model", model);
}


void DataStructure::Draw(){
    data -> RenderText(this -> textShader, current, 1.0f, 1.0f, 3.5f, glm::vec3(0.5f, 1.f, 0.85f) , _camera ,0.0f);
    
    Button::Draw();
}