#ifndef UPDATE_TOOL_H
#define UPDATE_TOOL_H
#include "Button.h"


class UpdateTool : public Button
{
    private :
    

    public :
        UpdateTool(glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera);
        ~UpdateTool();

        void Update(float deltaTime);
        void Draw();



};

#endif