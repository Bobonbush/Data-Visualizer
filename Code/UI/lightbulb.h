#include "Tool/button.h"

class LightBulb : public Button {
    public:
        LightBulb( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera);
        ~LightBulb();
        
        int Update(float deltaTime, float x, float y);
        void Draw();
};