#ifndef DARKMODE_H
#define DARKMODE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ColorMode {
    public:
    static int darkMode;

    static glm::vec4 GetbackgroundColor() {
        if(darkMode == 0) return glm::vec4(234.f/255.f, 253.0f/255.0f, 252.f / 255.f, 1.0f);
        else return glm::vec4(13.f/255.f, 45.0f/255.0f, 42.f / 255.f, 1.0f);
    }
};

int ColorMode::darkMode = 0;


#endif
