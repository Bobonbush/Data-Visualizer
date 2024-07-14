#include "Tool/button.h"

class LightBulb : public Button {
    public:
        LightBulb( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera);
        ~LightBulb();
        void Update(float deltaTime);
        void Draw();
};