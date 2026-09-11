#include "tree.h"
#include <stdlib.h>
#include <string.h>

// Статическая функция вывода одного узла (используется внутри модуля)
static void printNode(struct tree *node) {
    printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
        node->info.id,
        node->info.name,
        node->info.degree,
        node->info.area,
        node->info.articles,
        node->info.quotes,
        node->info.hirshIndex);
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
}

// Добавление узла (по ключу articles)
struct tree *addNode(struct tree *top, struct scientist data) {
    if (!top) {
        top = (struct tree*)malloc(sizeof(struct tree));
        if (!top) {
            printf("Ошибка выделения памяти!\n");
            return NULL;
        }
        top->info = data;
        top->left = top->right = NULL;
    } else if (data.articles <= top->info.articles) {
        top->left = addNode(top->left, data);
    } else {
        top->right = addNode(top->right, data);
    }
    return top;
}

// Поиск минимального узла (для удаления)
struct tree *findMin(struct tree *node) {
    while (node && node->left) node = node->left;
    return node;
}

// Удаление узла по количеству статей
struct tree *deleteNode(struct tree *node, int articles) {
    if (node == NULL) {
        printf("Учёный с количеством статей %d не найден.\n", articles);
        return NULL;
    }
    if (articles < node->info.articles) {
        node->left = deleteNode(node->left, articles);
    } else if (articles > node->info.articles) {
        node->right = deleteNode(node->right, articles);
    } else {
        if (node->left == NULL) {
            struct tree *temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            struct tree *temp = node->left;
            free(node);
            return temp;
        }
        struct tree *minNode = findMin(node->right);
        node->info = minNode->info;
        node->right = deleteNode(node->right, minNode->info.articles);
    }
    return node;
}

// Симметричный обход (вывод в таблицу)
void viewTree(struct tree *top) {
    if (top) {
        viewTree(top->left);
        printNode(top);
        viewTree(top->right);
    }
}

// Вывод структуры дерева (повёрнуто налево)
void printTreeStructure(struct tree *node, int space, int indent) {
    if (node == NULL) return;
    space += indent;
    printTreeStructure(node->right, space, indent);
    printf("\n");
    for (int i = indent; i < space; i++) printf(" ");
    printf("%d (%s) [статей: %d]\n", node->info.id, node->info.name, node->info.articles);
    printTreeStructure(node->left, space, indent);
}

// Сохранение дерева в файл (pre‑order с маркерами 'D' и 'N')
void saveToFile(struct tree *node, FILE *f) {
    if (node == NULL) {
        fprintf(f, "N\n");
        return;
    }
    fprintf(f, "D\n");
    fprintf(f, "%d\n", node->info.id);
    fprintf(f, "%s\n", node->info.name);
    fprintf(f, "%s\n", node->info.area);
    fprintf(f, "%s\n", node->info.degree);
    fprintf(f, "%d\n", node->info.articles);
    fprintf(f, "%d\n", node->info.quotes);
    fprintf(f, "%d\n", node->info.hirshIndex);
    saveToFile(node->left, f);
    saveToFile(node->right, f);
}

// Загрузка дерева из файла
struct tree *loadFromFile(FILE *f) {
    char buf[10];
    if (!fgets(buf, sizeof(buf), f)) return NULL;
    if (buf[0] == 'N') {
        return NULL;
    } else if (buf[0] == 'D') {
        struct scientist data;
        if (fscanf(f, "%d\n", &data.id) != 1) return NULL;
        if (fgets(data.name, sizeof(data.name), f) == NULL) return NULL;
        data.name[strcspn(data.name, "\n")] = '\0';
        if (fgets(data.area, sizeof(data.area), f) == NULL) return NULL;
        data.area[strcspn(data.area, "\n")] = '\0';
        if (fgets(data.degree, sizeof(data.degree), f) == NULL) return NULL;
        data.degree[strcspn(data.degree, "\n")] = '\0';
        if (fscanf(f, "%d\n", &data.articles) != 1) return NULL;
        if (fscanf(f, "%d\n", &data.quotes) != 1) return NULL;
        if (fscanf(f, "%d\n", &data.hirshIndex) != 1) return NULL;
        struct tree *node = (struct tree*)malloc(sizeof(struct tree));
        if (!node) return NULL;
        node->info = data;
        node->left = loadFromFile(f);
        node->right = loadFromFile(f);
        return node;
    }
    return NULL;
}

// Уровень самого правого узла
int findRightmostLevel(struct tree *node) {
    int level = 1;
    while (node->right) {
        node = node->right;
        level++;
    }
    return level;
}

// Вывод учёных с >100 статей (обход и подсчёт)
void printScientistsWithMoreThan100Articles(struct tree *node, int *count) {
    if (node) {
        printScientistsWithMoreThan100Articles(node->left, count);
        if (node->info.articles > 100) {
            printNode(node);
            (*count)++;
        }
        printScientistsWithMoreThan100Articles(node->right, count);
    }
}

// Освобождение памяти дерева
void freeTree(struct tree *node) {
    if (node) {
        freeTree(node->left);
        freeTree(node->right);
        free(node);
    }
}