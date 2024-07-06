#include "shader.h"



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



    void Run() {

        Initialize();
        
        
        float vertices[] = {
            0.5f,  -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
            0.0f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f,
        };
        

        basicshader = new Shader("vertexshader.vs", "fragmentshader.fs");
        
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

            
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);              // send the vertices to the GPU

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);          // 3 * sizeof(float) is the stride, 0 is the offset
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // 3 * sizeof(float) is the stride, 3 * sizeof(float) is the offset
        glEnableVertexAttribArray(1);


        glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the buffer

        glBindVertexArray(0); // unbind the VAO
        
        



        while (!glfwWindowShouldClose(window)) {
            
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            basicshader -> use();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);



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