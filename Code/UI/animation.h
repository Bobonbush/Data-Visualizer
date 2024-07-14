#include "Tool/button.h"

class Animation {
    private:
        Button * play;
        glm::vec3 position;
        glm::vec3 size;
        Camera * camera;
        float maximalTime = 1.f;
        float minimalTime = 0.f;

    public :
        Animation( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera);
        ~Animation();
        void Update(float deltaTime, float x, float y);
        void Draw();
};