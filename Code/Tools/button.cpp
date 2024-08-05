#include "button.h"

Button::Button(glm::vec3 _position, glm::vec3 _size, char *path,  Camera * _camera, std::string _text) {
    position = _position;
    size = _size;
    camera = _camera;
    text = new TextHandler();
    text->LoadFont("Font/Merriweather-Black.ttf", 32);
    textShader = new Shader("textShader.vs", "textShader.fs");

    
    shader = new Shader("button.vs", "button.fs");
    texture = TextureLoader::LoadTexture(path);
    float width = camera -> width;
    float height = camera -> height;
    float texture_width = TextureLoader::GetTextureSize(path).first;
    float texture_height = TextureLoader::GetTextureSize(path).second;
    texture_width = texture_width / width;
    texture_height = texture_height / height;
    name = _text;

    texture_width *= size.x;
    texture_height *= size.y;
    outline = new Outline(position, glm::vec3(texture_width, texture_height, 0.f), camera);
    position.y -= GetBoxSize().y /2.f;
    float offset = GetBoxSize().x / 40.f;
    position.x -= offset;
    float vertices[] = {
        position.x , position.y + texture_height /2.f, 0.0f,               0.0f, 0.0f,         // top left
        position.x + texture_width, position.y + texture_height/2.f, 0.0f, 1.0f, 0.0f, // top right
        position.x + texture_width, position.y - texture_height/2.f, 0.0f, 1.0f, 1.0f,  // bottom right
        position.x, position.y - texture_height/2.f, 0.0f,                 0.0f, 1.0f // bottom left
        
    };

    pivot = glm::vec3(position.x + texture_width / 2.f, position.y - texture_height / 2.f, 0.0f);



    size.x = texture_width;
    size.y = texture_height;

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Button::~Button() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete shader;
    delete textShader;
    delete text;
}

void Button::Draw() {
    
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    if(outline -> isHovered || isChosen) {
        model = glm::translate(model, pivot);
        model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.0f));
        model = glm::translate(model, -pivot);
    }

    float radius = 0.005f;

    model = glm::translate(model, glm::vec3(sin(glfwGetTime()) * radius, cos(glfwGetTime()) * radius , 0.0f));
    shader -> use();
    shader -> setMat4("projection", projection);
    shader -> setMat4("view", view);
    shader -> setMat4("model", model);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);


    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    textShader -> use();
    textShader -> setMat4("projection", projection);

    float offset = 0.005f;
    float scale =  1.25f;

    if(outline -> isHovered) {
        scale = 1.f;
    }

    float offset_x = camera -> height / camera->width * 0.15f;

    if(outline -> isHovered) {
        text -> RenderMiddleText(textShader, name, position.x + size.x +  offset_x  , position.y , scale, glm::vec3(221.f / 255.f, 235.f/255.f, 233.f/255.f), camera, offset);
    }
    //outline -> Draw();
    
    
}

bool Button::isClicked(float mouseX , float mouseY) {
    if(MouseWait > 0.f) return false;
    MouseWait = MouseWaitLimit;
    return glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}


bool Button::Update(float deltaTime, float mouseX , float mouseY) {
    MouseWait -= deltaTime;
    outline -> Update(mouseX, mouseY);

    
    return outline -> isHovered && isClicked(mouseX, mouseY);
    
}


glm::vec3 Button::GetBoxSize() {
    return outline -> size;
}












Outline:: Outline(glm::vec3 _position, glm::vec3 _size, Camera * _camera) {
    position = _position;
    size = _size;
    camera = _camera;
    shader = new Shader("button.vs", "button.fs");
    //size = glm::vec3(0.25f, 0.35f, 0.0f );

    float width = camera -> width;
    float height = camera -> height;
    float texture_width = TextureLoader::GetTextureSize("outline.png").first;
    float texture_height = TextureLoader::GetTextureSize("outline.png").second;
    texture = TextureLoader::LoadTexture("outline.png");
    

    texture_width = size.x;
    texture_height = size.y;

    float vertices[] = {
        position.x, position.y + texture_height/2.f, 0.0f, 0.0f, 0.0f,         // top left
        position.x + texture_width, position.y + texture_height/2.f, 0.0f, 1.0f, 0.0f, // top right
        position.x + texture_width, position.y - texture_height/2.f, 0.0f, 1.0f, 1.0f,  // bottom right
        position.x, position.y - texture_height/2.f, 0.0f, 0.0f, 1.0f // bottom left
    };

    size.x = texture_width;
    size.y = texture_height;


    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(1);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Outline::~Outline() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete shader;
}

void Outline::Draw() {
    if(isHovered == false) return ;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    shader -> use();
    
    shader -> setMat4("projection", projection);
    shader -> setMat4("view", view);
    shader -> setMat4("model", glm::mat4(1.0f));
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void Outline::Update(float MouseX , float MouseY) {
    isHovered = isInside(MouseX, MouseY);
}

bool Outline::isInside(float MouseX , float MouseY) {
    float width = camera -> width * size.x;
    float height = camera -> height * size.y;
    float x = position.x;
    float y = position.y;
    float half_x = camera -> width /2.f;
    float half_y = camera -> height /2.f;

    x = half_x + x * half_x;
    y = half_y - y * half_y;

    width /= 2.f;
    height /= 2.f;

    //std::cout << x << ' ' << y << ' ' << x + width << ' ' << y + height << '\n';
    //std::cout << MouseX << ' ' << MouseY << '\n';

    
    

    if (MouseX >= x && MouseX <= x + width && MouseY >= y && MouseY <= y + height) {
        return true;
    }
    return false;
}




