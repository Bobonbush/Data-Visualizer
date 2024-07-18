#ifndef TOOLS_H
#define TOOLS_H
#include "UpdateTool.h"
#include "Search.h"
#include "delete.h"
#include "NewTool.h"
#include "algorithmTool.h"
#include "data.h"
#include <vector>
#include "UI/Textbox.h"
class Tools {
    private :
        std::vector<Button*> button;
        int status = 0;
        TextBox* textBox;
        
    
    public :
        Tools(Camera * camera);
        ~Tools();
        void Update(float deltaTime, float x, float y);
        void Draw();
};

#endif