#include "bar.h"

Bar::Bar(Camera* _camera, glm::vec3 _position, glm::vec3 _size) {
    camera = _camera;
    shader = new Shader("button.vs", "button.fs");
    position = _position;
    size = _size;
    texture = TextureLoader::LoadTexture("Search Bar.png");
    float texture_width = TextureLoader::GetTextureSize("Search Bar.png").first;
    float texture_height = TextureLoader::GetTextureSize("Search Bar.png").second;
    texture_width = texture_width * size.x;
    texture_height = texture_height * size.y;
    float vertices[] = {
        position.x, position.y + texture_height/2.f, 0.0f, 0.0f, 0.0f,         // top left
        position.x + texture_width , position.y + texture_height/2.f, 0.0f, 1.0f, 0.0f, // top right
        position.x + texture_width, position.y - texture_height/2.f, 0.0f, 1.0f, 1.0f,  // bottom right
        position.x, position.y - texture_height/2.f, 0.0f, 0.0f, 1.0f // bottom left
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
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
}

Bar::~Bar() {
    delete shader;
}

void Bar::Update() {
    
}

void Bar::Draw() {
    shader -> use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    shader -> setMat4("projection", projection);
    shader -> setMat4("view", view);
    shader -> setMat4("model", model);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}