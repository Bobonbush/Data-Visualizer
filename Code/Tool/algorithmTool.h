#include "button.h"


class Algorithm : public Button {
    public :
        Algorithm( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera);
        ~Algorithm();
        void Update(float deltaTime);
        void Draw();
};