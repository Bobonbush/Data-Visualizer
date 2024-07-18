#include "Textbox.h"

int TextBox::cursorPos = -1;
std::string TextBox::textBoxContent = "";

TextBox::TextBox(Camera * _camera) {
    position = glm::vec3(0.7f, 0.8, 0.f);
    size = glm::vec3(1.f - position.x , (1.f + position.y) - (1.f - position.y) , 0.f);
    shader = new Shader("button.vs", "button.fs");
    textShader = new Shader("Revamped.vs", "Revamped.fs");
    
    textHandler = new TextHandler();
    textHandler -> LoadFont("Font/Roboto-Black.ttf", 16);
    camera = _camera;

    box_texture     = TextureLoader::LoadTexture("textbox.png");
    float vertices[] = {
        // position                                                            // texture
        position.x + size.x, position.y , position.z ,           1.0f , 1.0f,   // top right
        position.x + size.x, position.y -  size.y, position.z,   1.0f, 0.0f,  // bottom right
        position.x , position.y - size.y, position.z,            0.0f ,0.0f, // bottom left
        position.x , position.y , position.z,                    0.0f , 1.0f // top left
    };
    
    unsigned int indices[] = {
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

    glfwSetKeyCallback(glfwGetCurrentContext(), KeyCallBack);
}

TextBox::~TextBox() {
    
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);


    delete shader;
    delete textShader;
    delete textHandler;
}

void TextBox :: Draw() {
    TimeWait -= 0.01f;
    if(TimeWait <= 0.f) {
        TimeWait = TimeWaitMaximum;
        if(textBoxContent[cursorPos + 1] == '|') {
            TextBox::textBoxContent.erase(cursorPos + 1, 1);
        }
        else
            TextBox::textBoxContent.insert(cursorPos + 1, 1, '|');
    }
    float offsetx = 0.0005;
    float offsety = 0.03f;
    textHandler -> RenderTextInRangeX(textShader, "DATA :", position.x, position.y + offsety /2.f  , 4.5f, glm::vec3(0.6f, 1.0f, 1.0f), camera, offsetx, offsety, position.x + size.x, position.y - size.y);
    textHandler -> RenderTextInRangeX(textShader, TextBox::textBoxContent, position.x, position.y - offsety , 3.f, glm::vec3(0.0f, 0.0f, 0.0f), camera, offsetx, offsety, position.x + size.x, position.y - size.y);
    
    
    shader -> use();
    glBindVertexArray(VAO);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, box_texture);

    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    /*
    glBindVertexArray(outline_VAO);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, outline_texture);


    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    */
}

void TextBox::Update(float deltaTime, float x, float y) {
    shader -> use();
    shader -> setInt("texture1", 0);
    glm::mat4 view =   glm::mat4(1.0f);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    //glm :: mat4 projection = glm::mat4(1.0f);
    shader -> setMat4("view", view);
    shader -> setMat4("model", model);
    shader -> setMat4("projection", projection);

    textShader -> use();

    glm::mat4 textProjection = glm::mat4(1.0f);
    textShader -> setMat4("projection", textProjection);
    textShader -> setMat4("view", view);
    textShader -> setMat4("model", model);
}

void TextBox::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(!window) std::cout << "BAD" <<'\n';
    
    if(action == GLFW_PRESS || action == GLFW_REPEAT) {
        
        if(key == GLFW_KEY_BACKSPACE) {
            if(cursorPos >= 0) {
                if(TextBox::textBoxContent[cursorPos + 1] == '|') {
                    TextBox::textBoxContent.erase(cursorPos + 1, 1);
                }
                textBoxContent.erase(cursorPos, 1);
                TextBox::textBoxContent.insert(cursorPos, 1, '|');
                cursorPos--;
            }
        }else if (key == GLFW_KEY_LEFT) {
            if(TextBox::textBoxContent[cursorPos + 1] == '|') {
                TextBox::textBoxContent.erase(cursorPos + 1, 1);
            }
            if(cursorPos >= 0) {
                cursorPos--;
            }
            
            TextBox::textBoxContent.insert(cursorPos + 1, 1, '|');

        }else if (key == GLFW_KEY_RIGHT) {
            if(TextBox::textBoxContent[cursorPos + 1] == '|') {
                TextBox::textBoxContent.erase(cursorPos + 1, 1);
            }
            if(cursorPos < (int) textBoxContent.size()-1) {
                cursorPos++;
            }
            TextBox::textBoxContent.insert(cursorPos + 1, 1, '|');
        }else if (key == GLFW_KEY_ENTER) {
            if(TextBox::textBoxContent[cursorPos + 1] == '|') {
                TextBox::textBoxContent.erase(cursorPos + 1, 1);
            }

            TextBox::textBoxContent.insert(cursorPos+1 , 1, '\n');
            
            cursorPos++;
        }else if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
            if(TextBox::textBoxContent[cursorPos + 1] == '|') {
                TextBox::textBoxContent.erase(cursorPos + 1, 1);
            }
            textBoxContent.insert(cursorPos + 1 , 1, key);
            cursorPos++;
        }else if (key == GLFW_KEY_SPACE) {
            if(TextBox::textBoxContent[cursorPos + 1] == '|') {
                TextBox::textBoxContent.erase(cursorPos + 1, 1);
            }
            textBoxContent.insert(cursorPos + 1, 1, ' ');
            cursorPos++;
        }
        
    }
}
