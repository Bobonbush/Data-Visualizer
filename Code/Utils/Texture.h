#ifndef TEXTURE_H
#define TEXTURE_H
#define STBN_IMAGE_IMPLEMENTATION

#include <GLFW/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ft2build.h"
#include "Camera.h"
#include FT_FREETYPE_H
#include <map>
#include "Utils/shader.h"


class TextureLoader{
    
    private:
    static unsigned int LoadTexturePNG(char* path);
    static unsigned int LoadTextureJPG(char* path);
    static std::string prefix[2];
    


    public:
    TextureLoader();            // Init everything for texture}

    ~TextureLoader();


    static unsigned int LoadTexture(char* path);
    static std::pair<int ,int> GetTextureSize(char* path);

    static void buildCircle(float radius, int vCount, std::vector<glm::vec3> &vertices, std::vector<unsigned int> &indices);
    static GLFWcursor* createCustomCursor(const char* imagePath);
};

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph

};

class TextHandler {
    private : 
    std::map<char, Character> Characters;
    unsigned int VAO, VBO;
    
    public:
    TextHandler();
    ~TextHandler();
    void RenderText(Shader* s, std::string text, float x, float y, float scale, glm::vec3 color , Camera * camera, float offset);
    void LoadFont(char* fontPath, unsigned int fontSize);
    void RenderMiddleText(Shader* s, std::string text, float x, float y, float scale, glm::vec3 color , Camera * camera, float offset);
    void RenderTextInRangeX(Shader* s, std::string text, float x, float y, float scale, glm::vec3 color , Camera * camera, float range, float offset , int cursor);
};

#endif