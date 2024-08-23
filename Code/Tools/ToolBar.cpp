#ifndef TOOLBAR_CPP
#define TOOLBAR_CPP
#include "ToolBar.h"

ToolBar ::ToolBar(Camera * _camera) {
    camera = _camera;
    shader = new Shader("button.vs", "button.fs");
    manager = new Manager(camera);
    Algo = 0;
    
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

    glm::vec3 position = glm::vec3(-0.97f, 0.9f, 0.0f);

    Bar * bar = new Bar(camera, position, glm::vec3(0.001f, 0.0015f, 0.0f));
    bars.push_back(bar);

    float offset_x = -0.55 + 0.97;

    position.x += offset_x;
    Bar * bar2 = new Bar(camera, position, glm::vec3(0.001f, 0.0015f, 0.0f));
    bars.push_back(bar2);

    position.x += offset_x;
    position.y += 0.04f;
    //Button * ok = new Button(position, glm::vec3(1.4f, 1.75f, 0.0f), "ok.png", camera, "");
    //button.push_back(ok);
    
   
    Button* reverse = new Button(glm::vec3(0.92f, 0.75f, 0.0f), glm::vec3(0.2f, 0.2f, 0.0f), "restart.png", camera, "");
    button.push_back(reverse);
    
    
    
    buttonSlot.push_back(button);
    button.clear();
    Barslot.push_back(bars);
    bars.clear();

    bars.push_back(bar);
    position.x -= offset_x;
    //Button * ok2 = new Button(position, glm::vec3(1.4f, 1.75f, 0.0f), "ok.png", camera, "");
    //button.push_back(ok2);
    button.push_back(reverse);
    buttonSlot.push_back(button);
    button.clear();
    
    Barslot.push_back(bars);

    button.push_back(reverse);
    position.y += 0.07f;
    position.x += offset_x /3.f;
    Button * surprise_box = new Button(position, glm::vec3(0.35f, 0.35f, 0.0f), "surprise-box.png", camera, "");
    Button * ConnectedComponent = new Button(position, glm::vec3(0.3f, 0.3f, 0.0f), "Connected.png", camera, "");
    position.y -= 0.02;
    position.x =(2.f - (1.f + position.x)) - 1.f;
    position.x -= offset_x / 4.f;
    Button * MinimumSpanningTree = new Button(position, glm::vec3(0.3f, 0.3f, 0.0f), "MST.png", camera, "");
    Button * fromComputer = new Button(position, glm::vec3(0.3f, 0.3f, 0.0f), "Computer.png", camera, "");
    button.push_back(fromComputer);
    button.push_back(surprise_box);
    buttonSlot.push_back(button);
    button.clear();
    button.push_back(reverse);
    button.push_back(ConnectedComponent);
    button.push_back(MinimumSpanningTree);
    buttonSlot.push_back(button);


    
    button.clear();

    button = buttonSlot[0];
    bars = Barslot[0];
    
}

ToolBar ::~ToolBar() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete shader;

    for(int i = 0 ; i < (int)Barslot.size() ; i++) {
        for(int j = 0; j < (int)Barslot[i].size(); j++) {
            delete Barslot[i][j];
        }
    }
    for(int i = 0 ; i < (int)buttonSlot.size() ; i++) {
        for(int j = 0; j < (int)buttonSlot[i].size(); j++) {
            delete buttonSlot[i][j];
        }
    }
    delete manager;
}

void ToolBar ::Draw() {
    
    //manager -> Draw();
    manager -> Draw();
    
    for(int i = 0; i < bars.size(); i++) {
        bars[i] -> Draw();
    }

    for(int i = 0 ; i< button.size(); i++) {
        button[i] -> Draw();
    }
    
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

void ToolBar ::Update(int _status, float deltaTime, float MouseX, float MouseY) {
    manager -> Update(Algo, deltaTime, MouseX, MouseY);
    status = _status;
    if(status == 3) {
        button = buttonSlot[0];
        bars = Barslot[0];
    } else if(status == 1 || status == 2 || status == 3 || status == 4) {
        button = buttonSlot[1];
        bars = Barslot[1];
    }else if(status == 0) {
        bars.clear();
        button = buttonSlot[2];
    }else if(status == 5) {
        bars.clear();
        button = buttonSlot[3];
    }
    std::string input = "";
    std::string input2 = "";
    if((int)bars.size() > 0){
        input = bars[0] -> Update(deltaTime, MouseX, MouseY);
    }
    if((int)bars.size() > 1) {
        input2 = bars[1] -> Update(deltaTime, MouseX, MouseY);
    }

    //std::cout << input << ' ' << input2 << ' ' << status <<'\n';

    if(input != "inf" && input != "" && status == 1) {
        manager -> Insert(input);
    }
    if(input != "inf" && input != "" && status == 2) {
        manager -> Delete(input);
    }

    if(input != "inf" && input != "" && status == 4) {
        manager -> Search(input);
    }

    if(status == 7) {
        manager -> ShowSize();
    }
    for(int i = 0 ; i < button.size(); i++) {
        
        if(button[i] -> Update(deltaTime, MouseX, MouseY)) {
            if(status == 0 && i == 2) {
                manager -> Initialize();
            }

            if(status == 0 && i == 1) {
                manager -> InitializeReadFile();
            }

            if(status == 5 && i == 1) {
                manager -> ConnectedComponent();
            }
            if(status == 5 && i == 2) {
                manager -> MinimumSpanningTree();
            }

            
            
            if(i == 0) {
                manager -> Reverse();
            }
        }
    }
    
}

#endif