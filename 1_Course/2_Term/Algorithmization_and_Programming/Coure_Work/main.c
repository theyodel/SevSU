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
#define PAGESIZE (int)15
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

int showMenu(const char *, const char *[], int );
int inputInt(const char *);
void handleSort(struct list *);
void handleFind(struct list *);
void handleExport(struct list *, int *);
void handleImport(struct list **, int *);
void handleAdd(struct list **, int *);

//------------------------------главная функция------------------------------
int main() {
    srand(time(NULL));
    struct list *head = NULL;
    int saved = 0;

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

    while (1) {
        int choice = showMenu("МЕНЮ", mainMenuItems, mainMenuSize);
        if (choice == -1) choice = 10;

        switch (choice) {
            case 0:
                if (head && !saved && !captcha("создание нового списка (текущие данные будут потеряны)"))
                    break;
                head = createListFromKeyboard(head);
                saved = 0;
                break;

            case 1:
                if (head) viewList(head);
                else {
                    system("cls");
                    printf("Список пуст!\nРекомендуемое действие: 1.\n\nНажмите любую клавишу...");
                    getch();
                }
                break;

            case 2:
                handleAdd(&head, &saved);
                break;

            case 3:
                if (!head) {
                    printf("Список пуст!\nРекомендуемое действие: 1.");
                    getch();
                    break;
                }
                system("cls");
                int idToDel = inputInt("Введите ID удаляемой записи или 0 (удалить все) -> ");
                if (idToDel == 0 && !saved && !captcha("удаление ВСЕХ записей")) {
                    printf("Нажмите любую клавишу...");
                    getch();
                    break;
                }
                int delCount = deleteNode(&head, idToDel);
                if (delCount == -1) printf("Ошибка при удалении!\n");
                else printf("%d записей удалено\n", delCount);
                printf("Нажмите любую клавишу...");
                getch();
                break;

            case 4:
                if (!head) {
                    printf("Список пуст!");
                    getch();
                    break;
                }
                system("cls");
                int idToEdit = inputInt("Введите ID записи для корректировки -> ");
                head = editElement(head, idToEdit);
                saved = 0;
                break;

            case 5:
                handleSort(head);
                break;

            case 6:
                handleFind(head);
                break;

            case 7:
                handleExport(head, &saved);
                break;

            case 8:
                handleImport(&head, &saved);
                break;

            case 9:
                if (!head) {
                    printf("Список пуст!\n");
                    getch();
                    break;
                }
                top5ByArea(head);
                getch();
                break;

            case 10:
                if (head && !saved && !captcha("выход без сохранения")) break;
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

/// @brief Функция вывода шапки таблицы
void printTableHeader() {
    printf("╔══════╦═════════════════════════════════════════════════════════════════════════════╦═══════════════════════════╦═══════════════════════════╦═══════════════╦════════════════════╦══════════════╗\n");
    printf("║  ID  ║                             Фамилия Имя Отчество                            ║       Учёная Степень      ║        Область Науки      ║ Кол-во статей ║ Кол-во цитирований ║ Индекс Хирша ║\n");
    printf("╠══════╬═════════════════════════════════════════════════════════════════════════════╬═══════════════════════════╬═══════════════════════════╬═══════════════╬════════════════════╬══════════════╣\n");
}

/// @brief Функция печати очередного учёного
/// @param node указатель на элемент, который будет напечатан
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

/// @brief функция установки (обновления) уникального ID после изменений базы
/// @param head указатель на первый элемент списка
/// @param _mode режим
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

/// @brief функция перевода char -> int
/// @param ch символ
/// @return int представление символа
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

/// @brief функция проверки строки на число
/// @param _str строка для проверки
/// @return `-1` - если строка не является целым числом из открзка [0:]   иначе   число
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
        
        if (isNumber(buffer) != -1) {
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
        
        if (isNumber(buffer) != -1) {
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
        
        if (isNumber(buffer) != -1) {
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

    if (head == NULL) {
        head = temp;
        head->prev = NULL;
    } else {
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
    int choice = 1;

    setNewID(head, 1);

    while (choice) {
        system("cls");
        if (head == NULL) head = addFirst(head, readData());
        else addLast(head, readData());

        printf("\nВвести ещё? \n1. да \n0. нет");
        choice = getch();
        choice -= 48;
    }

    return head;
}

/// @brief Функция вывода таблицы со скроллингом
/// @param head - Указатель на начало списка
void viewList(struct list *head) {
    int total = 0, start = 0;
    struct list *cur = head;
    char key;

    for (; cur != NULL; cur = cur->next) total++;

    while (1) {
        system("cls");
        struct list *temp = head;
        int idx = 0, printed = 0;

        printTableHeader();

        for (; temp != NULL, idx < start; temp = temp->next) idx++;

        for (; temp != NULL && printed < PAGESIZE; temp = temp->next, printed++) printNode(temp);

        int first = start + 1;
        int last = (start + printed < total) ? start + printed : total;
        printf("\nЗаписи: %d-%d из %d. \nИспользуйте:\n← (предыдущая страница)\t → (следующая страница) \n↑ (предыдущая запись)\t ↓ (следующая запись) \nESC (выход)", first, last, total);
        printf("\n\n[ * ] Одна страница занимает %d записей", PAGESIZE);

        key = getch();

        if (key == LEFT) {
            int new_start = start - PAGESIZE;
            if (new_start < 0) new_start = 0;
            start = new_start;
        } else if (key == RIGHT) {
            int new_start = start + PAGESIZE;
            int max_start = total - PAGESIZE;
            if (max_start < 0) max_start = 0;
            if (new_start > max_start) new_start = max_start;
            start = new_start;
        } else if (key == UP) {
            int new_start = start - 1;
            if (new_start < 0) new_start = 0;
            start = new_start;
        } else if (key == DOWN) {
            int new_start = start + 1;
            int max_start = total - PAGESIZE;
            if (max_start < 0) max_start = 0;
            if (new_start > max_start) new_start = max_start;
            start = new_start;
        } else if (key == ESC) {
            return;
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

/// @brief  функция записи очередного учёного в файл
/// @param file указатель на файл
/// @param node указатель на записываемый элемент
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
    
    printf("+==============+ Экспорт в текстовый файл +==============+\n");
    printf("Введите название файла -> ");
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

    printf("\033[32mИнформация записана в файл '%s'\n", fileName);
    printf("\033[0mНажмите любую клавишу для возвращения в меню...");
    getch();
}

/// @brief Функция экспорта таблицы в `.bin` файл
/// @param head Указатель на начало списка
void exportToBin(struct list *head) {
    char fileName[101];
    printf("+==============+ Экспорт в бинарный файл +==============+\n");
    printf("Введите название файла -> ");
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

    printf("\033[32mТаблица успешно экспортирована в файл '%s'\n", fileName);
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
    printf("+==============+ Импорт из текстового файла +==============+\n");
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
        head = (head == NULL) ? addFirst(head, data) : addLast(head, data);
    }
    
    fclose(file);

    if (head == NULL) {
        printf("\n\n\033[31mВозникла ошибка во время чтения файла '%s'!\n", fileName);
        printf("\033[0mНажмите любую клавишу для возвращения в меню...");
        getch();
        return head;
    }
    
    setNewID(head, 1);

    printf("\n\033[32mТаблица успешно импортирована из файла '%s'\n", fileName);
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
    printf("+==============+ Импорт из бинарного файла +==============+\n");
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

    if (head == NULL) {
        printf("\n\n\033[31mВозникла ошибка во время чтения файла '%s'!\n", fileName);
        printf("\033[0mНажмите любую клавишу для возвращения в меню...");
        getch();
        return head;
    }

    setNewID(head, 1);

    printf("\n\033[32mТаблица успешно импортирована из файла '%s'\n", fileName);
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
                printf("\nЗапись с ID '%d' не найдена\n", choice);
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
        system("cls");
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
        printf("\nИспользуйте ↑ ↓ (стрелки вверх/вниз), Enter - выбор, ESC - выход.\n");

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

    system("cls");

    switch (choice) {
        case 1:
            while (flag) {
                printf("Введите ID -> "); 
                fgets(buffer, 75, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (isNumber(buffer) != -1) key = isNumber(buffer);
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
        
        case 2:
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
        
        case 3:
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
    
        case 4:
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

        case 5:
            while (flag) {
                printf("Введите Кол-во статей -> "); 
                fgets(buffer, 75, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (isNumber(buffer) != -1) key = isNumber(buffer);
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

        case 6:
            while (flag) {
                printf("Введите Кол-во цитирований -> ");
                fgets(buffer, 75, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (isNumber(buffer) != -1) key = isNumber(buffer);
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

        case 7:
            while (flag) {
                printf("Введите Индекс Хирша -> ");
                fgets(buffer, 75, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (isNumber(buffer) != -1) key = isNumber(buffer);
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
/// @param choice выбранное для сортировки поле
/// @param mode порядок сортировки (`1` - по убыванию | `0` - по возрастанию)
struct list *sortTable(struct list *head, const int choice, const int mode) {
    int swapped;
    struct list *temp, *last = NULL;
    struct scientist data;

    while (swapped) {
        swapped = 0;
        temp = head;

        for (; temp->next != last; temp = temp->next) {
            switch (mode) {
                case 1: // по убыванию
                    if (compareScientist(&temp->info, &temp->next->info, choice) < 0) {
                        data = temp->info;
                        temp->info = temp->next->info;
                        temp->next->info = data;
                        swapped = 1;
                    }
                    break;

                case 0: // по возрастанию
                    if (compareScientist(&temp->info, &temp->next->info, choice) > 0) {
                        data = temp->info;
                        temp->info = temp->next->info;
                        temp->next->info = data;
                        swapped = 1;
                    }
                    break;
            }
        }
        last = temp;
    }

    return head;
}

/// @brief функция удаления дубликатов из текстового файла
/// @param filename имя файла
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

/// @brief функция обработки согласно варианту (вывести по 5 уч. с наибольшим индексом хирша и кол-вом цитирований для каждой научной области)
/// @param head указатель на первый элемент
void top5ByArea(struct list *head) {
    if (head == NULL) {
        printf("Список пуст!\n");
        return;
    }

    system("cls");
    
    FILE *file = fopen("10results.txt", "wt");
    
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
    removeDuplicatesFromFile("10results.txt");

    printf("\n\033[32mРезультаты в файле '10results.txt'\n");
    printf("\033[0mНажмите любую клавишу для возвращения в меню...");
    getch();
}

/// @brief Проверка каптчи для критических действий
/// @param action описание действия (выводится пользователю)
/// @return `1` - если проверка пройдена, `0` - если не пройдена или отменено
int captcha(const char *action) {
    int a = rand() % 50 + 1;
    int b = rand() % 50 + 1;
    int op = rand() % 2; // 0 - сложение | 1 - вычитание
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
        printf("\033[32mПроверка пройдена. Выполняем...\033[0m\n\n");
        Sleep(2000);
        return 1;
    } else {
        printf("\033[31mНеверный ответ. Действие отменено.\033[0m\n\n");
        Sleep(2000);
        return 0;
    }
}

/// @brief вспомогательная функция для перерасчёта длины кириллических (utf8) символов
/// @param s строка для расчёта
/// @return длину строки с учётом разности байт
int utf8_strlen(const char *s) {
    int len = 0;
    while (*s) {
        if ((*s & 0xC0) != 0x80) len++;
        s++;
    }
    return len;
}

/// @brief Универсальная функция для отображения меню и получения выбора пункта.
/// @param title Заголовок меню (выводится сверху)
/// @param items Массив строк с пунктами меню
/// @param itemCount Количество пунктов
/// @return Индекс выбранного пункта (0..itemCount-1) или -1, если нажата ESC
int showMenu(const char *title, const char *items[], int itemCount) {
    int current = 0;
    int key;
    const int width = 98;   // внутренняя ширина (между вертикальными чертами)

    while (1) {
        system("cls");

        // Верхняя граница
        printf("╔");
        for (int i = 0; i < width; i++) printf("═");
        printf("╗\n");

        // Строка заголовка с центрированием по реальной (видимой) длине
        int visibleLen = utf8_strlen(title);
        int leftPad = (width - visibleLen) / 2;
        int rightPad = width - visibleLen - leftPad;

        printf("║");
        for (int i = 0; i < leftPad; i++) printf(" ");
        printf("%s", title);
        for (int i = 0; i < rightPad; i++) printf(" ");
        printf("║\n");

        // Нижняя граница
        printf("╚");
        for (int i = 0; i < width; i++) printf("═");
        printf("╝\n");

        // Пункты меню
        for (int i = 0; i < itemCount; i++) {
            printf("  %s %s\n", (i == current) ? "\033[35m▻" : " ", items[i]);
            printf("\033[0m");
        }
        printf("\nИспользуйте ↑ ↓ (стрелки вверх/вниз), Enter - выбор, ESC - выход.\n");

        key = getch();
        if (key == 224) {
            key = getch();
            if (key == UP) current = (current - 1 + itemCount) % itemCount;
            else if (key == DOWN) current = (current + 1) % itemCount;
        } else if (key == ENTER) {
            return current;
        } else if (key == ESC) {
            return -1;
        }
    }
}

/// @brief Ввод целого неотрицательного числа с консоли с проверкой.
/// @param prompt Приглашение к вводу
/// @return Введённое число
int inputInt(const char *prompt) {
    char buffer[25];
    int value;
    while (1) {
        printf("%s", prompt);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (isNumber(buffer) != -1) {
            value = isNumber(buffer);
            break;
        }
        printf("\033[31mОшибка: введите целое неотрицательное число!\033[0m\n");
    }
    return value;
}

/// @brief Обработка подменю "Сортировка"
void handleSort(struct list *head) {
    if (!head || !head->next) {
        system("cls");
        printf("Список не нуждается в сортировке или пуст.\nНажмите любую клавишу...");
        getch();
        return;
    }
    const char *sortItems[] = {
        "По ID", "По ФИО", "По учёной степени", "По области наук",
        "По числу статей", "По числу цитирований", "По индексу Хирша", "Вернуться в меню"
    };
    int choice = showMenu("СОРТИРОВКА ТАБЛИЦЫ", sortItems, 8);
    if (choice == -1 || choice == 7) return; // ESC или "Вернуться"

    const char *orderItems[] = { "По возрастанию", "По убыванию", "Вернуться" };
    int order = showMenu("ВЫБЕРИТЕ ПОРЯДОК СОРТИРОВКИ", orderItems, 3);
    if (order == -1 || order == 2) return;

    char sortChar = '1' + choice; // '1'..'7'
    sortTable(head, sortChar, order); // order: 0 - возр., 1 - убыв.
    printf("\nСортировка выполнена (%s). \nНажмите любую клавишу...",
           order == 0 ? "по возрастанию" : "по убыванию");
    getch();
}

/// @brief Обработка подменю "Поиск"
void handleFind(struct list *head) {
    if (!head) {
        printf("Таблица пуста!\n");
        getch();
        return;
    }
    const char *findItems[] = {
        "По ID", "По ФИО", "По учёной степени", "По области наук",
        "По числу статей", "По числу цитирований", "По индексу Хирша", "Вернуться"
    };
    int choice = showMenu("ПОИСК УЧЁНОГО", findItems, 8);
    if (choice == -1 || choice == 7) return;
    findScientist(head, choice + 1); // +1, потому что в findScientist choice от 1 до 7
    printf("\nНажмите любую клавишу...");
    getch();
}

/// @brief Обработка подменю "Экспорт"
void handleExport(struct list *head, int *savedFlag) {
    if (!head) {
        printf("Список пуст!\n");
        getch();
        return;
    }
    const char *expItems[] = { "В текстовый файл (.txt)", "В бинарный файл (.bin)", "Вернуться" };
    int choice = showMenu("ЭКСПОРТ ТАБЛИЦЫ", expItems, 3);
    if (choice == -1 || choice == 2) return;
    if (choice == 0) exportToTxt(head);
    else exportToBin(head);
    *savedFlag = 1;
}

/// @brief Обработка подменю "Импорт"
void handleImport(struct list **head, int *savedFlag) {
    if (*head && *savedFlag == 0) {
        if (!captcha("импорт таблицы с потерей несохранённых данных")) return;
        deleteNode(head, 0);
    }
    const char *impItems[] = { "Из текстового файла (.txt)", "Из бинарного файла (.bin)", "Вернуться" };
    int choice = showMenu("ИМПОРТ ТАБЛИЦЫ", impItems, 3);
    if (choice == -1 || choice == 2) return;
    if (choice == 0) *head = importFromTxt(*head);
    else *head = importFromBin(*head);
    *savedFlag = 0;
}

/// @brief Обработка подменю "Добавление записи"
void handleAdd(struct list **head, int *savedFlag) {
    if (!*head) {
        printf("Список пуст!\nРекомендуемое действие: 1.\nНажмите любую клавишу...");
        getch();
        return;
    }
    const char *addItems[] = { "Добавить в начало", "Добавить в конец", "Вернуться" };
    int choice = showMenu("ДОБАВЛЕНИЕ ЗАПИСИ", addItems, 3);
    if (choice == -1 || choice == 2) return;
    struct scientist newSci = readData();
    if (choice == 0) *head = addFirst(*head, newSci);
    else *head = addLast(*head, newSci);
    *savedFlag = 0;
}