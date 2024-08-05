#include "Texture.h"


TextureLoader::TextureLoader(){
}

TextureLoader::~TextureLoader(){
}

std::string TextureLoader::prefix[2] = {"png", "jpg"}; // Add more file formats here

unsigned int TextureLoader :: LoadTexturePNG(char* path){
        //stbi_set_flip_vertically_on_load(true);
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        //glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        
        // set texture filtering parameters
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data)
        {
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
        return texture;
}

std::pair<int ,int> TextureLoader::GetTextureSize(char* path){
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    return std::make_pair(width, height);
}


unsigned int TextureLoader::LoadTextureJPG(char* path)
    {
        int width, height, nrChannels;
        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
        
        unsigned int texture;
        glGenTextures(1, &texture);

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D ,texture);
         
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        
        float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        
        
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


        if(!data)
        {
            std::cout << "Failed to load texture" << std::endl;
            return -1;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        return texture;



}

unsigned int TextureLoader::LoadTexture(char* path){
        std::string pathstr = path;
        std::string ext = pathstr.substr(pathstr.find_last_of(".") + 1);
        if(ext == prefix[0]){
            return LoadTexturePNG(path);
        }
        else if(ext == prefix[1]){
            return LoadTextureJPG(path);
        }
        else{
            std::cout << "Invalid file format" << std::endl;
            return 0;
        }
}

void TextureLoader::buildCircle(float radius, int vCount , std::vector<glm::vec3>& vertices, std::vector<unsigned int> &indices)
{
    float angle = 360.0f / vCount;

    int triangleCount = vCount - 2;

    std::vector<glm::vec3> temp;
    // positions
    for (int i = 0; i < vCount; i++)
    {
        float currentAngle = angle * i;
        float x = radius * cos(glm::radians(currentAngle));
        float y = radius * sin(glm::radians(currentAngle));
        float z = 0.0f;

        vertices.push_back(glm::vec3(x, y, z));
    }

    // push indexes of each triangle points
    for (int i = 0; i < triangleCount; i++)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }
}



GLFWcursor* TextureLoader::createCustomCursor(const char* imagePath) {
    stbi_set_flip_vertically_on_load(false); // Flip the image vertically
    
    int width, height, channels;
    unsigned char* data = stbi_load(imagePath, &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load cursor image: " << imagePath << std::endl;
        return nullptr;
    }

    GLFWimage cursorImage;
    cursorImage.width = width;
    cursorImage.height = height;
    cursorImage.pixels = data;

    GLFWcursor* cursor = glfwCreateCursor(&cursorImage, 0, 0);
    stbi_image_free(data);

    return cursor;
}



// TextHandler


TextHandler::TextHandler(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextHandler::~TextHandler(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


void TextHandler::RenderText(Shader* s, std::string text, float x, float y, float scale, glm::vec3 color, Camera * camera, float offset){
    s -> use();
    glUniform3f(glGetUniformLocation(s-> ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    std::string::const_iterator p;
    float maximum = x;
    float maximum_y = y;
    for(p = text.begin() ; p != text.end(); p++) {
        Character ch = Characters[*p];
        maximum += ((ch.Advance >> 6) * scale ) / camera -> width + offset;
        float h = ch.Size.y * scale;
        h = h / camera -> height;
        if(y + h > maximum_y){
            maximum_y = y +  h;
        }
    }
    if(maximum_y > 0.95f)
    {
        y -= (maximum_y - 0.95f);
    }
    if(maximum > 0.95f)
    {
        x -= (maximum - 0.95f);
    }
    std::string::const_iterator c;
    for(c = text.begin(); c != text.end(); c++){
        Character ch = Characters[*c];
        float xpos = x + (ch.Bearing.x * scale)/ camera -> width;
        float ypos = y - ((ch.Size.y - ch.Bearing.y) * scale)/ camera -> height +offset;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        w = w / camera -> width;
        h = h / camera -> height;
        
        float vertices[6][4] = {
            {xpos, ypos + h, 0.0, 0.0},        // Top left
            {xpos, ypos, 0.0, 1.0},            // Bottom left
            {xpos + w, ypos, 1.0, 1.0},        // Bottom right

            {xpos, ypos + h, 0.0, 0.0},        // Top left
            {xpos + w, ypos, 1.0, 1.0},        // Bottom right
            {xpos + w, ypos + h, 1.0, 0.0}      // Top right
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += ((ch.Advance >> 6) * scale)/ camera -> width + offset;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void TextHandler::RenderMiddleText(Shader* s, std::string text, float x, float y, float scale, glm::vec3 color, Camera * camera, float offset){
    s -> use();
    glUniform3f(glGetUniformLocation(s-> ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    std::string::const_iterator p;
    float maximum = x;
    float maximum_y = y;
    for(p = text.begin() ; p != text.end(); p++) {
        Character ch = Characters[*p];
        maximum += ((ch.Advance >> 6) * scale ) / camera -> width + offset;
        float h = ch.Size.y * scale;
        h = h / camera -> height;
        if(y + h > maximum_y){
            maximum_y = y +  h;
        }
    }
    if(maximum_y > 0.95f)
    {
        y -= (maximum_y - 0.95f);
    }
    if(maximum > 0.95f)
    {
        x -= (maximum - 0.95f);
    }

    x -= (maximum -x ) / 2;
    y -= (maximum_y -y ) /2;

    
    std::string::const_iterator c;
    for(c = text.begin(); c != text.end(); c++){
        Character ch = Characters[*c];
        float xpos = x + (ch.Bearing.x * scale)/ camera -> width;
        float ypos = y - ((ch.Size.y - ch.Bearing.y) * scale)/ camera -> height +offset;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        w = w / camera -> width;
        h = h / camera -> height;
        
        float vertices[6][4] = {
            {xpos, ypos + h, 0.0, 0.0},        // Top left
            {xpos, ypos, 0.0, 1.0},            // Bottom left
            {xpos + w, ypos, 1.0, 1.0},        // Bottom right

            {xpos, ypos + h, 0.0, 0.0},        // Top left
            {xpos + w, ypos, 1.0, 1.0},        // Bottom right
            {xpos + w, ypos + h, 1.0, 0.0}      // Top right
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += ((ch.Advance >> 6) * scale)/ camera -> width + offset;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}



void TextHandler::LoadFont(char* font , unsigned int Size) {
    FT_Library ft;
    if(FT_Init_FreeType(&ft)){
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    FT_Face face;
    if(FT_New_Face(ft, font, 0, &face)){
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, Size);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(unsigned char c = 0; c < 128; c++){
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
            std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        Characters.insert(std::pair<char, Character>(c, character));   
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}


void TextHandler::RenderTextInRangeX(Shader* s, std::string text, float x, float y, float scale, glm::vec3 color, Camera * camera, float range, float offset, int cursor){
    s -> use();
    glUniform3f(glGetUniformLocation(s-> ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    std::string::const_iterator p;
    float maximum = x;
    float maximum_y = y;
    for(p = text.begin() ; p != text.end(); p++) {
        Character ch = Characters[*p];
        maximum += ((ch.Advance >> 6) * scale ) / camera -> width + offset;
        float h = ch.Size.y * scale;
        h = h / camera -> height;
        if(y + h > maximum_y){
            maximum_y = y +  h;
        }
    }
    if(maximum_y > 0.95f)
    {
        y -= (maximum_y - 0.95f);
    }
    if(maximum > 0.95f)
    {
        x -= (maximum - 0.95f);
    }

    if(x > range) {
        x = range;
        return ;
    }
    std::string::const_iterator c;
    for(c = text.begin(); c != text.end(); c++){
        Character ch = Characters[*c];
        float xpos = x + (ch.Bearing.x * scale)/ camera -> width;
        float ypos = y - ((ch.Size.y - ch.Bearing.y) * scale)/ camera -> height +offset;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        w = w / camera -> width;
        h = h / camera -> height;
        
        float vertices[6][4] = {
            {xpos, ypos + h, 0.0, 0.0},        // Top left
            {xpos, ypos, 0.0, 1.0},            // Bottom left
            {xpos + w, ypos, 1.0, 1.0},        // Bottom right

            {xpos, ypos + h, 0.0, 0.0},        // Top left
            {xpos + w, ypos, 1.0, 1.0},        // Bottom right
            {xpos + w, ypos + h, 1.0, 0.0}      // Top right
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += ((ch.Advance >> 6) * scale)/ camera -> width + offset;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);   
}