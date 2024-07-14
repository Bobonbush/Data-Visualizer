#ifndef DELETE_H
#define DELETE_H
#include "button.h"


class Delete : public Button {
    public :
        Delete( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera);
        ~Delete();
        void Update(float deltaTime, float x, float y);
        void Draw();
};

#endif