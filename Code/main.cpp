#define STB_IMAGE_IMPLEMENTATION
#include "Camera.h"
#include "Utils/Cursor.h"
#include "Tools/slideTools.h"
#include "Tools/ToolBar.h"


class Program {


private :
    GLFWwindow* window;   
    Shader * basicshader;
    Camera * camera;

    ToolBar * toolbar;

    SlideTools * slideTools;

public:
    Program()
    {
        window = nullptr;
        basicshader = nullptr;
        camera = nullptr;
        toolbar = nullptr;
        slideTools = nullptr;
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
        delete slideTools;
    }

    void Initialize()
    {
        // Initialize the library
        if (!glfwInit()) {
            return;
        }
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);  // Disable window resizing
        //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Disable window decorations (border/title bar)


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

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Error: GLAD not initialized" << std::endl;
            glfwTerminate();
            return;
        }
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       
        glEnable(GL_DEPTH_TEST);

        // Initialize UI
        Cursor::normalCursor = TextureLoader::createCustomCursor("cursor_none.png");
        Cursor::CurrentCursor = Cursor::normalCursor;
        Cursor::ReadyCursor = TextureLoader::createCustomCursor("hand_open.png");
        Cursor::HoldCursor = TextureLoader::createCustomCursor("hand_closed.png");

        toolbar = new ToolBar(camera);
        slideTools = new SlideTools(camera);

    }

    

    
    

    
    void Run() {
        
        
        Initialize();
        float currentFrame = 0;
        float lastFrame = 0;


        while (!glfwWindowShouldClose(window)) {
            
            Cursor::SetCursor(window);
            currentFrame = glfwGetTime();
            float deltaTime = currentFrame - lastFrame;
            
            // Updating
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastFrame = currentFrame;
            
            toolbar -> Update(deltaTime , xpos, ypos);
            slideTools -> Update(toolbar -> Algo, deltaTime, xpos, ypos);
            

            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glClearColor(14.f/255.f, 41.f / 255.f, 84.f/ 255.f, 1.0f);

            // Drawing 
            toolbar -> Draw();
            slideTools -> Draw();

            
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