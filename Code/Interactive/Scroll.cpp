#include "Scroll.h"

Scroll::Scroll(Camera * _camera) {

    position = glm::vec3(0.94f, -0.1f, 0.0f);
    size = glm::vec3(0.03f, 1.2f, 0.0f);
    camera = _camera;

    
    
    shader = new Shader("button.vs", "button.fs");
    texture = TextureLoader::LoadTexture("outlinetextbox.png");

    float texture_width = size.x;
    float texture_height = size.y;

    
    float vertices[] = {
        position.x , position.y + texture_height /2.f,               0.0f, 0.0f, 0.0f,         // top left
        position.x + texture_width, position.y + texture_height/2.f, 0.0f, 1.0f, 0.0f, // top right
        position.x + texture_width, position.y - texture_height/2.f, 0.0f, 1.0f, 1.0f,  // bottom right
        position.x, position.y - texture_height/2.f,                 0.0f, 0.0f, 1.0f // bottom left
    };

   

    //pivot = glm::vec3(position.x + texture_width / 2.f, position.y - texture_height / 2.f, 0.0f);



    size.x = texture_width;
    size.y = texture_height;
    //outline = new Outline(position, size, _camera);

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

    iter = new StaticButton(glm::vec3(position.x - size.x/ 1.5, position.y , 0.f), glm::vec3(0.225f, 0.225f, 0.f), "play.png", camera, "");
}

Scroll::~Scroll() {
    delete shader;
    delete camera;
}

void Scroll::Draw() {
    iter -> Draw();
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);

    //shader->setInt("texture1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

float Scroll::Update(float MouseX, float MouseY) {
    iter -> Update(MouseX, MouseY);
    
    if(iter -> position.y > position.y + size.y / 2.f) {
        iter -> position.y = position.y + size.y / 2.f;
    }

    if(iter -> position.y < position.y - size.y / 2.f) {
        iter -> position.y = position.y - size.y / 2.f;
    }

    return min_value + (max_value - min_value) * (iter -> position.y - (position.y - size.y / 2.f)) / size.y;
}

StaticButton::StaticButton(glm::vec3 _position, glm::vec3 _size, char* path, Camera * _camera, std::string _name) : Button(_position, _size, path, _camera, _name) {
    position = _position;
    size = _size;
    camera = _camera;

    
    
    shader = new Shader("button.vs", "button.fs");
    texture = TextureLoader::LoadTexture(path);
    float width = camera -> width;
    float height = camera -> height;
    float texture_width = TextureLoader::GetTextureSize(path).first;
    float texture_height = TextureLoader::GetTextureSize(path).second;
    texture_width = texture_width / width;
    texture_height = texture_height / height;

    texture_width *= size.x;
    texture_height *= size.y;

    
    float vertices[] = {
        position.x , position.y + texture_height /2.f,               0.0f, 0.0f, 0.0f,         // top left
        position.x + texture_width, position.y + texture_height/2.f, 0.0f, 1.0f, 0.0f, // top right
        position.x + texture_width, position.y - texture_height/2.f, 0.0f, 1.0f, 1.0f,  // bottom right
        position.x, position.y - texture_height/2.f,                 0.0f, 0.0f, 1.0f // bottom left
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

StaticButton::~StaticButton() {
    
}

bool StaticButton::isClicked() {
    return glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

void StaticButton::Update(float MouseX, float MouseY) {
    isHovered = isMouseHovered(MouseX, MouseY);
    if(isClicked() && isHovered) {
        float half_y = camera -> height /2.f;
        float y = MouseY;
        
        if(half_y > y) {
            //std::cout << "YES" <<'\n';
            y = half_y - y;
            y /= half_y;
            y =  y;
        } else {
            y = y - half_y;
            y /= half_y;
            y = -y;
        }
        position.y = y;
        //std::cout << position.y << '\n';
    }

    
}

bool StaticButton::isMouseHovered(float MouseX, float MouseY) {
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

    
    

    if (MouseX >= x && MouseX <= x + width && MouseY >= y - height && MouseY <= y + height /2.f) {
        return true;
    }
    return false;
}

void StaticButton::Draw() {
    shader -> use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    pivot = glm::vec3(position.x + size.x / 2.f, position.y - size.y / 2.f, 0.0f);

    if(isHovered) {
        //outline -> Draw();
        model = glm::translate(model, pivot);
        model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.0f));

        model = glm::translate(model, -pivot);
    }

    
    shader -> setMat4("projection", projection);
    shader -> setMat4("view", view);
    shader -> setMat4("model", model);


    float vertices[] = {
        position.x , position.y + size.y /2.f, 0.0f,               0.0f, 0.0f,         // top left
        position.x + size.x, position.y + size.y/2.f, 0.0f, 1.0f, 0.0f, // top right
        position.x + size.x, position.y - size.y / 2.f, 0.0f, 1.0f, 1.0f,  // bottom right
        position.x, position.y - size.y/2.f, 0.0f,                 0.0f, 1.0f // bottom left
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}