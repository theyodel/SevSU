#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

struct scientist {
    int id;
    char name[76];
    char area[26];
    char degree[26];
    int articles;
    int quotes;
    int hirshIndex;
};

struct tree {
    struct scientist info;
    struct tree *left, *right;
};

struct scientist readData();

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    struct tree *top;
    int choice;
    
    while (1) {
        system("cls");
        printf("=============== МЕНЮ ================\n");
        printf("1. Организация дерева\n");
        printf("2. Просмотр дерева (обход)\n");
        printf("3. Отображение структуры дерева\n");
        printf("4. Добавить узел дерева\n");
        printf("5. Исключить узел из дерева\n");
        printf("6. Сохранить дерево в файл\n");
        printf("7. Загрузить дерево из файла\n");
        printf("8. Определить уровень самого правого непустого дерева\n");
        printf("9. Вывести учёных с более чем 100 научных статей\n");
        printf("ESC. Выход\n");
        printf("-> ");
        choice = getch();

        switch (choice) {
        case '1':
            break;

        default:
            printf("Команда не распознана!\n");
            break;
        }
    }
}

struct scientist readData() {
    struct scientist data;
    static int nextID = 1;
    data.id = nextID++;
    fflush(stdin);
    printf("Введите ФИО: ");
    fgets(data.name, 75, stdin);
    data.name[strcspn(data.name, "\n")] = '\0';

    printf("Введите научную область: ");
    fgets(data.area, 25, stdin);
    data.area[strcspn(data.area, "\n")] = '\0';

    printf("Введите учёную степень: ");
    fgets(data.degree, 25, stdin);
    data.degree[strcspn(data.degree, "\n")] = '\0';
    
    printf("Введите количество научных статей: "); scanf("%d", &data.articles);
    printf("Введите количество цитирований: "); scanf("%d", &data.quotes);
    printf("Введите индекс Хирша: "); scanf("%d", &data.hirshIndex);
    printf("\n");
    fflush(stdin);
    return data;
}