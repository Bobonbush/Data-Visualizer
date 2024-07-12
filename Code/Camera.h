
#include "shader.h"

class Camera {
    private:
        float lastX ;
        float lastY ; 
        bool firstMouse;
        float fov;

        glm::vec3 cameraPos;
        glm::vec3 cameraFront;
        glm::vec3 cameraUp;

    public:

        float width;
        float height;

        Camera();            // Constructor
        ~Camera();           // Destructor
         
        void ProcessInput(GLFWwindow* window);

        void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

        void SetPointView(Shader * shader, std::string projection, std::string view);

        void DisplayViewPort();
        
        void DisplayViewPort(float x, float y, float width, float height);
};