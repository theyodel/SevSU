#include "tree.h"
#include "ui.h"
#include <windows.h>

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    
    struct tree *top = NULL;
    runProgram(&top);
    
    return 0;
}