#ifndef TOOLS_H
#define TOOLS_H
#include "UpdateTool.h"
#include "Search.h"
#include "delete.h"
#include "NewTool.h"
#include "algorithmTool.h"
#include "data.h"
#include <vector>
class Tools {
    private :
        std::vector<Button*> button;
    
    public :
        Tools(Camera * camera);
        ~Tools();
        void Update(float deltaTime, float x, float y);
        void Draw();
};

#endif