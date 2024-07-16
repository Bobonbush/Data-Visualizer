#ifndef CURSOR_H
#define CURSOR_H
#include <Algorithms/Texture.h>
#include <iostream>
#include <string>


class Cursor {
    public:
    static GLFWcursor * normalCursor;
    static GLFWcursor * ReadyCursor;
    static GLFWcursor * HoldCursor;
    static GLFWcursor * CurrentCursor;
    
    static void SetCursor(GLFWwindow* window);
};

#endif