#ifndef SEARCH_H
#define SEARCH_H
#include "button.h"


class Search : public Button {
    public :
        Search( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera);
        ~Search();
        int Update(float deltaTime, float x, float y);
        void Draw();
};

#endif