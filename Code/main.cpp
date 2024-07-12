#define STB_IMAGE_IMPLEMENTATION
#include "Camera.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Program {


private :
    GLFWwindow* window;   
    Shader * basicshader;
    TextureLoader * texture;
    Camera * camera;

public:
    Program()
    {
        window = nullptr;
        basicshader = nullptr;
        texture = nullptr;
        camera = nullptr;

    }

    ~Program()
    {
        if (window != nullptr)
        {
            glfwDestroyWindow(window);
        }

        delete basicshader;
        delete texture;
        delete camera;
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

        camera = new Camera();

        window = glfwCreateWindow(camera -> width, camera ->height, "DATA VISUALIZER", NULL, NULL);
        if (!window) {
            glfwTerminate();
            return;
        }

        GLFWimage image[1];
        image[0].pixels = stbi_load("icon.png", &image[0].width, &image[0].height, 0, 4); //rgba channels
        glfwSetWindowIcon(window, 1, image);
        stbi_image_free(image[0].pixels);
           


        
        

        
        
        
        glfwMakeContextCurrent(window);

        if(glewInit() != GLEW_OK)
        {
            std::cout << "Error!" << std::endl;
            return ;
        }
        glEnable(GL_MULTISAMPLE);

        camera -> DisplayViewPort();
        glEnable(GL_DEPTH_TEST);
    }

    

    


    
    void Run() {

        Initialize();
        
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        float currentFrame = 0;
        float lastFrame = 0;
        while (!glfwWindowShouldClose(window)) {
            currentFrame = glfwGetTime();
            float deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }


    
};

int main() {
    
    Program *program = new Program();
    
    program->Run();

    delete program;
    return 0;
}