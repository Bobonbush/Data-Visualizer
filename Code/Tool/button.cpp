#include "button.h"


Button::Button(glm::vec3 _position , glm::vec3 _size , char* path , Camera * camera) {
    shader = new Shader("button.vs","button.fs");
    position = _position;
    size = _size;
    
    
    

    texture_width = static_cast<float>(TextureLoader::GetTextureSize(path).first) / static_cast<float>(camera -> width);
    texture_height = static_cast<float>(TextureLoader::GetTextureSize(path).second) / static_cast<float>(camera -> height);
    texture_width *= size.x;
    texture_height *= size.y;

    texture_width *= sizeConst;
    float offset = 0.75f;
    position.y -= texture_height /2.f;
    texture_height *= offset;
    position.y += texture_height /2.f;


    position.x += texture_width /8.f ;
    

    

    stretchbar = new StretchBar(position, glm::vec3(texture_width, texture_height , 0.f) , "stretch bar.png", camera);
    
    position.x -= texture_width /8.f;
    position.y -= texture_height /2.f;
    texture_height /= offset;
    position.y += texture_height /2.f;
    texture_width /= sizeConst;

    float vertices[] = {
        // position                                                            // texture
        position.x + texture_width, position.y , position.z ,           1.0f , 1.0f,   // top right
        
        
        position.x + texture_width, position.y -  texture_height, position.z,   1.0f, 0.0f,  // bottom right
        position.x , position.y - texture_height, position.z,            0.0f ,0.0f, // bottom left
        position.x , position.y , position.z,                    0.0f , 1.0f // top left
    };
    

    pivot = glm::vec3(((position.x + texture_width) + position.x )/2.f , (position.y - texture_height + position.y)/2.f , 0);
    
    
    
    


    // Apply real position according to camera resolution
    float half_x = camera -> width / 2;
    float half_y = camera -> height / 2;

    
    if(position.x < 0)
    {
        position.x = half_x - abs(position.x) * half_x ;
    }else
    {
        position.x = position.x * half_x + half_x;
    }
    

    if(position.y < 0)
    {
        position.y = half_y + abs(position.y) * half_y;
    }else
    {
        position.y = half_y - position.y * half_y;
    }


    //position = glm::vec3(position.x * half_x + half_x, position.y * half_y + half_y, position.z);
    size = glm::vec3(texture_width * camera -> width , texture_height * camera -> height , 0.0f);
    size.x /= 2.f;
    size.y /= 2.f;

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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

Button::~Button() {
    delete this->shader;
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);

    glDeleteBuffers(1, &outline_VBO);
    glDeleteBuffers(1, &outline_EBO);
    glDeleteVertexArrays(1, &outline_VAO);

    delete stretchbar;

    //delete[] vertices;
    //delete[] indices;
}

void Button::Update(float deltaTime, float mouseX, float mouseY) {
    // Update button
    stretchbar -> Update(deltaTime, mouseX, mouseY);
    shader -> use();
    shader -> setInt("texture1", 0);
    glm::mat4 view =   glm::mat4(1.0f);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    model = glm::translate(model , glm::vec3(sin(glfwGetTime()) * minimal_up, std::max(static_cast<float>(sin(glfwGetTime() )* maximal_up), minimal_up ), 0.0f));
    if(MouseOver(mouseX, mouseY)) {
        model = glm::translate(model, pivot);
        model = glm::scale(model, glm::vec3(1.25f, 1.25f, 1.25f));
        model = glm::translate(model, -pivot);
    }
    
    shader -> setMat4("model", model);
    shader -> setMat4("view", view);
    shader -> setMat4("projection", projection);

    
}

bool Button::MouseOver(float x, float y) {
    if (x >= position.x && x <= position.x + size.x && y >= position.y && y <= position.y + size.y) {
        return true;
    }
    return false;
}

void Button::Draw() {

    stretchbar -> Draw();
    shader ->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    

    
   
    
    glBindVertexArray(VAO);
    

    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

   // shader -> setBool("isOutline", true);
    //shader -> setVec3("outlineColor", glm::vec3(0.0f, 0.0f, 0.0f));
    
    //glBindVertexArray(outline_VAO);
   // glDrawElements(GL_TRIANGLES, 180, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
    

}

