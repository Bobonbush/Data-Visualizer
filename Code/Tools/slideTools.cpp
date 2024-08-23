#include "slideTools.h"

SlideTools::SlideTools(Camera * _camera) {
    camera = _camera;
    shader = new Shader("button.vs", "button.fs");

    buttons = std::vector<Button*>();
    glm::vec3 position = glm::vec3(-0.97, 0.7f, 0.0f);
    glm::vec3 size = glm::vec3(0.4f, 1.8f, 0.0f);
    Button * add = new Button(position, glm::vec3(0.3f, 0.3f, 0.0f), "add.png", camera, "Add");
    texture = TextureLoader::LoadTexture("slide.png");
    
    
    
    
    position.y -= add ->GetBoxSize().y;
    float off_set = 0.2f;
    position.y -= off_set;
    position.x += off_set * 0.1f;
    
    Button * remove = new Button(position , glm::vec3(0.2f, 0.2f, 0.0f), "cancel.png", camera, "Delete");
    
    position.y -= add ->GetBoxSize().y;

    position.y -= off_set;

    Button * edit = new Button(position, glm::vec3(0.22f, 0.22f, 0.0f), "Create.png", camera, "Update");   
    
    Button* Search = new Button(position, glm::vec3(0.22f, 0.22f, 0.0f), "search.png", camera, "Search");

    Button * getTop = new Button(position, glm::vec3(0.22f, 0.22f, 0.0f), "top.png", camera, "Get Top");
    
    position.y -= add ->GetBoxSize().y;

    position.y -= off_set;

    Button * New = new Button(position, glm::vec3(0.22f, 0.22f, 0.0f), "shuttle.png", camera, "Initialize");
    buttons.push_back(New);
    
    position.y -= add -> GetBoxSize().y;
    position.y -= off_set;
    position.x -= off_set * 0.05f;

    Button * Algorithm = new Button(position, glm::vec3(0.3f, 0.35f, 0.0f), "algorithm.png", camera, "Algorithm");
    buttons.push_back(Algorithm);
    slot.push_back(buttons);
    buttons.clear();


    Button * GetSize = new Button(position, glm::vec3(0.3f, 0.35f, 0.0f), "size.png", camera, " Get Size");
    buttons.push_back(add);
    buttons.push_back(remove);
    buttons.push_back(getTop);
    
    buttons.push_back(GetSize);
    buttons.push_back(New);
    slot.push_back(buttons);
    
    // use for graph
    
    buttons.clear();          

    buttons.push_back(add);
    buttons.push_back(remove);
    //buttons.push_back(edit);
    buttons.push_back(Search);
    buttons.push_back(New);

    // Use for trees and hash
    slot.push_back(buttons);

    buttons.clear();
   
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
    for(int i = 0; i < slot.size(); i++) {
        for(int j = 0; j < slot[i].size(); j++) {
            delete slot[i][j];
        }
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

void SlideTools::Update(int Algo, float deltaTime, float MouseX, float MouseY) {
    
    // buttons[0] = {add, remove, edit, New, Algorithm}


    //butons[1] = {add , remove , get Top, Get Size, New}              
    //buttons[2] = {add , remove, Search , New}                             
    if(Algo == 3) {
        buttons = slot[0];
    }else if(Algo == 4) {
        buttons = slot[1];
    } else  {
        buttons = slot[2];
    }
    int index = 0;
    for (int i = 0; i < (int) buttons.size(); i++) {
        index++;
        buttons[i] -> isChosen = false;
        if(Algo == 3 && buttons[i] -> Update(deltaTime, MouseX, MouseY)) {
            if(index == 1) status = 0;
            if(index == 2) status = 5;
        }
        if(Algo == 4 && buttons[i] -> Update(deltaTime, MouseX, MouseY)) {
            if(index == 1) status = 1;
            if(index == 2) status = 2;
            if(index == 3) status = 5;
            if(index == 4) status = 7;
            if(index == 5) status = 0;
        }

        if(Algo != 3 && Algo != 4 && buttons[i] -> Update(deltaTime, MouseX, MouseY)) {
            if(index == 1) status = 1;
            if(index == 2) status = 2;
            if(index == 3) status = 4;
            if(index == 4) status = 0;
        }
    }

    if(Algo == 3) {
        if(status == 0) buttons[0] -> isChosen = true;
        if(status == 5) buttons[1] -> isChosen = true;
    }
    if(Algo == 4) {
        if(status == 1 ) buttons[0] -> isChosen = true;
        if(status == 2) buttons[1] -> isChosen = true;
        if(status == 5) buttons[2] -> isChosen = true;
        if(status == 7) buttons[3] -> isChosen = true;
        if(status == 0) buttons[4] -> isChosen = true;
    }

    if(Algo != 3 && Algo != 4) {
        if(status == 1 ) buttons[0] -> isChosen = true;
        if(status == 2) buttons[1] -> isChosen = true;
        if(status == 4) buttons[2] -> isChosen = true;
        if(status == 0) buttons[3] -> isChosen = true;
    }
    
}
