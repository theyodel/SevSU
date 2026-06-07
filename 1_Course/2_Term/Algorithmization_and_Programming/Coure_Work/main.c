/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

            Севастопольский государственный университет
           Кафедра "Информационные технологии и системы"

          Программа для работы с базой сведений об учёных
                         Текст программы
                            РАЗРАБОТАЛ
                     Студент гр. ИИ/б-25-6-о
                           Заварзин А.В.
                               2026
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Программа работает с базой данных об учёных, которая считывается из
текстового файла. Каждая строка файла содержит запись об одном учёном, 
для которой указывается ФИО учёного, научная область, 
учёная степень, количество статей и цитирований, индекс Хирша.

    Основные функции программы:
- вывод базы на экран
- добавление записи об учёном в базу;
- исправление записи об учёном в базе;
- удаление записи об учёном из базы;
- поиск записи об учёном в базе;
- сортировка записей об учёных;

Вариант задания 4. Утверждено 18.02.2026
Среда программирования Visual Studio Code version 1.123.0
Дата последней коррекции: 4.06.2026.
Версия 0.6.64
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#define ESC (int)27
#define UP (int)72
#define DOWN (int)80
#define LEFT (int)75
#define RIGHT (int)77
#define ENTER (int)13
int nextID = 1;

//-----------------------------структуры данных------------------------------
struct scientist {           // Структура для хранения данных об учёном
    int id;                  // ID
    char name[76];           // ФИО
    char area[26];           // Научная область
    char degree[26];         // Учёная степень
    int articles;            // Количество статей
    int quotes;              // Количество цитирований
    int hirshIndex;          // Индекс Хирша
};

struct list {                // Элемент 2- направленного списка
    struct scientist info;   // Данные об учёном
    struct list *prev;       // Указатель на элемент слева (предыдущий)
    struct list *next;       // Указатель на элемент справа (следующий)
};

//-----------------------------прототипы функций-----------------------------

struct scientist readData();
struct list *createListFromKeyboard(struct list *);
struct list *addFirst(struct list *, const struct scientist);
struct list *addLast(struct list *, const struct scientist);
void viewList(struct list*);
int deleteNode(struct list **, int);
struct list *editElement(struct list *, int);
struct list *sortTable(struct list *, const int, const int);
void findScientist(struct list *, const int);
void exportToTxt(struct list *);
void exportToBin(struct list *);
struct list *importFromTxt(struct list *);
struct list *importFromBin(struct list *);
void top5ByArea(struct list *);

void printTableHeader();
void printNode(struct list *);
int charToInt(char);
int isNumber(char *);
void checkFileExt(char *, const char *);
int compareScientist(const struct scientist *, const struct scientist *, const int);
int captcha(const char *);

//------------------------------главная функция------------------------------
int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    srand(time(NULL));

    int mainChoice, secondChoice, flag, saved = 0;
    struct list *head = NULL;
    char buffer[25];

    const char *mainMenuItems[] = {
        "Организация списка",
        "Просмотр таблицы",
        "Добавление новой записи",
        "Удаление записи",
        "Корректировка записи",
        "Сортировка данных",
        "Поиск записи",
        "Сохранить таблицу в файл",
        "Чтение таблицы из файла",
        "Вывести по 5 учёных с наибольшим индексом Хирша и кол-вом цитирований для каждой области",
        "Выход"
    };
    int mainMenuSize = sizeof(mainMenuItems) / sizeof(mainMenuItems[0]);
    int currentMainItem = 0;

    while (1) {
        system("cls");
        printf("╔════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
        printf("║                                                МЕНЮ                                                ║\n");
        printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
        for (int i = 0; i < mainMenuSize; i++) {
            printf("  %s %s\n", (i == currentMainItem) ? "\033[35m▻" : " ", mainMenuItems[i]);
            printf("\033[0m");
        }
        printf("\nИспользуйте ↑ ↓ (стрелки вверх/вниз), Enter - выбор, ESC - выход.\n");

        int key = getch();
        if (key == 224) {
            key = getch();
            if (key == 72) currentMainItem = (currentMainItem - 1 + mainMenuSize) % mainMenuSize;
            else if (key == 80) currentMainItem = (currentMainItem + 1) % mainMenuSize;
            continue;
        } else if (key == ENTER) {
            mainChoice = currentMainItem + 1;
        } else if (key == ESC) {
            mainChoice = 11;
        } else {
            continue;
        }

        switch (mainChoice) {
            case 1:
                if (head && !saved) {
                    if (!captcha("создание нового списка (текущие данные будут потеряны)")) {
                        break;
                    }
                }
                head = createListFromKeyboard(head);
                saved = 0;
                break;

            case 2:
                if (head) viewList(head);
                else {
                    printf("Список пуст!\nРекомендуемое действие: 1.");
                    getch();
                }
                break;

            case 3:
                if (!head) {
                    printf("Список пуст!\nРекомендуемое действие: 1.\nНажмите любую клавишу...");
                    getch();
                    break;
                }
                const char *addMenu[] = { "Добавить в начало", "Добавить в конец", "Вернуться в меню" };
                int addSize = 3, addCur = 0;
                int addDone = 0;
                while (!addDone) {
                    system("cls");
                    printf("╔════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
                    printf("║                                          ДОБАВЛЕНИЕ ЗАПИСИ                                         ║\n");
                    printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
                    for (int i = 0; i < addSize; i++) {
                        printf("  %s %s\n", (i == addCur) ? "\033[35m▻" : " ", addMenu[i]);
                        printf("\033[0m");
                    }
                    printf("\n\U0001F817 Стрелки \x18 \x19, Enter, ESC - отмена.\n");
                    int k = getch();
                    if (k == 224) {
                        k = getch();
                        if (k == 72) addCur = (addCur - 1 + addSize) % addSize;
                        else if (k == 80) addCur = (addCur + 1) % addSize;
                    } else if (k == ENTER) {
                        if (addCur == 0) {
                            head = addFirst(head, readData());
                            addDone = 1;
                        } else if (addCur == 1) {
                            head = addLast(head, readData());
                            addDone = 1;
                        } else { // возврат
                            addDone = 1;
                        }
                    } else if (k == ESC) {
                        addDone = 1;
                    }
                }
                saved = 0;
                break;

            case 4:
                if (!head) {
                    printf("Список пуст!\nРекомендуемое действие: 1.");
                    getch();
                    break;
                }
                system("cls");
                flag = 1;
                while (flag) {
                    printf("Введите ID удаляемой записи или 0 (удалить все) -> ");
                    fgets(buffer, 25, stdin);
                    buffer[strcspn(buffer, "\n")] = '\0';
                    if (isNumber(buffer) >= 0) {
                        secondChoice = isNumber(buffer);
                        flag = 0;
                    } else {
                        printf("\nВведено некорректное число! Повторите ввод\n");
                    }
                }
                if (secondChoice == 0) {
                    if (!captcha("удаление ВСЕХ записей")) {
                        printf("Нажмите любую клавишу...");
                        getch();
                        break;
                    }
                }
                flag = deleteNode(&head, secondChoice);
                if (flag == -1) printf("Ошибка при удалении!\n");
                printf("Нажмите любую клавишу...");
                getch();
                break;

            case 5: // Корректировка записи
                if (!head) {
                    printf("Список пуст!");
                    getch();
                    break;
                }
                system("cls");
                flag = 1;
                while (flag) {
                    printf("Введите ID записи для корректировки -> ");
                    fgets(buffer, 25, stdin);
                    buffer[strcspn(buffer, "\n")] = '\0';

                    if (isNumber(buffer) >= 0) {
                        secondChoice = isNumber(buffer);
                        flag = 0;
                    } else {
                        printf("\nВведено некорректное число! Повторите ввод\n");
                    }
                }
                head = editElement(head, secondChoice);
                saved = 0;
                getch();
                break;

            case 6:
                if (!head || !head->next) {
                    system("cls");
                    printf("Список не нуждается в сортировке или пуст.\nНажмите любую клавишу...");
                    getch();
                    break;
                }
                const char *sortMenu[] = {
                    "По ID", "По ФИО", "По учёной степени", "По области наук",
                    "По числу статей", "По числу цитирований", "По индексу Хирша", "Вернуться в меню"
                };
                int sortSize = 8, sortCur = 0;
                int sortDone = 0;
                while (!sortDone) {
                    system("cls");
                    printf("╔════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
                    printf("║                                          СОРТИРОВКА ТАБЛИЦЫ                                        ║\n");
                    printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
                    for (int i = 0; i < sortSize; i++) {
                        printf("  %s %s\n", (i == sortCur) ? "\033[35m▻" : " ", sortMenu[i]);
                        printf("\033[0m");
                    }
                    printf("\n\U0001F817 Стрелки \x18 \x19, Enter, ESC - отмена.\n");
                    int k = getch();
                    if (k == 224) {
                        k = getch();
                        if (k == 72) sortCur = (sortCur - 1 + sortSize) % sortSize;
                        else if (k == 80) sortCur = (sortCur + 1) % sortSize;
                    } else if (k == ENTER) {
                        if (sortCur >= 0 && sortCur <= 6) {
                            char sortChar = '1' + sortCur;
                            const char *orderMenu[] = { "По возрастанию", "По убыванию", "Вернуться к выбору поля" };
                            int orderSize = 3, orderCur = 0;
                            int orderDone = 0;
                            while (!orderDone) {
                                system("cls");
                                printf("╔════════════════════════════════════════════════════════════════════════════════════╗\n");
                                printf("║                             ВЫБЕРИТЕ ПОРЯДОК СОРТИРОВКИ                             ║\n");
                                printf("╚════════════════════════════════════════════════════════════════════════════════════╝\n");
                                for (int i = 0; i < orderSize; i++) {
                                    printf("  %s %s\n", (i == orderCur) ? "\033[35m▻" : " ", orderMenu[i]);
                                    printf("\033[0m");
                                }
                                printf("\n\U0001F817 Стрелки \x18 \x19, Enter, ESC - отмена.\n");
                                int ch = getch();
                                if (ch == 224) {
                                    ch = getch();
                                    if (ch == 72) orderCur = (orderCur - 1 + orderSize) % orderSize;
                                    else if (ch == 80) orderCur = (orderCur + 1) % orderSize;
                                } else if (ch == ENTER) {
                                    if (orderCur == 0) {          // по возрастанию
                                        sortTable(head, sortChar, 0);
                                        printf("\nСортировка выполнена (по возрастанию). Нажмите любую клавишу...");
                                        getch();
                                        sortDone = 1;
                                        orderDone = 1;
                                    } else if (orderCur == 1) {   // по убыванию
                                        sortTable(head, sortChar, 1);
                                        printf("\nСортировка выполнена (по убыванию). Нажмите любую клавишу...");
                                        getch();
                                        sortDone = 1;
                                        orderDone = 1;
                                    } else {                      // вернуться к выбору поля
                                        orderDone = 1;
                                    }
                                } else if (ch == ESC) {
                                    orderDone = 1;   // отмена – возврат к выбору поля
                                }
                            }
                            if (sortDone == 1) break;  // если сортировка выполнена, выходим из внешнего цикла
                        } else { // sortCur == 7 -> возврат в главное меню
                            sortDone = 1;
                        }
                    } else if (k == ESC) {
                        sortDone = 1;
                    }
                }
                break;

            case 7:
                if (!head) {
                    printf("Таблица пуста!\n");
                    getch();
                    break;
                }
                const char *findMenu[] = {
                    "По ID", "По ФИО", "По учёной степени", "По области наук",
                    "По числу статей", "По числу цитирований", "По индексу Хирша", "Вернуться в меню"
                };
                int findSize = 8, findCur = 0;
                int findDone = 0;
                while (!findDone) {
                    system("cls");
                    printf("╔════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
                    printf("║                                            ПОИСК УЧЁНОГО                                           ║\n");
                    printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
                    for (int i = 0; i < findSize; i++) {
                        printf("  %s %s\n", (i == findCur) ? "\033[35m▻" : " ", findMenu[i]);
                        printf("\033[0m");
                    }
                    printf("\n\U0001F817 Стрелки \x18 \x19, Enter, ESC - отмена.\n");
                    int k = getch();
                    if (k == 224) {
                        k = getch();
                        if (k == 72) findCur = (findCur - 1 + findSize) % findSize;
                        else if (k == 80) findCur = (findCur + 1) % findSize;
                    } else if (k == ENTER) {
                        if (findCur >= 0 && findCur <= 6) {
                            char findChar = '1' + findCur;
                            findScientist(head, findChar);
                            printf("\nНажмите любую клавишу для продолжения...");
                            getch();
                            findDone = 1;
                        } else {
                            findDone = 1;
                        }
                    } else if (k == ESC) {
                        findDone = 1;
                    }
                }
                break;

            case 8:
                if (!head) {
                    printf("Список пуст!\n");
                    getch();
                    break;
                }
                const char *exportMenu[] = { "В текстовый файл (.txt)", "В бинарный файл (.bin)", "Вернуться в меню" };
                int expSize = 3, expCur = 0;
                int expDone = 0;
                while (!expDone) {
                    system("cls");
                    printf("╔════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
                    printf("║                                           ЭКСПОРТ ТАБЛИЦЫ                                          ║\n");
                    printf("╠════════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
                    for (int i = 0; i < expSize; i++) {
                        printf("  %s %s\n", (i == expCur) ? "\033[35m▻" : " ", exportMenu[i]);
                        printf("\033[0m");
                    }
                    printf("");
                    printf("\n, Enter - выбор, ESC - отмена.\n");
                    int k = getch();
                    if (k == 224) {
                        k = getch();
                        if (k == 72) expCur = (expCur - 1 + expSize) % expSize;
                        else if (k == 80) expCur = (expCur + 1) % expSize;
                    } else if (k == ENTER) {
                        if (expCur == 0) exportToTxt(head);
                        else if (expCur == 1) exportToBin(head);
                        expDone = 1;
                    } else if (k == ESC) {
                        expDone = 1;
                    }
                }
                saved = 1;
                
                break;

            case 9:
                if (head && saved == 0) {
                    // Уже есть несохранённые данные – требуем каптчу
                    if (!captcha("импорт таблицы с потерей несохранённых данных")) {
                        break;
                    }
                }
                const char *importMenu[] = { "Из текстового файла (.txt)", "Из бинарного файла (.bin)", "Вернуться в меню" };
                int impSize = 3, impCur = 0;
                int impDone = 0;
                while (!impDone) {
                    system("cls");
                    printf("╔════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
                    printf("║                                           ИМПОРТ ТАБЛИЦЫ                                           ║\n");
                    printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════╝\n");
                    for (int i = 0; i < impSize; i++) {
                        printf("  %s %s\n", (i == impCur) ? "\033[35m▻" : " ", importMenu[i]);
                        printf("\033[0m");
                    }
                    printf("\n\U0001F817 Стрелки \x18 \x19, Enter, ESC - отмена.\n");
                    int k = getch();
                    if (k == 224) {
                        k = getch();
                        if (k == 72) impCur = (impCur - 1 + impSize) % impSize;
                        else if (k == 80) impCur = (impCur + 1) % impSize;
                    } else if (k == ENTER) {
                        if (impCur == 0) head = importFromTxt(head);
                        else if (impCur == 1) head = importFromBin(head);
                        impDone = 1;
                    } else if (k == ESC) {
                        impDone = 1;
                    }
                }
                saved = 0;
                break;

            case 10:
                if (!head) {
                    printf("Список пуст!\n");
                    getch();
                    break;
                }
                top5ByArea(head);
                getch();
                break;

            case 11:
                if (head && !saved) {
                    if (!captcha("выход без сохранения")) {
                        break;
                    }
                }
                int cnt = deleteNode(&head, 0);
                printf("\nУдалено %d записей. Выход...\n", cnt);
                Sleep(3000);
                return 0;

            default:
                printf("\nОшибка: неизвестный пункт!");
                break;
        }
    }
}


void printTableHeader() {
    printf("╔══════╦═════════════════════════════════════════════════════════════════════════════╦═══════════════════════════╦═══════════════════════════╦═══════════════╦════════════════════╦══════════════╗\n");
    printf("║  ID  ║                             Фамилия Имя Отчество                            ║       Учёная Степень      ║        Область Науки      ║ Кол-во статей ║ Кол-во цитирований ║ Индекс Хирша ║\n");
    printf("╠══════╬═════════════════════════════════════════════════════════════════════════════╬═══════════════════════════╬═══════════════════════════╬═══════════════╬════════════════════╬══════════════╣\n");
}

void printNode(struct list *node) {
    printf("║ %-4d ║ %-75s ║ %-25s ║ %-25s ║ %-13d ║ %-18d ║ %-12d ║\n",
            node->info.id,
            node->info.name,
            node->info.degree,
            node->info.area,
            node->info.articles,
            node->info.quotes,
            node->info.hirshIndex);
    printf("╠══════╬═════════════════════════════════════════════════════════════════════════════╬═══════════════════════════╬═══════════════════════════╬═══════════════╬════════════════════╬══════════════╣\n");
}

void setNewID(struct list *head, const int _mode) {
    int maxID = -1;
    struct list *temp = head;
    if (_mode == 0) {
        nextID = 1;
    } else if (_mode == 1) {
        if (head == NULL) {
            nextID = 1;
            return;
        }
        for (; temp != NULL; temp = temp->next) {
            if (maxID < temp->info.id) nextID = temp->info.id + 1;
        }
    }
}

int charToInt(char ch) {
    switch (ch) {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        default: return -1;
    }
}

int isNumber(char *_str) {
    int itog = 0;
    for(int i = 0; i < strlen(_str); i++) {
        if ('0' <= _str[i] && _str[i] <= '9') itog += charToInt(_str[i]) * pow(10, strlen(_str) - 1 - i);
        else return -1;
    }
    return itog;
}

/// @brief Функция чтения данных с клавиатуры
/// @return Данные об учёном
struct scientist readData() {
    struct scientist data;
    char buffer[25];
    int flag = 1;
    
    data.id = nextID++;
    
    printf("Введите ФИО: ");
    fgets(data.name, 75, stdin);
    data.name[strcspn(data.name, "\n")] = '\0';
    
    printf("Введите научную область: ");
    fgets(data.area, 25, stdin);
    data.area[strcspn(data.area, "\n")] = '\0';
    
    printf("Введите учёную степень: ");
    fgets(data.degree, 25, stdin);
    data.degree[strcspn(data.degree, "\n")] = '\0';
    
    while (flag) {
        printf("Введите количество научных статей: ");
        fgets(buffer, 25, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (isNumber(buffer) >= 0) {
            data.articles = isNumber(buffer);
            flag = 0;
        } else {
            printf("\nВведено некорректное число! Повторите ввод\n");
        }
    }
    flag = 1;
    
    while (flag) {
        printf("Введите количество цитирований: ");
        fgets(buffer, 25, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (isNumber(buffer) >= 0) {
            data.quotes = isNumber(buffer);
            flag = 0;
        } else {
            printf("\nВведено некорректное число! Повторите ввод\n");
        }
    }
    flag = 1;
    
    while (flag) {
        printf("Введите индекс Хирша: ");
        fgets(buffer, 25, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (isNumber(buffer) >= 0) {
            data.hirshIndex = isNumber(buffer);
            flag = 0;
        } else {
            printf("\nВведено некорректное число! Повторите ввод\n");
        }
    }
    
    return data;
}

/// @brief Функция добавления в начало списка
/// @param head начало списка
/// @param data данные об учённом
/// @return Указатель на начало списка (указатель на добавленный в начало элемент)
struct list *addFirst(struct list *head, const struct scientist data) {
    struct list *temp;
    temp = (struct list*)malloc(sizeof(struct list));
    temp->info = data;
    temp->next = head;
    temp->prev = NULL;
    return temp;
}

/// @brief Функция добавления в конец списка
/// @param head начало списка
/// @param data данные об учённом
/// @return Указатель на начало списка
struct list *addLast(struct list *head, const struct scientist data) {
    struct list *temp, *e;
    temp = (struct list*)malloc(sizeof(struct list));
    temp->info = data;
    temp->next = NULL;

    if (head == NULL) head = temp;
    else {
        for (e = head; e->next != NULL; e = e->next);
        temp->prev = e;
        e->next = temp;
    }

    return head;
}

/// @brief Функция создания списка с клавиатуры
/// @param head указатель на начало списка
/// @return Указатель на начало списка
struct list *createListFromKeyboard(struct list *head) {
    int choice = 0;

    setNewID(head, 1);

    do {
        system("cls");
        if (head == NULL) head = addFirst(head, readData());
        else addLast(head, readData());

        printf("\nВвести ещё? \n1. да \n0. нет");
        choice = getch();
        if (choice == '0') choice = 0;
    } while (choice);

    return head;
}

/// @brief Функция вывода таблицы со скроллингом
/// @param head - Указатель на начало списка
void viewList(struct list *head) {
    int total = 0;
    struct list *cur = head;
    while (cur) {
        total++;
        cur = cur->next;
    }

    const int pageSize = 15;
    int start = 0;
    char key;

    while (1) {
        system("cls");

        printTableHeader();

        struct list *temp = head;
        int idx = 0;
        while (temp && idx < start) {
            temp = temp->next;
            idx++;
        }

        int printed = 0;
        for (; temp != NULL && printed < pageSize; temp = temp->next, printed++) printNode(temp);

        if (printed == 0) break;

        int first = start + 1;
        int last = (start + printed < total) ? start + printed : total;
        printf("\nЗаписи: %d-%d из %d. \nИспользуйте:\n← (предыдущая страница)\t → (следующая страница) \n↑ (предыдущая запись)\t ↓ (следующая запись) \nESC (выход)", first, last, total);
        printf("\n\n[ * ] Одна страница занимает %d записей", pageSize);

        key = getch();

        if (key == LEFT) {
            int new_start = start - pageSize;
            if (new_start < 0) new_start = 0;
            start = new_start;
        } else if (key == RIGHT) {
            int new_start = start + pageSize;
            int max_start = total - pageSize;
            if (max_start < 0) max_start = 0;
            if (new_start > max_start) new_start = max_start;
            start = new_start;
        } else if (key == UP) {
            int new_start = start - 1;
            if (new_start < 0) new_start = 0;
            start = new_start;
        } else if (key == DOWN) {
            int new_start = start + 1;
            int max_start = total - 1;
            if (max_start < 0) max_start = 0;
            if (new_start > max_start) new_start = max_start;
            start = new_start;
        } else if (key == ESC) {
            break;
        }
    };
}

/// @brief Проверяет и добавляет расширение файла, если его нет
void checkFileExt(char *filename, const char *ext) {
    int len = strlen(filename);
    if (len < 4 || strcmp(filename + len - 4, ext) != 0) {
        strcat(filename, ext);
    }
}

void writeToTextFile(FILE *file, struct list *node) {
    fprintf(file, "%d\t%s\t%s\t%s\t%d\t%d\t%d\n",
            node->info.id,
            node->info.name,
            node->info.degree,
            node->info.area,
            node->info.articles,
            node->info.quotes,
            node->info.hirshIndex
            );
}

/// @brief Функция экспорта таблицы в текстовый (`.txt`) файл
/// @param head Указатель на начало списка
void exportToTxt(struct list *head) {
    char fileName[101];
    struct list *temp = head;
    
    printf("\nВведите название файла -> ");
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    
    checkFileExt(fileName, ".txt");
    
    FILE *file = fopen(fileName, "wt");
    if (file == NULL) {
        printf("\033[31mОшибка создания файла '%s'!\n", fileName);
        printf("\033[0mНажмите любую клавишу для возвращения в меню...");
        getch();
        return;
    }
    
    for (; temp != NULL; temp = temp->next) {
        writeToTextFile(file, temp);
    }
    fclose(file);

    printf("\033[36mИнформация записана в файл '%s'\n", fileName);
    printf("\033[0mНажмите любую клавишу для возвращения в меню...");
    getch();
}

/// @brief Функция экспорта таблицы в `.bin` файл
/// @param head Указатель на начало списка
void exportToBin(struct list *head) {
    char fileName[101];
    printf("\n\nВведите название файла -> ");
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    
    checkFileExt(fileName, ".bin");
    
    FILE *file = fopen(fileName, "wb");
    if (file == NULL) {
        printf("\033[31mОшибка создания файла '%s'!\n", fileName);
        printf("\033[0mНажмите любую клавишу для возвращения в меню...");
        getch();
        return;
    }
    
    for (struct list *temp = head; temp != NULL; temp = temp->next) {
        fwrite(&temp->info, sizeof(struct scientist), 1, file);
    }
    fclose(file);

    printf("\033[36mТаблица успешно экспортирована в файл '%s'\n", fileName);
    printf("\033[0mНажмите любую клавишу для возвращения в меню...");
    getch();
}

/// @brief Функция импорта таблицы из текстового (`.txt`) файла
/// @param head Указатель на начало списка
/// @return Указатель на начало списка
struct list *importFromTxt(struct list *head) {
    char fileName[101];
    struct scientist data;
    system("cls");
    printf("================ Импорт из текстового файла ================\n");
    printf("Введите название файла -> ");
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    
    checkFileExt(fileName, ".txt");
    
    FILE *file = fopen(fileName, "rt");
    if (file == NULL) {
        printf("\n\n\033[31mФайл '%s' не найден!\n", fileName);
        printf("\033[0mНажмите любую клавишу для возвращения в меню...");
        getch();
        return head;
    }
    
    while (fscanf(file, "%d\t%[^\t]\t%[^\t]\t%[^\t]\t%d\t%d\t%d\n",
                  &data.id, data.name, data.degree, data.area,
                  &data.articles, &data.quotes, &data.hirshIndex) == 7) {
        if (head == NULL)
            head = addFirst(head, data);
        else
            head = addLast(head, data);
    }
    
    fclose(file);

    setNewID(head, 1);

    printf("\n\033[36mТаблица успешно импортирована из файла '%s'\n", fileName);
    printf("\033[0mНажмите любую клавишу для возвращения в меню...");
    getch();

    return head;
}

/// @brief Функция импорта таблицы из бинарного (`.bin`) файла
/// @param head Указатель на начало списка
/// @return Указатель на начало списка
struct list *importFromBin(struct list *head) {
    char fileName[101];
    struct scientist data;
    system("cls");
    printf("================ Импорт из бинарного файла ================\n");
    printf("Введите название файла -> ");
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    
    checkFileExt(fileName, ".bin");
    
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("\n\n\033[31mФайл '%s' не найден!\n", fileName);
        printf("\033[0mНажмите любую клавишу для возвращения в меню...");
        getch();
        return head;
    }
    
    while (fread(&data, sizeof(struct scientist), 1, file) == 1) {
        if (head == NULL) head = addFirst(head, data);
        else head = addLast(head, data);
    }
    
    fclose(file);

    setNewID(head, 1);

    printf("\n\033[36mТаблица успешно импортирована из файла '%s'\n", fileName);
    printf("\033[0mНажмите любую клавишу для возвращения в меню...");
    getch();

    return head;
}

/// @brief Функция очистки памяти
/// @param head Указатель на начало списка
/// @return Кол-во удалённых записей
int deleteNode(struct list **head, int choice) {
    if (head == NULL) return -1;

    struct list *temp = NULL;

    switch (choice) {
        case 0: {
            if (*head == NULL) {
                printf("\nСписок пуст...\n");
                return 0;
            }
            int count = 0;
            while (*head != NULL) {
                temp = *head;
                *head = (*head)->next;
                free(temp);
                count++;
            }
            *head = NULL;
            setNewID(*head, 0);
            return count;
        }

        case 1: {
            if (*head == NULL) {
                printf("\nСписок пуст, нечего удалять\n");
                return -1;
            }
            temp = *head;
            *head = (*head)->next;
            if (*head != NULL) (*head)->prev = NULL;
            free(temp);
            setNewID(*head, 1);
            return 1;
        }

        default: {
            struct list *toDel = *head;
            
            while (toDel != NULL && toDel->info.id != choice) toDel = toDel->next;
            
            if (toDel == NULL) {
                printf("\nЭлемент с ID '%d' не найден\n", choice);
                return -1;
            }
            
            if (toDel->prev != NULL) toDel->prev->next = toDel->next;
            else *head = toDel->next;
            
            if (toDel->next != NULL) toDel->next->prev = toDel->prev;
            
            free(toDel);
            return 1;
        }
    }
}

/// @brief Функция корректировки данных об учёном по его ID
/// @param head Указатель на начало списка
/// @param id ID записи, которая будет откорректирована
/// @return Указатель на начало списка
struct list *editElement(struct list *head, int id) {
    int choice = 0;           // 1..6 или 0 (меню)
    int oldInt, flag;
    char oldLine[76], buffer[25];
    struct list *temp = head;

    for (; temp != NULL && temp->info.id != id; temp = temp->next);

    if (temp == NULL) {
        printf("Запись с ID '%d' не найдена!", id);
        printf("\nНажмите любую клавишу для возврата в меню...");
        getch();
        return head;
    }

    const char *editItems[] = {
        "ФИО",
        "Учёная степень",
        "Область науки",
        "Кол-во статей",
        "Кол-во цитирований",
        "Индекс Хирша",
        "В меню"
    };
    int editSize = sizeof(editItems) / sizeof(editItems[0]);
    int currentEdit = 0;

    while (1) {
        system("cls");
        printf("Запись с ID '%d' найдена!\n\n", id);
        printf("Выберите поле, которое хотите изменить:\n");
        for (int i = 0; i < editSize; i++) {
            printf("  %s %s\n", (i == currentEdit) ? "\033[35m▻" : " ", editItems[i]);
            printf("\033[0m");
        }
        printf("\n\U0001F817 Используйте \x18 \x19 (стрелки вверх/вниз), Enter - выбор, ESC - отмена.\n");

        int key = getch();
        if (key == 224) {
            key = getch();
            if (key == UP)
                currentEdit = (currentEdit - 1 + editSize) % editSize;
            else if (key == DOWN)
                currentEdit = (currentEdit + 1) % editSize;
        } else if (key == ENTER) {
            if (currentEdit == editSize - 1) {
                choice = 0;
            } else {
                choice = currentEdit + 1;
            }
            break;
        } else if (key == ESC) {
            choice = 0;
            break;
        }
    }

    system("cls");
    switch (choice) {
        case 1:
            printf("Введите ФИО -> ");
            strcpy(oldLine, temp->info.name);
            fgets(temp->info.name, 75, stdin);
            temp->info.name[strcspn(temp->info.name, "\n")] = '\0';
            printf("ФИО успешно изменено!\n");
            printf("%s  ->  %s\n", oldLine, temp->info.name);
            break;

        case 2:
            printf("Введите учёную степень -> ");
            strcpy(oldLine, temp->info.degree);
            fgets(temp->info.degree, 25, stdin);
            temp->info.degree[strcspn(temp->info.degree, "\n")] = '\0';
            printf("Учёная степень успешно изменена!\n");
            printf("%s  ->  %s\n", oldLine, temp->info.degree);
            break;

        case 3:
            printf("Введите область наук -> ");
            strcpy(oldLine, temp->info.area);
            fgets(temp->info.area, 25, stdin);
            temp->info.area[strcspn(temp->info.area, "\n")] = '\0';
            printf("Область наук успешно изменена!\n");
            printf("%s  ->  %s\n", oldLine, temp->info.area);
            break;

        case 4:
            oldInt = temp->info.articles;
            flag = 1;
            while (flag) {
                printf("Введите кол-во статей -> ");
                fgets(buffer, 25, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                int val = isNumber(buffer);
                if (val >= 0) {
                    temp->info.articles = val;
                    flag = 0;
                } else {
                    printf("\nОшибка: введите целое неотрицательное число!\n");
                }
            }
            printf("Кол-во статей успешно изменено!\n");
            printf("%d  ->  %d\n", oldInt, temp->info.articles);
            break;

        case 5:
            oldInt = temp->info.quotes;
            flag = 1;
            while (flag) {
                printf("Введите кол-во цитирований -> ");
                fgets(buffer, 25, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                int val = isNumber(buffer);
                if (val >= 0) {
                    temp->info.quotes = val;
                    flag = 0;
                } else {
                    printf("\nОшибка: введите целое неотрицательное число!\n");
                }
            }
            printf("Кол-во цитирований успешно изменено!\n");
            printf("%d  ->  %d\n", oldInt, temp->info.quotes);
            break;

        case 6:
            oldInt = temp->info.hirshIndex;
            flag = 1;
            while (flag) {
                printf("Введите индекс Хирша -> ");
                fgets(buffer, 25, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                int val = isNumber(buffer);
                if (val >= 0) {
                    temp->info.hirshIndex = val;
                    flag = 0;
                } else {
                    printf("\nОшибка: введите целое неотрицательное число!\n");
                }
            }
            printf("Индекс Хирша успешно изменён!\n");
            printf("%d  ->  %d\n", oldInt, temp->info.hirshIndex);
            break;

        default:  // choice == 0 или некорректный
            // Ничего не делаем, просто возвращаемся
            break;
    }

    printf("\nНажмите любую клавишу для возврата в меню...");
    getch();
    return head;
}

/// @brief Функция поиска данных в таблице
/// @param head указатель на начало списка
/// @param choice поле для поиска
void findScientist(struct list *head, const int choice) {
    struct list *temp = head;
    int key, count = 0, flag = 1;
    char buffer[75];

    switch (choice) {
        case '1':
            while (flag) {
                printf("Введите ID -> "); 
                fgets(buffer, 75, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (isNumber(buffer) > 0) key = isNumber(buffer);
                else {
                    printf("\nВведено некорректное число в поле 'ID'! Повторите ввод снова\n");
                    continue;
                }
                flag = 0;
            }
            printTableHeader();
            for (; temp != NULL; temp = temp->next) {
                if (temp->info.id == key) {
                    printNode(temp);
                    count++;
                }
            }
            printf("Найдено %d записей с ID '%d'", count, key);
            return;
        
        case '2':
            printf("Введите ФИО -> "); 
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            printTableHeader();
            for (; temp != NULL; temp = temp->next) {
                if (strcmp(temp->info.name, buffer) == 0) {
                    printNode(temp);
                    count++;
                }
            }
            printf("Найдено %d записей с ФИО '%s'", count, buffer);
            return;
        
        case '3':
            printf("Введите Учёную степень -> "); 
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            printTableHeader();
            for (; temp != NULL; temp = temp->next) {
                if (strcmp(temp->info.degree, buffer) == 0) {
                    printNode(temp);
                    count++;
                }
            }
            printf("Найдено %d записей с Учёной степенью '%s'", count, buffer);
            return;
    
        case '4':
            printf("Введите Область наук -> "); fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            printTableHeader();
            for (; temp != NULL; temp = temp->next) {
                if (strcmp(temp->info.area, buffer) == 0) {
                    printNode(temp);
                    count++;
                }
            }
            printf("Найдено %d записей с Областью наук '%s'", count, buffer);
            return;

        case '5':
            while (flag) {
                printf("Введите Кол-во статей -> "); 
                fgets(buffer, 75, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (isNumber(buffer) >= 1) key = isNumber(buffer);
                else {
                    printf("\nВведено некорректное число в поле 'кол-во статей'! Повторите ввод снова\n");
                    continue;
                }
                flag = 0;
            }
            printTableHeader();
            for (; temp != NULL; temp = temp->next) {
                if (temp->info.articles == key) {
                    printNode(temp);
                    count++;
                }
            }
            printf("Найдено %d записей с Кол-вом статей '%d'", count, key);
            return;

        case '6':
            while (flag) {
                printf("Введите Кол-во цитирований -> ");
                fgets(buffer, 75, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (isNumber(buffer) >= 1) key = isNumber(buffer);
                else {
                    printf("\nВведено некорректное число в поле 'кол-во цитирований'! Повторите ввод снова\n");
                    continue;
                }
                flag = 0;
            }
            printTableHeader();
            for (; temp != NULL; temp = temp->next) {
                if (temp->info.quotes == key) {
                    printNode(temp);
                    count++;
                }
            }
            printf("Найдено %d записей с Кол-вом цитирований '%d'", count, key);
            return;

        case '7':
            while (flag) {
                printf("Введите Индекс Хирша -> ");
                fgets(buffer, 75, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (isNumber(buffer) >= 1) key = isNumber(buffer);
                else {
                    printf("\nВведено некорректное число в поле 'индекс Хирша'! Повторите ввод снова\n");
                    continue;
                }
                flag = 0;
            }
            printTableHeader();
            for (; temp != NULL; temp = temp->next) {
                if (temp->info.hirshIndex == key) {
                    printNode(temp);
                    count++;
                }
            }
            printf("Найдено %d записей с Индексом Хирша '%d'", count, key);
            return;

        default:
            printf("Ошибка выбора!\n");
            break;
    }

    printf("\nНажмите любую клавишу для возврата в меню...");
    getch();
}

/// @brief Функция сравнения двух элементов по выбранному полю (вспомогательная функция для sortTable())
int compareScientist(const struct scientist *a, const struct scientist *b, const int choice) {
    switch (choice) {
        case '1': return a->id - b->id;
        case '2': return strcmp(a->name, b->name);
        case '3': return strcmp(a->area, b->area);
        case '4': return strcmp(a->degree, b->degree);
        case '5': return a->articles - b->articles;
        case '6': return a->quotes - b->quotes;
        case '7': return a->hirshIndex - b->hirshIndex;
        default: return 0;
    }
}

/// @brief Функция сортировки таблицы по указанному полю
struct list *sortTable(struct list *head, const int choice, const int mode) {
    int swapped;
    struct list *temp;
    struct list *last = NULL;

    do {
        swapped = 0;
        temp = head;

        for (; temp->next != last; temp = temp->next) {
            switch (mode) {
                case 1: // по убыванию
                    if (compareScientist(&temp->info, &temp->next->info, choice) < 0) {
                        struct scientist information = temp->info;
                        temp->info = temp->next->info;
                        temp->next->info = information;
                        swapped = 1;
                    }
                    break;

                case 0: // по возрастанию
                    if (compareScientist(&temp->info, &temp->next->info, choice) > 0) {
                        struct scientist information = temp->info;
                        temp->info = temp->next->info;
                        temp->next->info = information;
                        swapped = 1;
                    }
                    break;
            }
            
        }
        last = temp;
    } while (swapped);

    return head;
}

void removeDuplicatesFromFile(const char *filename) {
    FILE *f = fopen(filename, "rt");
    if (f == NULL) {
        printf("Файл %s не найден!\n", filename);
        return;
    }
    
    char lines[2000][256];
    int ids[2000];
    int lineCount = 0;
    char buffer[256];
    
    while (fgets(buffer, sizeof(buffer), f)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        
        int id = -1;
        if (sscanf(buffer, "%d", &id) == 1) {
            int duplicate = 0;
            for (int i = 0; i < lineCount; i++) {
                if (ids[i] == id) {
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate) {
                ids[lineCount] = id;
                strcpy(lines[lineCount], buffer);
                lineCount++;
            }
        } else {
            ids[lineCount] = -1;
            strcpy(lines[lineCount], buffer);
            lineCount++;
        }
    }
    fclose(f);
    
    f = fopen(filename, "wt");
    if (f == NULL) {
        printf("\033[31mОшибка записи в файл %s\n\033[0m", filename);
        return;
    }
    for (int i = 0; i < lineCount; i++) {
        fprintf(f, "%s\n", lines[i]);
    }
    fclose(f);
}

void top5ByArea(struct list *head) {
    if (head == NULL) {
        printf("Список пуст!\n");
        return;
    }

    system("cls");
    
    FILE *file = fopen("0results-success.txt", "wt");
    
    struct list *temp = head;
    
    while (temp != NULL) {
        char currentArea[26];
        strcpy(currentArea, temp->info.area);
        
        int processed = 0;
        struct list *prev = head;
        while (prev != temp) {
            if (strcmp(prev->info.area, currentArea) == 0) {
                processed = 1;
                break;
            }
            prev = prev->next;
        }
        
        if (!processed) {
            struct list *areaList = NULL;
            struct list *scan = head;
            
            while (scan != NULL) {
                if (strcmp(scan->info.area, currentArea) == 0) {
                    struct list *newNode = (struct list*)malloc(sizeof(struct list));
                    newNode->info = scan->info;
                    newNode->next = areaList;
                    newNode->prev = NULL;
                    if (areaList) areaList->prev = newNode;
                    areaList = newNode;
                }
                scan = scan->next;
            }
            
            areaList = sortTable(areaList, '7', 0);
            
            printf("\n\n========== %s ==========\n", currentArea);
            
            printf("\nТоп-5 по Хиршу:\n");
            
            struct list *show = areaList;
            printTableHeader();
            for (int i = 0; i < 5 && show != NULL; i++, show = show->next) {
                printNode(show);
                writeToTextFile(file, show);
            }
            
            areaList = sortTable(areaList, '6', 0);
            
            printf("\nТоп-5 по цитированиям:\n");
            
            show = areaList;
            printTableHeader();
            for (int i = 0; i < 5 && show != NULL; i++, show = show->next) {
                printNode(show);
                writeToTextFile(file, show);
            }
            
            deleteNode(&areaList, 0);
        }
        temp = temp->next;
    }
    
    fclose(file);
    removeDuplicatesFromFile("0results-success.txt");

    printf("\n\033[36mРезультаты в файле '0results-success.txt'\n");
    printf("\033[0mНажмите любую клавишу для возвращения в меню...");
    getch();
}

/// @brief Проверка каптчи для критических действий
/// @param action описание действия (выводится пользователю)
/// @return `1` - если проверка пройдена, `0` - если не пройдена или отменено
int captcha(const char *action) {
    int a = rand() % 50 + 1;
    int b = rand() % 50 + 1;
    int op = rand() % 2;
    int correctAnswer, answer;
    char input[10];

    system("cls");
    
    if (op == 0) {
        correctAnswer = a + b;
        printf("\nПодтверждение действия: %s\n", action);
        printf("   Для продолжения решите пример: %d + %d = ", a, b);
    } else {
        if (a < b) { 
            int t = a;
            a = b; 
            b = t; 
        }
        correctAnswer = a - b;
        printf("\nПодтверждение действия: %s\n", action);
        printf("   Для продолжения решите пример: %d - %d = ", a, b);
    }
    
    fgets(input, 10, stdin);
    input[strcspn(input, "\n")] = '\0';

    while (1) {
        if (isNumber(input) != -1) {
            answer = isNumber(input);
            break;
        }
        else {
            printf("Введено некорректное число! Повторите ввод.\n");
            if (op == 0) {
                printf("\nПодтверждение действия: %s\n", action);
                printf("   Для продолжения решите пример: %d + %d = ", a, b);
            } else {
                printf("\nПодтверждение действия: %s\n", action);
                printf("   Для продолжения решите пример: %d - %d = ", a, b);
            }
        }
    }
    
    if (answer == correctAnswer) {
        printf("\033[36mПроверка пройдена. Выполняем...\033[0m\n\n");
        Sleep(2000);
        return 1;
    } else {
        printf("\033[31mНеверный ответ. Действие отменено.\033[0m\n\n");
        Sleep(2000);
        return 0;
    }
}