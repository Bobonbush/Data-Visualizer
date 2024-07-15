#include "stretchbar.h"

StretchBar::StretchBar( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera)
{
    shader = new Shader("outline.vs","outline.fs");
    position = _position;
    
    position.z -= -0.25f;
    size = _size;
    
    texture_width = size.x;
    texture_height = size.y;
    
    float vertices[] = {
        // position                                                            // texture
        position.x + texture_width, position.y , position.z ,           1.0f , 1.0f,   // top right
        
        
        position.x + texture_width, position.y -  texture_height, position.z,   1.0f, 0.0f,  // bottom right
        position.x , position.y - texture_height, position.z,            0.0f ,0.0f, // bottom left
        position.x , position.y , position.z,                    0.0f , 1.0f // top left
    };
    
    
    


    // Apply real position according to camera resolution

    //position = glm::vec3(position.x * half_x + half_x, position.y * half_y + half_y, position.z);

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    



    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
    glBindVertexArray(0); //    Unbind VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);          // Unbind EBO

    texture = TextureLoader::LoadTexture(path);
}

StretchBar::~StretchBar()
{
}

void StretchBar::Update(float deltaTime, float x, float y)
{
   stretch += StretchSensitive * deltaTime;
    if(stretch > 1.0f)
    {
         stretch = 1.0f;
    }

    shader -> use();
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::translate(model , glm::vec3(sin(glfwGetTime()) * minimal_up, std::max(static_cast<float>(sin(glfwGetTime() )* maximal_up), minimal_up ), 0.0f));
    

    shader -> setMat4("view", view);
    shader -> setMat4("model", model);
    shader -> setMat4("projection", projection);
    shader -> setVec4("Color", glm::vec4(0.0f, 0.0f, 0.0f, 255.f/255.f));
    
}

void StretchBar::Draw()
{
    shader -> use();
    shader -> setInt("texture1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    float vertex[] = {
        position.x + texture_width * stretch, position.y , position.z ,           1.0f , 1.0f,   // top right
        position.x + texture_width * stretch, position.y -  texture_height, position.z,   1.0f, 0.0f,  // bottom right
        position.x , position.y - texture_height, position.z,            0.0f ,0.0f, // bottom left
        position.x , position.y , position.z,                    0.0f , 1.0f // top left
    } ;
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex), vertex);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

