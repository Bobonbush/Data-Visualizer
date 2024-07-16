#include "Cursor.h"

GLFWcursor * Cursor::normalCursor = nullptr;
GLFWcursor * Cursor::ReadyCursor = nullptr;
GLFWcursor * Cursor::HoldCursor = nullptr;
GLFWcursor * Cursor::CurrentCursor =  nullptr;


void Cursor::SetCursor(GLFWwindow* window) {

    glfwSetCursor(window, Cursor::CurrentCursor);
}