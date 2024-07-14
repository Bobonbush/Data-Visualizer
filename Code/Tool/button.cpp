#include "button.h"


Button::Button(glm::vec3 _position , glm::vec3 _size , char* path , Camera * camera) {
    shader = new Shader("button.vs","button.fs");
    position = _position;
    size = _size;
    texture_width = static_cast<float>(TextureLoader::GetTextureSize(path).first) / static_cast<float>(camera -> width);
    texture_height = static_cast<float>(TextureLoader::GetTextureSize(path).second) / static_cast<float>(camera -> height);
    texture_width *= size.x;
    texture_height *= size.y;
    float vertices[] = {
        // position                                                            // texture
        position.x + texture_width, position.y , position.z ,           1.0f , 1.0f,   // top right
        
        
        position.x + texture_width, position.y -  texture_height, position.z,   1.0f, 0.0f,  // bottom right
        position.x , position.y - texture_height, position.z,            0.0f ,0.0f, // bottom left
        position.x , position.y , position.z,                    0.0f , 1.0f // top left
    };
    

    // Apply real position according to camera resolution
    position = glm::vec3(position.x * camera -> width, position.y *  camera -> height, position.z);
    size = glm::vec3(texture_width * camera -> width , texture_height * camera -> height , 0.0f);

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

    glGenVertexArrays(1, &outline_VAO);
    glBindVertexArray(outline_VAO);

    glGenBuffers(1, &outline_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, outline_VBO);
    
    std::vector<glm::vec3> vertices_outline;
    std::vector<unsigned int > indices_outline;
    TextureLoader::buildCircle(1.0f, 180, vertices_outline, indices_outline);

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices_outline.size(), &vertices_outline[0], GL_STATIC_DRAW);
    glGenBuffers(1, &outline_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, outline_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices_outline.size(), &indices_outline[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    
}

Button::~Button() {
    delete this->shader;
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);

    glDeleteBuffers(1, &outline_VBO);
    glDeleteBuffers(1, &outline_EBO);
    glDeleteVertexArrays(1, &outline_VAO);

    //delete[] vertices;
    //delete[] indices;
}

void Button::Update(float deltaTime) {
    // Update button
}

void Button::Draw() {
    shader->use();
    
    shader -> setInt("ourTexture", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glBindVertexArray(VAO);
    

    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    glBindVertexArray(outline_VAO);
    glDrawElements(GL_TRIANGLES, 180, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    

}

