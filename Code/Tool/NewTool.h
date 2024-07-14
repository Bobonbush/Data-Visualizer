#ifndef _NEWTOOL_H
#define _NEWTOOL_H

#include "button.h"



class NewTool : public Button {
    public :
        NewTool( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera);
        ~NewTool();
        void Update(float deltaTime);
        void Draw();
};


#endif