#include "slideTools.h"

SlideTools::SlideTools(Camera * _camera) {
    camera = _camera;
    shader = new Shader("button.vs", "button.fs");

    buttons = std::vector<Button*>();
    glm::vec3 position = glm::vec3(-0.98, 0.75f, 0.0f);
    glm::vec3 size = glm::vec3(0.4f, 1.8f, 0.0f);
    Button * add = new Button(position, glm::vec3(0.25f, 0.25f, 0.0f), "add.png", camera, "Add");
    buttons.push_back(add);
    texture = TextureLoader::LoadTexture("slide.png");
    
    
    
    
    position.y -= add ->GetBoxSize().y;
    float off_set = 0.05f;
    position.y -= off_set;
    position.x += off_set * 0.2f;
    
    Button * remove = new Button(position , glm::vec3(0.17f, 0.17f, 0.0f), "cancel.png", camera, "Delete");
    buttons.push_back(remove);
    
    position.y -= add ->GetBoxSize().y;

    position.y -= off_set;

    Button * edit = new Button(position, glm::vec3(0.2f, 0.2f, 0.0f), "Create.png", camera, "Update");   
    buttons.push_back(edit);
    /*
    position.y -= add ->GetBoxSize().y;

    Button * New = new Button(position, glm::vec3(0.3f, 0.3f, 0.0f), "res/new.png", camera);
    buttons.push_back(New);

    position.y -= add -> GetBoxSize().y;

    Button * Algorithm = new Button(position, glm::vec3(0.3f, 0.3f, 0.0f), "res/algorithm.png", camera);
    buttons.push_back(Algorithm);
    */
    float width = camera -> width;
    float height = camera -> height;
    float texture_width = size.x;
    float texture_height = size.y;
    position = glm::vec3(-1.f, 0.8f, 0.0f);

    float vertices[] = {
        position.x , position.y, 0.0f, 0.0f, 0.0f,         // top left
        position.x + texture_width, position.y, 0.0f, 1.0f, 0.0f, // top right
        position.x + texture_width, position.y - texture_height, 0.0f, 1.0f, 1.0f,  // bottom right
        position.x, position.y - texture_height, 0.0f, 0.0f, 1.0f // bottom left
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
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

SlideTools::~SlideTools() {
    for (int i = 0; i < buttons.size(); i++) {
        delete buttons[i];
    }
    delete shader;
}


void SlideTools::Draw() {
    shader -> use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    shader -> setMat4("projection", projection);
    shader -> setMat4("view", view);
    shader -> setMat4("model", model);
    for (int i = 0; i < buttons.size(); i++) {
       buttons[i] -> Draw();
    }
    /*
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    */

}

void SlideTools::Update(float MouseX, float MouseY) {
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i] -> Update(MouseX, MouseY);
    }
}
