#ifndef UI_H
#define UI_H

#include "tree.h"

// Главная функция управления программой
void runProgram(struct tree **top);

// Вспомогательные функции интерфейса (используются в runProgram)
struct scientist readData();
void clearInputBuffer();
void printTableHeader();
struct tree *createTree();

#endif