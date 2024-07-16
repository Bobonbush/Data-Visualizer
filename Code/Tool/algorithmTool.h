#include "button.h"


class Algorithm : public Button {
    public :
        Algorithm( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera);
        ~Algorithm();
        int Update(float deltaTime, float x, float y);
        void Draw();
};