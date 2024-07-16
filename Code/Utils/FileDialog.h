#include <string>
#include <windows.h>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


class FileDialogs {
    public:
        static std::string OpenFile(const char* filter);
        static std::string SaveFile(const char* filter);

};