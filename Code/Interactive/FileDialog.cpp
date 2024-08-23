#include "FileDialog.h"


std::string FileDiaLog::OpenFile() {
    const char* lFilterPatterns[2] = { "*.txt", "*.inp"};
    const char* lTheOpenFileName = tinyfd_openFileDialog(
        "Choose a file",
        "",
        2,
        lFilterPatterns,
        NULL,
        0);
    
    return std::string(lTheOpenFileName);
}