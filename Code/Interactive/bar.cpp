#include "bar.h"



std::string Bar::input = "";
int Bar::cursor = 0;
bool Bar::CanHaveComma = true;
bool Bar::CanHaveSpace = true;
bool Bar::Enter = false;
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
    size.x = texture_width;
    size.y = texture_height;

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

    text = new TextHandler();
    text -> LoadFont("Font/Revamped.ttf", 24);
    textShader = new Shader("textShader.vs", "textShader.fs");
    glfwSetKeyCallback(glfwGetCurrentContext(), KeyCallBack);
}

Bar::~Bar() {
    delete shader;
    delete text;
}

bool Bar::IsClicked(float x, float y) {
    if(glfwGetMouseButton(glfwGetCurrentContext() , GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        return true;
    }
    return false;
}

bool Bar::IsHovered(float MouseX, float MouseY) {
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

    
    

    if (MouseX >= x && MouseX <= x + width && MouseY >= y - height/2.f && MouseY <= y + height /4.f) {
        return true;
    }
    return false;
}

std::string Bar::Update(float deltaTime , float MouseX, float MouseY) {
    if(isChosen) {
        waitingTime -= deltaTime;
        if(waitingTime <= 0) {
            waitingTime = waitingTimeLimit;
            if(cursor >= 1 && input[cursor-1] == '|') {
                input.erase(cursor-1, 1);
                cursor--;
            } else {
                input.insert(cursor, 1, '|');
                cursor++;
            }
        }
    }
    if(IsClicked(MouseX, MouseY)) {
        isChosen = false;
    }

    if(IsHovered(MouseX , MouseY) && IsClicked(MouseX, MouseY)) {
        isChosen = true;
    }

    if(Enter) {
        isChosen = false;
        Enter = false;
        std::string ret =  input;
        input = "";
        cursor = 0;        
        for(int i = 0; i < ret.size(); i++) {
            if(ret[i] == '|') {
                ret.erase(i, 1);
                break;
            }
        }
        
        return ret;
    }
    
    return "inf";
}

void Bar::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) {
        if(key == GLFW_KEY_BACKSPACE) {
            
            if(cursor > 0) {
                if(input[cursor-1] == '|') {
                    input.erase(cursor-1, 1);
                    cursor--;
                }
                if(cursor > 0) {
                    cursor--;
                    input.erase(cursor, 1);
                }
            }
        } else if(key == GLFW_KEY_SPACE && CanHaveSpace) {
            if(cursor > 0 && input[cursor-1] == '|') {
                input.erase(cursor-1, 1);
                cursor--;
            }
            
            input.insert(cursor, 1, ' ');
            cursor++;
        } else if(key == GLFW_KEY_ENTER) {
            Enter = true;
        } else if (key <= GLFW_KEY_9 && key >= GLFW_KEY_0) {
            if(cursor > 0 && input[cursor-1] == '|') {
                input.erase(cursor-1, 1);
                cursor--;
            }
            input.insert(cursor, 1, key);
            cursor++;
        }else if(key ==GLFW_KEY_COMMA && CanHaveComma) {
            if(cursor > 0 && input[cursor-1] == '|') {
                input.erase(cursor-1, 1);
                cursor--;
            }
            input.insert(cursor, 1, ',');
            cursor++;
        }else if(key == GLFW_KEY_LEFT) {
            if(cursor > 0) {
                if(input[cursor-1] == '|') {
                    input.erase(cursor, 1);
                    cursor--;
                }
                cursor--;
            } 
            
        } else if(key == GLFW_KEY_RIGHT) {
            if(cursor < input.size()-1) {
                if(input[cursor] == '|') {
                    input.erase(cursor, 1);
                    cursor++;
                }
            }
        }
    }
}



void Bar::Draw() {
    shader -> use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    shader -> setMat4("projection", projection);
    shader -> setMat4("view", view);
    shader -> setMat4("model", model);
    
    textShader -> use();
    float offset = 0.0005f;
    textShader -> setMat4("projection", projection);
    text -> RenderText(textShader, input, position.x + offset * 20 , position.y - offset * 40, 2.75f, glm::vec3(0.0f, 0.0f, 0.0f),camera, offset);
    shader ->use();
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
}