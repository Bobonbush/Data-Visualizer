#include "slidebox.h"


SlideBox::SlideBox(glm::vec3 _position, glm::vec3 _size , Camera * _camera, std::string _data) {
    data = _data;
    camera = _camera;
    position = _position;
    size = _size;
    shader = new Shader("textShader.vs", "textShader.fs");
    text = new TextHandler();
    text -> LoadFont("Font/Revamped.ttf", 48);

    outline = new OutlineSlideBox(position, size, camera);
    

}

SlideBox::~SlideBox() {
    delete shader;
    delete text;

}

bool SlideBox::isClicked() {
    if(MouseWait > 0.f) return false;
    MouseWait = MouseWaitLimit;
    return glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

bool SlideBox::Update(float deltaTime, float MouseX , float MouseY) {
    MouseWait -= deltaTime;
    outline -> position = position;
    outline -> position.y += 0.05f; 
    outline -> Update(MouseX , MouseY);
    
    return outline -> isHovered && isClicked();
}

void SlideBox::Draw() {
    shader -> use();
    shader -> setMat4("projection", glm::mat4(1.0f) );
    float offset = camera -> height / camera -> width * 0.009f;
    text -> RenderText(shader, data, position.x + 0.1f, position.y  , 2.f, glm::vec3(75.f/255.f, 213.f/255.f, 226.f/255.f), camera, offset);
    
    outline -> Draw(isChosen);
}


OutlineSlideBox::OutlineSlideBox(glm::vec3 _position, glm::vec3 _size , Camera * _camera) {
    position = _position;
    size = _size;
    camera = _camera;
    shader = new Shader("button.vs", "button.fs");
    size = glm::vec3(0.63f, 0.15f, 0.0f );
    position.y += 0.05f;
    //position.x -= 0.13f;

    float width = camera -> width;
    float height = camera -> height;
    float texture_width = TextureLoader::GetTextureSize("outline.png").first;
    float texture_height = TextureLoader::GetTextureSize("outline.png").second;
    texture = TextureLoader::LoadTexture("slideOutLine.png");
    

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

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

OutlineSlideBox::~OutlineSlideBox() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteBuffers(1, &EBO);
    delete shader;
}

void OutlineSlideBox:: Draw(bool isChosen) {
    if(!isHovered) return ;
    if(isChosen == true ) return ;
    shader -> use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    shader -> setMat4("projection", projection);
    shader -> setMat4("view", view);
    shader -> setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    float texture_width = size.x;
    float texture_height = size.y;


    float vertices[] = {
        position.x, position.y + texture_height/2.f, 0.0f, 0.0f, 0.0f,         // top left
        position.x + texture_width, position.y + texture_height/2.f, 0.0f, 1.0f, 0.0f, // top right
        position.x + texture_width, position.y - texture_height/2.f, 0.0f, 1.0f, 1.0f,  // bottom right
        position.x, position.y - texture_height/2.f, 0.0f, 0.0f, 1.0f // bottom left
    };

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void OutlineSlideBox:: Update(float MouseX , float MouseY) {
    
    isHovered = isInside(MouseX, MouseY);
}

bool OutlineSlideBox::isInside(float MouseX , float MouseY) {
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

    
    

    if (MouseX >= x && MouseX <= x + width && MouseY >= y - height/2.f  && MouseY <= y + height/2.f) {
        return true;
    }
    return false;
}



