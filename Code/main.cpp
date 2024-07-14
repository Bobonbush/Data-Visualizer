#define STB_IMAGE_IMPLEMENTATION
#include "Camera.h"
#include "Tool/Tool.h"
#include "UI/darkmode.h"
#include "UI/ui.h"


class Program {


private :
    GLFWwindow* window;   
    Shader * basicshader;
    Camera * camera;
    Tools * toolbar;
    UI * ui;
    
    

public:
    Program()
    {
        window = nullptr;
        basicshader = nullptr;
        camera = nullptr;
        toolbar = nullptr;
        ui = nullptr;
    }

    ~Program()
    {
        if (window != nullptr)
        {
            glfwDestroyWindow(window);
        }

        delete basicshader;
        delete camera;
        delete toolbar;
        delete ui;
    }

    void Initialize()
    {
        // Initialize the library
        if (!glfwInit()) {
            return;
        }
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);  // Disable window resizing
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Disable window decorations (border/title bar)


        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        
       
        glfwWindowHint(GLFW_SAMPLES, 64);  // defined samples for  GLFW Window

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
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

       
        glEnable(GL_DEPTH_TEST);
        ColorMode::darkMode = 0;               // Light Mode for default
        // Initialize UI
        toolbar = new Tools(camera);
        ui = new UI(camera);
    }

    

    
    

    
    void Run() {

        Initialize();
        float currentFrame = 0;
        float lastFrame = 0;

        glfwSetScrollCallback(window, Camera::ScrollCallback);
         glfwSetFramebufferSizeCallback(window, Camera::framebuffer_size_callback);
        while (!glfwWindowShouldClose(window)) {
            currentFrame = glfwGetTime();
            float deltaTime = currentFrame - lastFrame;
            
            // Updating
            toolbar -> Update(deltaTime);
            ui -> Update(deltaTime);
            
            lastFrame = currentFrame;
            glClearColor(234.f/255.f, 253.0f/255.0f, 252.f / 255.f, 1.0f);
            glClearColor(ColorMode::GetbackgroundColor().x, ColorMode::GetbackgroundColor().y, ColorMode::GetbackgroundColor().z, ColorMode::GetbackgroundColor().w);   
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Drawing 

            toolbar -> Draw();
            ui -> Draw();

            
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