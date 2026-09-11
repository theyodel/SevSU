#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

#define ESC 27

// Очистка буфера ввода
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Вывод шапки таблицы
void printTableHeader() {
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
}

// Ввод данных одного учёного
struct scientist readData() {
    struct scientist data;
    static int nextID = 1;
    data.id = nextID++;
    
    printf("Введите ФИО: ");
    fgets(data.name, sizeof(data.name), stdin);
    data.name[strcspn(data.name, "\n")] = '\0';
    while (strlen(data.name) == 0) {
        printf("ФИО не может быть пустым. Введите ФИО: ");
        fgets(data.name, sizeof(data.name), stdin);
        data.name[strcspn(data.name, "\n")] = '\0';
    }

    printf("Введите научную область: ");
    fgets(data.area, sizeof(data.area), stdin);
    data.area[strcspn(data.area, "\n")] = '\0';

    printf("Введите учёную степень: ");
    fgets(data.degree, sizeof(data.degree), stdin);
    data.degree[strcspn(data.degree, "\n")] = '\0';
    
    printf("Введите количество научных статей: ");
    scanf("%d", &data.articles);
    printf("Введите количество цитирований: ");
    scanf("%d", &data.quotes);
    printf("Введите индекс Хирша: ");
    scanf("%d", &data.hirshIndex);
    clearInputBuffer();
    
    printf("\n");
    return data;
}

// Создание дерева через многократный ввод
struct tree *createTree() {
    struct tree *top = NULL;
    char ch;
    do {
        top = addNode(top, readData());
        system("cls");
        printf("\n\n1 - ввести ещё\n0 - закончить ввод\n-> ");
        ch = getch();
        printf("\n");
        if (ch != '1' && ch != '0') ch = '0';
    } while (ch == '1');
    return top;
}

// Главный цикл программы (меню, обработка команд)
void runProgram(struct tree **top) {
    int choice;
    char filename[256];
    FILE *f;
    int cnt, level, toDel;

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
        printf("8. Определить уровень самого правого узла\n");
        printf("9. Вывести учёных с более чем 100 статей\n");
        printf("ESC. Выход\n");
        printf("-> ");
        choice = getch();

        switch (choice) {
        case '1':
            system("cls");
            *top = createTree();
            break;
        case '2':
            system("cls");
            printTableHeader();
            viewTree(*top);
            printf("\nНажмите любую клавишу...");
            getch();
            break;
        case '3':
            system("cls");
            printf("\nСтруктура дерева (повёрнута влево):\n");
            printTreeStructure(*top, 0, 10);
            printf("\nНажмите любую клавишу...");
            getch();
            break;
        case '4':
            system("cls");
            *top = addNode(*top, readData());
            printf("Узел добавлен.\nНажмите любую клавишу...");
            getch();
            break;
        case '5':
            system("cls");
            printf("Введите количество статей удаляемого учёного: ");
            scanf("%d", &toDel);
            clearInputBuffer();
            *top = deleteNode(*top, toDel);
            printf("Нажмите любую клавишу...");
            getch();
            break;
        case '6':
            if (*top == NULL) {
                printf("Дерево пусто, сохранять нечего.\n");
                Sleep(1500);
                break;
            }
            printf("Введите имя файла для сохранения: ");
            fgets(filename, 256, stdin);
            filename[strcspn(filename, "\n")] = '\0';
            f = fopen(filename, "w");
            if (!f) {
                printf("Ошибка открытия файла для записи.\n");
                Sleep(1500);
                break;
            }
            saveToFile(*top, f);
            fclose(f);
            printf("Дерево сохранено в файл %s\n", filename);
            printf("Нажмите любую клавишу...");
            getch();
            break;
        case '7':
            system("cls");
            printf("Введите имя файла для загрузки: ");
            fgets(filename, 256, stdin);
            filename[strcspn(filename, "\n")] = '\0';
            f = fopen(filename, "r");
            if (!f) {
                printf("Ошибка открытия файла для чтения.\n");
                Sleep(1500);
                break;
            }
            struct tree *newnode = loadFromFile(f);
            fclose(f);
            if (newnode) {
                freeTree(*top);
                *top = newnode;
                printf("Дерево успешно загружено из файла %s\n", filename);
            } else {
                printf("Файл пуст или повреждён.\n");
            }
            printf("Нажмите любую клавишу...");
            getch();
            break;
        case '8':
            system("cls");
            if (*top == NULL) {
                printf("Дерево пусто.\n");
                Sleep(1500);
                break;
            }
            level = findRightmostLevel(*top);
            printf("Самый правый узел находится на уровне: %d\n", level);
            printf("Нажмите любую клавишу...");
            getch();
            break;
        case '9':
            system("cls");
            if (*top == NULL) {
                printf("Дерево пусто.\n");
                Sleep(1500);
                break;
            }
            cnt = 0;
            printf("\nУчёные с количеством статей > 100:\n");
            printTableHeader();
            printScientistsWithMoreThan100Articles(*top, &cnt);
            if (cnt == 0) {
                printf("Нет учёных с количеством статей более 100.\n");
            }
            printf("Нажмите любую клавишу...");
            getch();
            break;
        case ESC:
            freeTree(*top);
            return;
        default:
            printf("Команда не распознана!\n");
            Sleep(1500);
            break;
        }
    }
}