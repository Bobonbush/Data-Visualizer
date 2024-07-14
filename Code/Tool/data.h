#include "Tool/button.h"
#include "Algorithms/Texture.h"

class DataStructure : public Button {
    private :
        TextHandler* data;
        std::string text;
        std::string current;
        Shader * textShader;
        Camera * _camera;
    public:
        DataStructure( glm::vec3 _position , glm::vec3 _size , char * path , Camera * camera);
        ~DataStructure();
        void Update(float deltaTime);
        void Draw();
    
};