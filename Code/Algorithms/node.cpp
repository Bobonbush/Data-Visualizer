#include "node.h"
bool Node::isDragging = false;
bool Node::isOver = false;
float Node::speed = 3.f;
Node::Node(glm::vec3 _position, glm::vec3 _size, Camera * _camera, int _value) {
    value = _value;
    left = nullptr;
    right = nullptr;
    height = 1;

    position = _position;
    size = _size;
    camera = _camera;
    shader = new Shader("Button.vs", "Button.fs");
    lineShader = new Shader("line.vs", "line.fs");

    float width = camera -> width;
    float height = camera -> height;
    float texture_width = TextureLoader::GetTextureSize("node.png").first;
    float texture_height = TextureLoader::GetTextureSize("node.png").second;
    texture = TextureLoader::LoadTexture("node.png");
    textureHovered = TextureLoader::LoadTexture("on_node.png");
    textureDel = TextureLoader::LoadTexture("node_del.png");
    textureNew = TextureLoader::LoadTexture("new_node.png");
    textureRelated = TextureLoader::LoadTexture("traverse.png");
    textureFound = TextureLoader::LoadTexture("node_found.png");
    texture_width = texture_width / width;
    texture_height = texture_height / height;
    
    texture_width  = texture_width  * size.x;
    texture_height = texture_height * size.y;

    int temp = value;
    while(temp > 0 ) {
        text.push_back(temp % 10 + '0');
        temp /= 10;
    }
    std::reverse(text.begin(), text.end());

    textHandler = new TextHandler();
    textHandler->LoadFont("Font/AisyKhadijah.ttf", 24);
    textShader = new Shader("textShader.vs", "textShader.fs");

    float vertices[] = {
        position.x - texture_width /2.f, position.y + texture_height/2.f, 0.0f, 1.0f, 1.0f, // top right
        position.x + texture_width / 2.f, position.y - texture_height /2.f, 0.0f, 1.0f, 0.0f, // bottom right
        position.x + texture_width / 2.f , position.y - texture_height/2.f, 0.0f, 0.0f, 0.0f, // bottom left
        position.x - texture_width / 2.f, position.y + texture_height/2.f, 0.0f, 0.0f, 1.0f // top left
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

Node::~Node() {
    delete left;
    delete right;
}


void Node::RecalculatePosition(float offsetx, float offsety) {
    float bonus = 0.02f;
    //status = -1;
    if(left != nullptr) {
        left -> targetPosition.x = targetPosition .x - offsetx - ((1 << left -> getHeight(left))  +2) * bonus;
        left -> targetPosition.y = targetPosition.y - offsety;
        left -> RecalculatePosition(offsetx, offsety);
    }
    if(right != nullptr) {
        right -> targetPosition.x = targetPosition.x + offsetx + bonus *  ((1 << right ->getHeight(right) )+ 2) ;
        right -> targetPosition.y = targetPosition.y - offsety;
        right -> RecalculatePosition(offsetx, offsety);
    }
}

int Node::SubTreeHeight(Node * node) {
    if(node == nullptr) {
        return 0;
    }
    return 1 + std::max(SubTreeHeight(node -> left), SubTreeHeight(node -> right));
}

int Node::getHeight(Node* node) {
    if(node == nullptr) {
        return 0;
    }
    return node -> height;
}


int Node::getBalance(Node* node) {
    if(node == nullptr) {
        return 0;
    }
    return getHeight(node -> left) - getHeight(node -> right);
}

Node* Node::rightRotate(Node* y) {
    Node* x = y -> left;
    Node* T2 = x -> right;

    x -> status = 3;
    y -> status = 3;

    x -> right = y;
    y -> left = T2;

    y -> height = 1 + std::max(getHeight(y -> left), getHeight(y -> right));
    x -> height = 1 + std::max(getHeight(x -> left), getHeight(x -> right));

    return x;
}

Node* Node::leftRotate(Node* x) {
    Node* y = x -> right;
    Node* T2 = y -> left;

    x -> status = 3;
    y -> status = 3;

    y -> left = x;
    x -> right = T2;

    x -> height = 1 + std::max(getHeight(x -> left), getHeight(x -> right));
    y -> height = 1 + std::max(getHeight(y -> left), getHeight(y -> right));

    return y;
}


void Node::Update(float deltaTime, float mouseX, float mouseY) {
    
    if(!Node::isDragging) {
        isHovered = isInside(mouseX, mouseY);
        if(isHovered) {
            Node::isDragging = true;
        }
    }
    if(!isOver) {
        isOver = isInside(mouseX, mouseY);
    }

    
    textShader -> use();
    textShader -> setMat4("projection", glm::mat4(1.0f));

    if(left != nullptr) {
        left -> Update(deltaTime, mouseX, mouseY);
    }
    if(right != nullptr) {
        right -> Update(deltaTime, mouseX, mouseY);
    }


    if(isOver) {
        Cursor::CurrentCursor = Cursor::ReadyCursor;
    }
    
    if(isHovered) {
        Cursor::CurrentCursor = Cursor::ReadyCursor;
        if(glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            float half_x = camera -> width / 2.f;
            float half_y = camera -> height / 2.f;
            mouseX = (mouseX - half_x) / half_x;
            mouseY = (half_y - mouseY) / half_y;
            targetPosition.x = mouseX;
            position.x = mouseX;
            targetPosition.y = mouseY;
            position.y = mouseY;
            Cursor::CurrentCursor = Cursor::HoldCursor;
        } else {
            isDragging = false;
            isHovered = false;
        }
    }

    if(!isOver) Cursor::CurrentCursor = Cursor::normalCursor;

    

    Cursor::SetCursor(glfwGetCurrentContext());

    if(status != -1) {

        return ;
    }

    
    
    
    
    float offset_x = targetPosition.x - position.x;
    float offset_y = targetPosition.y - position.y;
    

    if(offset_x < 0.01f && offset_x > -0.01f) {
        offset_x = 0.f;
    }
    if(offset_y < 0.01f && offset_y > -0.01f) {
        offset_y = 0.f;
    }
    if(offset_x == 0.f && offset_y == 0.f) {
        Animating = false;
    } else Animating = true;
    
    position.x += offset_x * speed * deltaTime;
    position.y += offset_y * speed * deltaTime;
}

void Node::Draw() {
    
    if(left != nullptr) {
        left -> Draw();
    }
    if(right != nullptr) {
        right -> Draw();
    }
    float offset = 0.0005f;
    textHandler -> RenderMiddleText(textShader, text, position.x, position.y, 1.5f, glm::vec3(0.1f, 0.2f, 0.6f),camera, offset);

    shader -> use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    shader -> setMat4("projection", projection);
    shader -> setMat4("view", view);
    shader -> setMat4("model", model);

    float texutre_width = size.x;
    float texture_height = size.y;
    
    float vertices[] = {
        position.x - texutre_width /2.f, position.y + texture_height/2.f, 0.0f, 1.0f, 1.0f, // top right
        position.x + texutre_width / 2.f, position.y + texture_height /2.f, 0.0f, 1.0f, 0.0f, // bottom right
        position.x + texutre_width / 2.f , position.y - texture_height/2.f, 0.0f, 0.0f, 0.0f, // bottom left
        position.x - texutre_width / 2.f, position.y - texture_height/2.f, 0.0f, 0.0f, 1.0f // top left
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    
    
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    if(Animating && status == -1) {
        glBindTexture(GL_TEXTURE_2D, textureHovered);
    } else if(status == -1) {
        glBindTexture(GL_TEXTURE_2D, texture);
    }else if(status == 2) {
        glBindTexture(GL_TEXTURE_2D, textureDel);
    }else if(status == 1) {
        
        glBindTexture(GL_TEXTURE_2D, textureNew);
    }else if(status == 0) {
        glBindTexture(GL_TEXTURE_2D, textureRelated);
    }else if(status == 3) {
        glBindTexture(GL_TEXTURE_2D, textureFound);
    }
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if(left != nullptr) {
        float lineVertices[] = {
            position.x, position.y, 0.0f,
            left->position.x, left -> position.y, 0.0f
        };

        lineShader -> use();

        unsigned int lineVBO;
        unsigned int lineVAO;

        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineVBO);
        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
        glLineWidth(3.0f);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        if(left -> status == 3 && status == 3) {                   // edge change
            lineShader -> setVec4("lineColor", glm::vec4(255.f/ 255.f, 249.f/255.f, 72.f/255.f, 1.0f));
        }else lineShader -> setVec4("lineColor", glm::vec4(142.f/ 255.f, 162.f/255.f, 254.f/255.f, 1.0f));
        
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }



    if(right != nullptr) {
        float lineVertices[] = {
            position.x, position.y, 0.0f,
            right->position.x, right -> position.y, 0.0f
        };

        lineShader -> use();

        unsigned int lineVBO;
        unsigned int lineVAO;

        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineVBO);
        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
        glLineWidth(3.0f);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        if(right -> status == 3 && status == 3) {                   // edge change
            lineShader -> setVec4("lineColor", glm::vec4(255.f/ 255.f, 249.f/255.f, 72.f/255.f, 1.0f));
        }else  lineShader -> setVec4("lineColor", glm::vec4(142.f/ 255.f, 162.f/255.f, 254.f/255.f, 1.0f));
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

bool Node::isInside(float mouseX, float mouseY) {
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

    if (mouseX >= x -width && mouseX <= x + width /2 && mouseY >= y - height && mouseY <= y + height/4.f) {
        return true;
    }
    return false;
}