#ifndef TREE_H
#define TREE_H

#include <stdio.h>

// Структура данных учёного
struct scientist {
    int id;
    char name[76];
    char area[26];
    char degree[26];
    int articles;
    int quotes;
    int hirshIndex;
};

// Узел бинарного дерева (ключ – количество статей)
struct tree {
    struct scientist info;
    struct tree *left, *right;
};

// Функции для работы с деревом
struct tree *addNode(struct tree *top, struct scientist data);
struct tree *deleteNode(struct tree *node, int articles);
void viewTree(struct tree *top);
void printTreeStructure(struct tree *node, int space, int indent);
void saveToFile(struct tree *node, FILE *f);
struct tree *loadFromFile(FILE *f);
int findRightmostLevel(struct tree *node);
void printScientistsWithMoreThan100Articles(struct tree *node, int *count);
void freeTree(struct tree *node);

// Вспомогательная для удаления
struct tree *findMin(struct tree *node);

#endif