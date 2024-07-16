#include "edge.h"

Edge :: Edge(Node* _u , Node* _v, bool Directed , Camera * _camera) {
    u = _u;
    v = _v;
    camera = _camera;
    shader = new Shader("vertexshader.vs", "fragmentShader.fs");
    this->Directed = Directed;
    float vertices[] = {
        u -> origin.x , u -> origin.y , u -> origin.z,
        v -> origin.x , v -> origin.y , v -> origin.z
    };
    shader -> use();
    glGenVertexArrays(1 , &VAO);
    glGenBuffers(1 , &VBO);
    //glGenBuffers(1 , &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER , VBO);
    glBufferData(GL_ARRAY_BUFFER , sizeof(vertices) , vertices , GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 3 * sizeof(float) , (void*)0);
    glEnableVertexAttribArray(0);
}

Edge:: ~Edge() {
    glDeleteVertexArrays(1 , &VAO);
    glDeleteBuffers(1 , &VBO);
    glDeleteBuffers(1 , &EBO);
    delete shader;
    delete u;
    delete v;
}

void Edge:: Update(float deltaTime) {    
    glLineWidth(10.0f);
    shader -> use();
    glm :: mat4 model = glm::mat4(1.0f);
    
    model = glm::translate(model, glm::vec3(sin(glfwGetTime()) * minimal_up, std::max(static_cast<float>(sin(glfwGetTime() )* maximal_up), minimal_up ), 0.0f));
    glm::mat4 view = glm::mat4(1.0f);
    glm ::mat4 projection = glm::mat4(1.0f);
    shader -> setMat4("model" , model);
    shader -> setMat4("view" , view);
    shader -> setMat4("projection" , projection);

    
}

void Edge:: Draw() {
    shader -> use();
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER , VBO);
    float vertices[] = {
        u -> origin.x , u -> origin.y , u -> origin.z,
        v -> origin.x , v -> origin.y , v -> origin.z
    };
    glBufferSubData(GL_ARRAY_BUFFER , 0 , sizeof(vertices) , vertices);
    glDrawArrays(GL_LINES , 0 , 2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER , 0);
}

int Edge:: getStart() {
    return u -> id;
}

int Edge:: getEnd() {
    return v -> id;
}

int Edge:: getWeight() {
    return w;
}