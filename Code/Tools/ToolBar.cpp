#include "ToolBar.h"


ToolBar ::ToolBar(Camera * _camera) {
    camera = _camera;
    shader = new Shader("button.vs", "button.fs");
    
    float vertices[] {
        1.f, 1.f, 0.f, 1.f, 1.f, // top right
        1.f, 0.8f, 0.f, 1.f, 0.f, // bottom right
        -1.f, 0.8f, 0.f, 0.f, 0.f, // bottom left
        -1.f, 1.f, 0.f, 0.f, 1.f // top left

    };

    unsigned int indices[] {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    outline_Texture = TextureLoader::LoadTexture("UpperBar.png");
}

ToolBar ::~ToolBar() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete shader;
}

void ToolBar ::Draw() {
    
    shader -> use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    shader -> setMat4("projection", projection);
    shader -> setMat4("view", view);
    shader -> setMat4("model", glm::mat4(1.0f));
    glBindVertexArray(VAO);


    glBindTexture(GL_TEXTURE_2D, outline_Texture);
    glActiveTexture(GL_TEXTURE0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void ToolBar ::Update() {
    
}