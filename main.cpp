#define STB_IMAGE_IMPLEMENTATION
#include "shader.h"


#include <GLFW/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Program {


private :
    int width = 1920;
    int height = 1080;     
    GLFWwindow* window;   
    Shader * basicshader;
public:
    Program()
    {
        window = nullptr;
        basicshader = nullptr;
    }

    ~Program()
    {
        if (window != nullptr)
        {
            glfwDestroyWindow(window);
        }

        delete basicshader;
    }

    void Initialize()
    {
        if (!glfwInit()) {
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        
       
        glfwWindowHint(GLFW_SAMPLES, 8 );  // defined samples for  GLFW Window


        window = glfwCreateWindow(width, height, "DATA VISUALIZER", NULL, NULL);
        if (!window) {
            glfwTerminate();
            return;
        }

        
        
        
        glfwMakeContextCurrent(window);

        if(glewInit() != GLEW_OK)
        {
            std::cout << "Error!" << std::endl;
            return ;
        }
        glEnable(GL_MULTISAMPLE);
        
        glViewport(0, 0, width, height);
        
    }

    unsigned int LoadTexturePNG(char* path){
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
        return texture;
    }

    unsigned int LoadTextureJPG(char* path)
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
        
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


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



    void Run() {

        Initialize();
        
        
        float vertices[] = {
            // positions          // colors           // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
        };

        unsigned int indices[] = {  
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        basicshader = new Shader("vertexshader.vs", "fragmentshader.fs");
        
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        unsigned int buffer;
        unsigned int VBO;
        glGenBuffers(1, &buffer);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);

            
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);              // send the vertices to the GPU

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);          // 3 * sizeof(float) is the stride, 0 is the offset
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // 3 * sizeof(float) is the stride, 3 * sizeof(float) is the offset
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // 3 * sizeof(float) is the stride, 6 * sizeof(float) is the offset
        glEnableVertexAttribArray(2);


        

        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the buffer

        glBindVertexArray(0); // unbind the VAO

        glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
        glm::mat4 trans = glm::mat4(1.0f);
        //trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
        vec = trans * vec;
        std::cout << vec.x << vec.y << vec.z << std::endl;

        
        
        
        
        //tran = glm::scale(tran, glm::vec3(0.5, 0.5, 0.5));
        //trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));


        while (!glfwWindowShouldClose(window)) {
            
            // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            basicshader -> use();

            

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, LoadTextureJPG("container.jpg"));

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, LoadTexturePNG("awesomeface.png"));
            basicshader -> setInt("ourTexture", 0);
            basicshader -> setInt("ourTexture2", 1);
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        
            trans = glm::rotate(trans, (float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            
            unsigned int transformLoc = glGetUniformLocation(basicshader -> ID, "transform");
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            trans = glm::mat4(1.0f);
            trans = glm::translate(trans, glm::vec3(-0.5f, -0.5f, 0.0f));
            trans = glm::rotate(trans, (float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);




            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &buffer);           // delete the buffer in the GPU
        glfwTerminate();
    }
};

int main() {
    
    Program *program = new Program();
    
    program->Run();

    delete program;
    return 0;
}