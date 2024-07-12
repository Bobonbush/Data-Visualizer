#include <GLFW/stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

class TextureLoader{
    
    private:
    unsigned int LoadTexturePNG(char* path);
    unsigned int LoadTextureJPG(char* path);
    std::string prefix[2];


    public:
    TextureLoader() {                // Init everything for texture
        prefix[0] = "png";
        prefix[1] = "jpg";
    }

    ~TextureLoader() {

    }


    unsigned int LoadTexture(char* path);


};