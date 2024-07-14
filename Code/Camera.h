#ifndef CAMERA_H
#define CAMERA_H
#include "Algorithms/shader.h"

class Camera {
    private:
        float lastX ;
        float lastY ; 
        bool firstMouse;
        

        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;

    public:
        static float fov;
        float width;
        float height;

        Camera();            // Constructor
        ~Camera();           // Destructor
         
        void ProcessInput(GLFWwindow* window);

        static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        void SetPointView(Shader * shader, std::string projection, std::string view);

        void DisplayViewPort();
        
        void DisplayViewPort(float x, float y, float width, float height);

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif