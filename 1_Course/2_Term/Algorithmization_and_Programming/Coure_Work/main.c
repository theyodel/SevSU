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
#include <string.h>
#include <windows.h>
#define ESC (int)27
#define PGUP (int)72
#define PGDOWN (int)80
int nextID = 1;

//-----------------------------структуры данных------------------------------
struct scientist {              // Структура для хранения данных об учёном
    int id;                     // ID
    char name[76];              // ФИО
    char area[26];              // Научная область
    char degree[26];            // Учёная степень
    unsigned int articles;      // Количество статей
    unsigned int quotes;        // Количество цитирований
    unsigned int hirshIndex;    // Индекс Хирша
};

struct list {                   // Элемент 2- направленного списка
    struct scientist info;      // Данные об учёном
    struct list *prev;          // Указатель на элемент слева (предыдущий)
    struct list *next;          // Указатель на элемент справа (следующий)
};

//-----------------------------прототипы функций-----------------------------

struct scientist readData();
struct list *createListFromKeyboard(struct list *);
struct list *addFirst(struct list *, const struct scientist);
struct list *addLast(struct list *, const struct scientist);
void viewList(struct list*);
int freeMemory(struct list **, int);
struct list *editElement(struct list *, unsigned int);
struct list *sortTable(struct list *, const int);
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

//------------------------------главная функция------------------------------
int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    
    int mainChoice, secondChoice, flag, saved = 0;
    struct list *head = NULL;

    while (1) {
        system("cls");
        printf("================ МЕНЮ ================\n");
        printf("1. Организация списка\n");
        printf("2. Просмотр таблицы\n");
        printf("3. Добавление новой записи\n");
        printf("4. Удаление записи\n");
        printf("5. Корректировка записи\n");
        printf("6. Сортировка данных\n");
        printf("7. Поиск записи\n");
        printf("8. Сохранить таблицу в файл\n");
        printf("9. Чтение таблицы из файла\n");
        printf("0. Вывести по 5 учёных с наибольшим индексом Хирша и кол-вом цитирований для каждой научной области\n");
        printf("ESC. Выход из программы\n");
        printf("\nВыберите действие (0-9) -> ");
        mainChoice = getch();

        switch (mainChoice) {  
            case '1':
                head = createListFromKeyboard(head);
                saved = 0;
                break;

            case '2':
                if (head != NULL) {
                    viewList(head);
                } else {
                    printf("Список пуст!\nРекомендуемое действие: 1.");
                    getch();
                }
                break;

            case '3':
                if (head == NULL) {
                    printf("Список пуст!\nРекомендуемое действие: 1.\n\n");
                    printf("Нажмите любую клавишу для выхода в меню...");
                    getch();
                    break;
                }
                flag = 1;
                while (flag) {
                    system("cls");
                    printf("================ Считывание данных об учёном ================\n");
                    printf("1. Добавить в начало\n");
                    printf("2. Добавить в конец\n");
                    printf("0. В меню\n");
                    printf("Выберите действие (0-2) -> ");
                    secondChoice = getch();
                    system("cls");
                    switch (secondChoice) {
                        case '1':
                            head = addFirst(head, readData());
                            flag = 0;
                            break;
                        case '2':
                            head = addLast(head, readData());
                            flag = 0;
                            break;
                        case '0':
                            flag = 0;
                            break;
                        default:
                            printf("\nКоманда не распознана! Нажмите любую клавишу...");
                            getch();
                    }
                }
                saved = 0;
                break;

            case '4':
                if (head == NULL) {
                    printf("Список пуст!\nРекомендуемое действие: 1.");
                    getch();
                    break;
                }
                system("cls");
                printf("Введите ID удаляемой записи или 0 (удалить все) -> ");
                scanf("%d", &secondChoice);
                flag = freeMemory(&head, secondChoice);
                if (flag == -1) printf("Ошибка при удалении!\n");
                printf("Нажмите любую клавишу для выхода в меню...");
                getch();
                break;

            case '5':
                if (head == NULL) {
                    printf("Список пуст!");
                    getch();
                    break;
                }
                system("cls");
                printf("Введите ID записи для корректировки -> ");
                scanf("%d", &secondChoice);
                head = editElement(head, secondChoice);
                getch();
                break;

            case '6':
                system("cls");
                if (head == NULL || head->next == NULL) {    
                    printf("Список не нуждается в сортировке или пуст\n\nНажмите любую клавишу, чтобы вернуться в меню...");
                    getch();
                    break;
                }
                flag = 1;
                while (flag) {
                    system("cls");
                    printf("================ Сортировка таблицы ================\n");
                    printf("1. По ID\n2. По ФИО\n3. По учёной степени\n4. По области наук\n");
                    printf("5. По числу статей\n6. По числу цитирований\n7. По индексу Хирша\nESC. В меню\n");
                    printf("Выберите действие (0-7) -> ");
                    secondChoice = getch();
                    if (secondChoice == ESC) flag = 0;
                    else if ('1' <= secondChoice && secondChoice <= '7') {
                        sortTable(head, secondChoice);
                    } else {
                        printf("Команда не распознана!\n");
                    }
                    if (flag) {
                        printf("\nНажмите любую клавишу для продолжения...");
                        getch();
                    }
                }
                break;

            case '7':
                if (head == NULL) {
                    printf("Таблица пуста!\n");
                    getch();
                    break;
                }
                flag = 1;
                while (flag) {
                    system("cls");
                    printf("================ Поиск учёного ================\n");
                    printf("1. По ID\n2. По ФИО\n3. По учёной степени\n4. По области наук\n");
                    printf("5. По числу статей\n6. По числу цитирований\n7. По индексу Хирша\nESC. В меню\n");
                    printf("Выберите действие (0-7) -> ");
                    secondChoice = getch();
                    if (secondChoice == ESC) flag = 0;
                    else if ('1' <= secondChoice && secondChoice <= '7') {
                        findScientist(head, secondChoice);
                    } else {
                        printf("Команда не распознана!\n");
                    }
                    if (flag) {
                        printf("\nНажмите любую клавишу для продолжения...");
                        getch();
                    }
                }
                break;

            case '8':
                if (head == NULL) {
                    printf("Список пуст!\n");
                    getch();
                    break;
                }
                flag = 1;
                while (flag) {
                    system("cls");
                    printf("================ Экспорт таблицы ================\n");
                    printf("1. В текстовый файл (.txt)\n");
                    printf("2. В бинарный файл (.bin)\n");
                    printf("0. В меню\n");
                    printf("Выберите действие (0-2) -> ");
                    secondChoice = getch();
                    switch (secondChoice) {
                        case '1':
                            exportToTxt(head);
                            flag = 0;
                            break;
                        case '2':
                            exportToBin(head);
                            flag = 0;
                            break;
                        case '0':
                            flag = 0;
                            break;
                        default:
                            printf("\n\nНеверный выбор! Нажмите любую клавишу...");
                            getch();
                    }
                }
                saved = 1;
                break;

            case '9':
                if (head != NULL && saved == 0) {
                    char word[10];
                    system("cls");
                    printf("[!] Внимание! Есть несохранённые данные.\nВведите 'CAPTCHA' для : ");
                    fgets(word, 10, stdin);
                    if (strcmp(word, "CAPTCHA") != 0) break;
                }
                flag = 1;
                while (flag) {
                    system("cls");
                    printf("================ Импорт таблицы ================\n");
                    printf("1. Из текстового файла (.txt)\n");
                    printf("2. Из бинарного файла (.bin)\n");
                    printf("0. В меню\n");
                    printf("Выберите действие (0-2) -> ");
                    secondChoice = getch();
                    switch (secondChoice) {
                        case '1':
                            head = importFromTxt(head);
                            flag = 0;
                            break;
                        case '2':
                            head = importFromBin(head);
                            flag = 0;
                            break;
                        case '0':
                            flag = 0;
                            break;
                        default:
                            printf("\n\nНеверный выбор! Нажмите любую клавишу...");
                            getch();
                    }
                }
                saved = 0;
                break;

            case '0':
                if (head == NULL) {
                    printf("Список пуст!\n");
                    getch();
                    break;
                }
                top5ByArea(head);
                getch();
                break;

            case ESC:
                int count = freeMemory(&head, 0);
                printf("\nУдалено %d записей. Выход...\n", count);
                Sleep(3000);
                return 0;

            default:
                printf("\nКоманда не распознана!");
                break;
        }
    }
}



void printTableHeader() {
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
}

void printNode(struct list *node) {
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

        printf("Ввести ещё? \n1. да \n0. нет \n-> ");
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
        printf("\nЗаписи: %d-%d из %d. \nИспользуйте W (предыдущая страница)\t S (следующая страница) \nPageUp (предыдущая запись)\t PageDown (следующая запись) \nESC (выход)", first, last, total);
        printf("\n\n[ * ] Одна страница занимает %d записей", pageSize);

        key = getch();

        if (key == 'w' || key == 'W') {
            int new_start = start - pageSize;
            if (new_start < 0) new_start = 0;
            start = new_start;
        } else if (key == 's' || key == 'S') {
            int new_start = start + pageSize;
            int max_start = total - pageSize;
            if (max_start < 0) max_start = 0;
            if (new_start > max_start) new_start = max_start;
            start = new_start;
        } else if (key == PGUP) {
            int new_start = start - 1;
            if (new_start < 0) new_start = 0;
            start = new_start;
        } else if (key == PGDOWN) {
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
        printf("Ошибка создания файла '%s'!\n", fileName);
        printf("Нажмите любую кнопку для возвращения в меню...");
        getch();
        return;
    }
    
    for (; temp != NULL; temp = temp->next) {
        writeToTextFile(file, temp);
    }
    fclose(file);

    printf("Информация записана в файл '%s'\n", fileName);
    printf("Нажмите любую кнопку для возвращения в меню...");
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
        printf("Ошибка создания файла '%s'!\n", fileName);
        printf("Нажмите любую кнопку для возвращения в меню...");
        getch();
        return;
    }
    
    for (struct list *temp = head; temp != NULL; temp = temp->next) {
        fwrite(&temp->info, sizeof(struct scientist), 1, file);
    }
    fclose(file);

    printf("Таблица успешно экспортирована в файл '%s'\n", fileName);
    printf("Нажмите любую кнопку для возвращения в меню...");
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
        printf("\n\nФайл '%s' не найден!\n", fileName);
        printf("Нажмите любую кнопку для возвращения в меню...");
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

    printf("\nТаблица успешно импортирована из файла '%s'\n", fileName);
    printf("Нажмите любую кнопку для возвращения в меню...");
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
        printf("\n\nФайл '%s' не найден!\n", fileName);
        printf("Нажмите любую кнопку для возвращения в меню...");
        getch();
        return head;
    }
    
    while (fread(&data, sizeof(struct scientist), 1, file) == 1) {
        if (head == NULL)
            head = addFirst(head, data);
        else
            head = addLast(head, data);
    }
    
    fclose(file);

    setNewID(head, 1);

    printf("\nТаблица успешно импортирована из файла '%s'\n", fileName);
    printf("Нажмите любую кнопку для возвращения в меню...");
    getch();

    return head;
}

/// @brief Функция очистки памяти
/// @param head Указатель на начало списка
/// @return Кол-во удалённых записей
int freeMemory(struct list **head, int choice) {
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
            
            // Поиск элемента с нужным id
            while (toDel != NULL && toDel->info.id != choice) toDel = toDel->next;
            
            // Элемент не найден
            if (toDel == NULL) {
                printf("\nЭлемент с id=%d не найден\n", choice);
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
struct list *editElement(struct list *head, unsigned int id) {
    int choice, oldInt, flag = 1;
    char oldLine[76], buffer[25];
    struct list *temp = head;

    for (; temp->info.id != id; temp = temp->next);

    if (temp != NULL) {
        printf("Запись с ID '%d' найдена!\n\n");
        printf("Выберите поле, которое хотите изменить:\n");
        printf("1. ФИО\n");
        printf("2. Учёная степень\n");
        printf("3. Область науки\n");
        printf("4. Кол-во статей\n");
        printf("5. Кол-во цитирований\n");
        printf("6. Индекс Хирша\n");
        printf("0. В меню\n");
        printf("Выберите действие (0-6) ->\n");
        choice = getch();
        switch (choice) {
            case '1':
                printf("Введите ФИО -> ");
                strcpy(oldLine, temp->info.name);
                fgets(temp->info.name, 75, stdin);
                printf("ФИО успешно изменено!\n");
                printf("%s  ->  %s\n", oldLine, temp->info.name);
                break;
            
            case '2':
                printf("Введите Учёную сепень -> ");
                strcpy(oldLine, temp->info.degree);
                fgets(temp->info.degree, 25, stdin);
                printf("Учёная степень успешно изменена!\n");
                printf("%s  ->  %s\n", oldLine, temp->info.degree);
                break;

            case '3':
                printf("Введите Область наук -> ");
                strcpy(oldLine, temp->info.area);
                fgets(temp->info.area, 25, stdin);
                printf("Учёная степень успешно изменена!\n");
                printf("%s  ->  %s\n", oldLine, temp->info.area);
                break;

            case '4':
                oldInt = temp->info.articles;
                while (flag) {
                    printf("Введите Кол-во статей -> ");
                    scanf("%25s", buffer);
                    if (isNumber(buffer)>=0) temp->info.articles = isNumber(buffer);
                    else {
                        printf("\nВведено некорректное число в поле 'кол-во научных статей'! Повторите ввод снова\n");
                        continue;
                    }
                    flag = 0;
                }
                printf("Кол-во статей успешно изменено!\n");
                printf("%d  ->  %d\n", oldInt, temp->info.articles);
                break;

            case '5':
                oldInt = temp->info.quotes;
                while (flag) {
                    printf("Введите Кол-во цитирований -> ");
                    scanf("%25s", buffer);
                    if (isNumber(buffer)>=0) temp->info.quotes = isNumber(buffer);
                    else {
                        printf("\nВведено некорректное число в поле 'кол-во цитирований'! Повторите ввод снова\n");
                        continue;
                    }
                    flag = 0;
                }
                printf("Кол-во цитирований успешно изменено!\n");
                printf("%d  ->  %d\n", oldInt, temp->info.quotes);
                break;

            case '6':
                oldInt = temp->info.hirshIndex;
                while (flag) {
                    printf("Введите Кол-во цитирований -> ");
                    scanf("%25s", buffer);
                    if (isNumber(buffer)>=0) temp->info.hirshIndex = isNumber(buffer);
                    else {
                        printf("\nВведено некорректное число в поле 'индекс Хирша'! Повторите ввод снова\n");
                        continue;
                    }
                    flag = 0;
                }
                printf("Индекс Хирша успешно изменён!\n");
                printf("%d  ->  %d\n", oldInt, temp->info.hirshIndex);
                break;
        }
    } else {
        printf("Запись с ID '%d' не найдена!", id);
    }

    printf("\nНажмите любую кнопку для возврата в меню...");
    getch();

    return head;
}

/// @brief Функция поиска данных в таблице
/// @param head указатель на начало списка
/// @param choice поле для поиска
void findScientist(struct list *head, const int choice) {
    struct list *temp = head;
    int key, count = 0, flag = 1;
    char kWord[75];

    switch (choice) {
        case '1':
            while (flag) {
                printf("Введите ID -> "); fgets(kWord, 75, stdin);
                if (isNumber(kWord) >= 1) key = isNumber(kWord);
                else {
                    printf("\nВведено некорректное число в поле 'индекс Хирша'! Повторите ввод снова\n");
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
            printf("Введите ФИО -> "); fgets(kWord, sizeof(kWord), stdin);
            kWord[strcspn(kWord, "\n")] = 0;
            printTableHeader();
            for (; temp != NULL; temp = temp->next) {
                if (strcmp(temp->info.name, kWord) == 0) {
                    printNode(temp);
                    count++;
                }
            }
            printf("Найдено %d записей с ФИО '%s'", count, kWord);
            return;
        
        case '3':
            printf("Введите Учёную степень -> "); fgets(kWord, sizeof(kWord), stdin);
            kWord[strcspn(kWord, "\n")] = 0;
            printTableHeader();
            for (; temp != NULL; temp = temp->next) {
                if (strcmp(temp->info.degree, kWord) == 0) {
                    printNode(temp);
                    count++;
                }
            }
            printf("Найдено %d записей с Учёной степенью '%s'", count, kWord);
            return;
    
        case '4':
            printf("Введите Область наук -> "); fgets(kWord, sizeof(kWord), stdin);
            kWord[strcspn(kWord, "\n")] = 0;
            printTableHeader();
            for (; temp != NULL; temp = temp->next) {
                if (strcmp(temp->info.area, kWord) == 0) {
                    printNode(temp);
                    count++;
                }
            }
            printf("Найдено %d записей с Областью наук '%s'", count, kWord);
            return;

        case '5':
            while (flag) {
                printf("Введите Кол-во статей -> "); fgets(kWord, 75, stdin);
                if (isNumber(kWord) >= 1) key = isNumber(kWord);
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
                printf("Введите Кол-во цитирований -> "); fgets(kWord, 75, stdin);
                if (isNumber(kWord) >= 1) key = isNumber(kWord);
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
                printf("Введите Индекс Хирша -> "); fgets(kWord, 75, stdin);
                if (isNumber(kWord) >= 1) key = isNumber(kWord);
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

    printf("\nНажмите любую кнопку для возврата в меню...");
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
struct list *sortTable(struct list *head, const int choice) {
    int swapped;
    struct list *temp;
    struct list *last = NULL;

    do {
        swapped = 0;
        temp = head;

        while (temp->next != last) {
            if (compareScientist(&temp->info, &temp->next->info, choice) > 0) {
                struct scientist information = temp->info;
                temp->info = temp->next->info;
                temp->next->info = information;
                swapped = 1;
            }
            temp = temp->next;
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
        printf("Ошибка записи в файл %s\n", filename);
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
            
            areaList = sortTable(areaList, '7');
            
            printf("\n\n========== %s ==========\n", currentArea);
            
            printf("\nТоп-5 по Хиршу:\n");
            
            struct list *show = areaList;
            printTableHeader();
            for (int i = 0; i < 5 && show != NULL; i++, show = show->next) {
                printNode(show);
                writeToTextFile(file, show);
            }
            
            areaList = sortTable(areaList, '6');
            
            printf("\nТоп-5 по цитированиям:\n");
            
            show = areaList;
            printTableHeader();
            for (int i = 0; i < 5 && show != NULL; i++, show = show->next) {
                printNode(show);
                writeToTextFile(file, show);
            }
            
            freeMemory(&areaList, 0);
        }
        temp = temp->next;
    }
    
    fclose(file);
    removeDuplicatesFromFile("0results-success.txt");

    printf("\nРезультаты в файле '0results-success.txt'\n");
    printf("Нажмите любую кнопку для возвращения в меню...");
    getch();
}