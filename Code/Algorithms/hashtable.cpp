#include "hashtable.h"

Block::Block(glm::vec3 _position, glm::vec3 _size, int _key, Camera * _camera, char * path, bool isNull) {
    key = _key;
    camera = _camera;
    position = _position;
    targetPosition = _position;
    size = _size;
    shader = new Shader("Button.vs", "Button.fs");
    textShader = new Shader("textShader.vs", "textShader.fs");
    float width = camera -> width;
    float height = camera -> height;

    float texture_width;
    float texture_height;
    nullTexture = isNull;
    if(nullTexture) {
        nullTexture = true;
    }else 
    {
        texture_width = TextureLoader::GetTextureSize(path).first;
        texture_height = TextureLoader::GetTextureSize(path).second;

        texture = TextureLoader::LoadTexture(path);
    }
    if(nullTexture == false) {

        texture_Found = TextureLoader::LoadTexture("box_found.png");
        texture_New = TextureLoader::LoadTexture("box_new.png");
        texture_Delete = TextureLoader::LoadTexture("box_red.png");
        texture_Related = TextureLoader::LoadTexture("box_traverse.png");

        texture_width /= camera -> width;
        texture_height /= camera -> height;

        size.x = size.x * texture_width;
        size.y = size.y * texture_height;
    }
    

    text = new TextHandler();
    text -> LoadFont("Font/Roboto-Black.ttf", 24);

    float vertices[] = {
        position.x - size.x/2.f, position.y + size.y/2.f, 0.0f, 1.0f, 1.0f,
        position.x + size.x / 2.f , position.y + size.y /2.f, 0.0f, 1.0f, 0.0f,
        position.x + size.x / 2.f, position.y - size.y / 2.f, 0.0f, 0.0f, 0.0f,
        position.x - size.x / 2.f, position.y - size.y / 2.f, 0.0f, 0.0f, 1.0f
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Block::~Block() {
    delete shader;
}

unsigned int Block::getKey() {
    return key;
}

void Block::setKey(unsigned int _key) {
    key = _key;
}

void Block::Update(float deltaTime) {
    float speed = 3.f;
    float offset_x = targetPosition.x - position.x;
    float offset_y = targetPosition.y - position.y;

    if(offset_x < 0.01f && offset_x > -0.01f) {
        offset_x = 0.f;
    }
    if(offset_y < 0.01f && offset_y > -0.01f) {
        offset_y = 0.f;
    }

    position.x += offset_x * speed * deltaTime;
    position.y += offset_y * speed * deltaTime;
}

glm::vec3 Block::getSize() {
    return size;
}

void Block::Draw() {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);



    textShader -> use();
    textShader -> setMat4("projection", projection);
    float texture_width = size.x;
    float texture_height = size.y;
    float offset = 0.0005f;
    if(key != INT_MAX) {
        text -> RenderMiddleText(textShader, std::to_string(key), position.x, position.y, 1.5f, glm::vec3(0.1f, 0.0f, 1.f), camera, offset);
    }
    shader -> use();

    shader -> setMat4("model", model);
    shader -> setMat4("view", view);
    shader -> setMat4("projection", projection);

    if(nullTexture ) {
        return ;
    }

    

    float vertices[] = {
        position.x - texture_width / 2.f , position.y + texture_height / 2.f, 0.0f, 1.0f, 1.0f,
        position.x + texture_width / 2.f, position.y + texture_height / 2.f, 0.0f, 1.0f, 0.0f,
        position.x + texture_width / 2.f, position.y - texture_height / 2.f, 0.0f, 0.0f, 0.0f,
        position.x - texture_width / 2.f, position.y - texture_height / 2.f, 0.0f, 0.0f, 1.0f
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    if(status == -1) {
        glBindTexture(GL_TEXTURE_2D, texture);
    }else if(status == 0) {
        glBindTexture(GL_TEXTURE_2D, texture_Found);
    }else if(status == 1) {
        glBindTexture(GL_TEXTURE_2D, texture_Delete);
    }else if(status == 2) {
        glBindTexture(GL_TEXTURE_2D, texture_New);
    }else if( status == 3) {
        glBindTexture(GL_TEXTURE_2D, texture_Related);
    }
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

LinkedBlock::LinkedBlock(Block * _block) {
    block = _block;
    next = nullptr;
}

LinkedBlock::~LinkedBlock() {
    delete block;
}

Block * LinkedBlock::getBlock() {
    return block;
}

LinkedBlock * LinkedBlock::getNext() {
    return next;
}

void LinkedBlock::setNext(LinkedBlock * _next) {
    next = _next;
}

HashTable::HashTable(Camera * _camera) {
    camera = _camera;
    size = 0;
    lineShader = new Shader("line.vs", "line.fs");
    capacity = 22;
    blocks.resize(capacity);
    textShader  = new Shader("textShader.vs", "textShader.fs");
    text = new TextHandler();
    text -> LoadFont("Font/Roboto-Black.ttf", 48);
    glm::vec3 position = glm::vec3(-0.8f, -0.75f, 0.0f);
    for(int i = 0 ; i < capacity ; i++) {
        HashTable::blocks[i] = new LinkedBlock(new Block(position, glm::vec3(0.6f, 0.6f, 0.0f), i, camera, "box_red.png", false));
        position.x += 0.01f + blocks[i] -> getBlock() -> getSize().x;
    }
}

HashTable::~HashTable() {
    for(int i = 0 ; i < capacity ; i++) {
        delete blocks[i];
    }
    delete text;
    delete textShader;
}

int HashTable::HashFunction(int key) {
    HashText = "Hash(";
    HashText += std::to_string(key);
    HashText += ") = ";
    HashText += std::to_string(key);
    HashText += " % ";
    HashText += std::to_string(capacity);

    HashText += " = ";
    HashText += std::to_string(key % capacity);
    return key % capacity;
}

bool HashTable::insert(int key) {
    int index = HashFunction(key);
    LinkedBlock * current = blocks[index];
    float distance = 0.05f;
    int lmao = 0;
    while(current -> getNext() != nullptr) {
        if(current -> getBlock() -> traverse == false && lmao > 0) {
            current -> getBlock() -> status = 3;
            current -> getBlock() -> traverse = true;
            return false;
        }
        lmao = 1;
        distance += current -> getBlock() -> getSize().y /2.f + 0.1f  ;
        current = current -> getNext();
    }

    if(current -> getBlock() -> status == 2) {
        return true;
    }
    

    if(current -> getBlock() -> traverse == false && lmao > 0) {
        current -> getBlock() -> status = 3;
        current -> getBlock() -> traverse = true;
        return false;
    }

    
    distance += current -> getBlock() -> getSize().y/2.f + 0.1f;
    current -> setNext(new LinkedBlock(new Block(glm::vec3(blocks[index] -> block -> position.x, blocks[index] -> block -> position.y + distance, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), key, camera, "box.png", false)));
    current -> getNext() -> getBlock() -> status = 2;
    size++;
    return false;
}

void HashTable::Initialize(int key) {
    int index = HashFunction(key);
    LinkedBlock * current = blocks[index];
    float distance = 0.05f;
    while(current -> getNext() != nullptr) {
        distance += current -> getBlock() -> getSize().y /2.f + 0.1f  ;
        current = current -> getNext();
    }
    distance += current -> getBlock() -> getSize().y/2.f + 0.1f;
    current -> setNext(new LinkedBlock(new Block(glm::vec3(blocks[index] -> block -> position.x, blocks[index] -> block -> position.y + distance, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), key, camera, "box.png", false)));
    size++;
}


void HashTable::Recalculate() {
    for(int index = 0 ; index < capacity ; index++) {
        LinkedBlock * current = blocks[index];
        float distance = 0.f;
        while(current != nullptr) {
            current -> getBlock() -> targetPosition = glm::vec3(blocks[index] -> block -> position.x, blocks[index] -> block -> position.y + distance, 0.0f);
            distance += current -> getBlock() -> getSize().y /2.f + 0.1f  ;
            current = current -> getNext();
        }
    }
}

bool HashTable::Delete(int key) {
    int index = HashFunction(key);
    LinkedBlock * current = blocks[index] -> getNext();
    LinkedBlock * previous = blocks[index];
    while(current != nullptr) {
        if(current -> getBlock() -> traverse == false ) {
            current -> getBlock() -> status = 3;
            current -> getBlock() -> traverse = true;
            if(current -> getBlock() -> getKey() == key) {
                current -> getBlock() -> status = 1;
            }
            return false;
        }
        if(current -> getBlock() -> getKey() == key) {
            if(previous == nullptr) {
                blocks[index] = current -> getNext();
            } else {
                previous -> setNext(current -> getNext());
            }
            delete current;
            size--;
            return true;
        }
        previous = current;
        current = current -> getNext();
    }
    return true;
}

void HashTable::Update(float deltaTime) {
    for(int i = 0 ; i < capacity ; i++) {
        blocks[i] -> getBlock() -> Update(deltaTime);
        LinkedBlock * current = blocks[i] -> getNext();
        while(current != nullptr) {
            current -> getBlock() -> Update(deltaTime);
            current = current -> getNext();
        }
    }
}

void HashTable::Draw() {

    textShader -> use();
    textShader -> setMat4("projection", glm::mat4(1.0f));
    glm::vec3 textPosition = glm::vec3(0 , 0.75f , 0.0f);
    text -> RenderMiddleText(textShader, HashText, textPosition.x, textPosition.y, 1.0f, glm::vec3(1.f, 0.1f, 0.1f), camera, 0.0005f);
    for(int i = 0 ; i < capacity ; i++) {
        LinkedBlock * current = blocks[i] ;
        Block* previous = nullptr;
        while(current != nullptr) {
            current -> getBlock() -> Draw();
            if(previous ) {
                
                float vertices[] = {
                    previous -> position.x, previous -> position.y, 0.0f,
                    current -> getBlock() -> position.x, current -> getBlock() -> position.y, 0.0f
                };
    
                lineShader -> use();
                
                unsigned int LineVBO, LineVAO;
                glGenVertexArrays(1, &LineVAO);
                
                glGenBuffers(1, &LineVBO);
                glBindVertexArray(LineVAO);
                glBindBuffer(GL_ARRAY_BUFFER, LineVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
                glLineWidth(3.0f);
                lineShader -> setVec4("lineColor", glm::vec4(142.f/ 255.f, 162.f/255.f, 254.f/255.f, 1.0f));
            
                glDrawArrays(GL_LINES, 0, 2);
                
                glBindVertexArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glDeleteVertexArrays(1, &LineVAO);
                glDeleteBuffers(1, &LineVBO);

            }
            previous = current -> getBlock();
            current = current -> getNext();           
        }
    }
}

bool HashTable::Find(int key) {
    int index = HashFunction(key);
    LinkedBlock * current = blocks[index] -> getNext();
    while(current != nullptr) {
        if(current -> getBlock() -> traverse == false) {
            if(current -> getBlock() -> getKey() == key) {
                current -> getBlock() -> status = 0;
            }else {
                current -> getBlock() -> status = 3;
            }
            current -> getBlock() -> traverse = true;
            return false;
        }
        if(current -> getBlock() -> getKey() == key) {
            current -> getBlock() -> status = 0;
            return true;
        }
        current = current -> getNext();
    }
    return true;

}

void HashTable::Reset() {
    for(int index = 0 ; index < capacity ; index++) {
        LinkedBlock * current = blocks[index] -> getNext();
        while(current != nullptr) {
            current -> getBlock() -> status = -1;
            current->getBlock()->traverse = false;
            current = current -> getNext();
        }
    }
    HashText = "";
}