#include "hashtable.h"

Block::Block(glm::vec3 _position, glm::vec3 _size, unsigned int _key, Camera * _camera) {
    key = _key;
    camera = _camera;
    position = _position;
    size = _size;
    shader = new Shader("Button.vs", "Button.fs");
    textShader = new Shader("textShader.vs", "textShader.fs");
    float width = camera -> width;
    float height = camera -> height;

    float texture_width = TextureLoader::GetTextureSize("box.png").first;
    float texture_height = TextureLoader::GetTextureSize("box.png").second;

    size.x = size.x * texture_width;
    size.y = size.y * texture_height;

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

    shader -> use();

    shader -> setMat4("model", model);
    shader -> setMat4("view", view);
    shader -> setMat4("projection", projection);

    float texture_width = size.x;
    float texture_height = size.y;

    float vertices[] = {
        position.x - texture_width / 2.f , position.y + texture_height / 2.f, 0.0f, 1.0f, 1.0f,
        position.x + texture_width / 2.f, position.y + texture_height / 2.f, 0.0f, 1.0f, 0.0f,
        position.x + texture_width / 2.f, position.y - texture_height / 2.f, 0.0f, 0.0f, 0.0f,
        position.x - texture_width / 2.f, position.y - texture_height / 2.f, 0.0f, 0.0f, 1.0f
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);


    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    float offset = 0.005f;
    text -> RenderText(textShader, std::to_string(key), position.x - texture_width / 2.f, position.y - texture_height / 2.f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), camera, offset);
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
    capacity = 30;
    blocks.resize(capacity);
    glm::vec3 position = glm::vec3(-0.75f, -0.75f, 0.0f);
    for(int i = 0 ; i < capacity ; i++) {
        HashTable::blocks[i] = new LinkedBlock(new Block(position, glm::vec3(0.3f, 0.1f, 0.0f), i, camera));
    }
}

HashTable::~HashTable() {
    for(int i = 0 ; i < capacity ; i++) {
        delete blocks[i];
    }
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
    for(int i = 0 ; i < capacity ; i++) {
        blocks[i] -> getBlock() -> Draw();
        LinkedBlock * current = blocks[i] ->getNext() ;
        while(current != nullptr) {
            current -> getBlock() -> Draw();
            current = current -> getNext();
        }
        
        
    }
}