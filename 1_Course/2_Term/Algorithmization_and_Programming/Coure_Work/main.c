/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
для которой указывается ФИО учёного (75 символов), научная область, учёная степень,
количество статей и цитирований, индекс Хирша.
    Основные функции программы:
- вывод базы на экран
- добавление записи о телефоне в базу;
- исправление записи о телефоне в базе;
- удаление записи о телефоне из базы;
- поиск записи о телефоне в базе;
- выбор записей о моделях телефонов, попадающих в заданный пользователем
диапазон цен.

Вариант задания 4. Утверждено 18.02.2026
Среда программирования Visual Studio Code version 1.115.0
Дата последней коррекции: 10.04.2026.
Версия 1.0
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// TEMPORARY VARIABLES
int STEP = 2;

//-----------------------------типы и глобальные переменные-----------------------------
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
struct list *createListKeyboard(struct list *s);
struct list *addFirst(struct list *s, struct scientist data);
struct list *addLast(struct list *s, struct scientist data);
void exportToTxt(struct list *head);
void exportToBin(struct list *head);
void freeMemory(struct list *s);

//-----------------------------главная функция-----------------------------
int main() {
    int choice;
    struct list *head;
    head = NULL;
    while (1) {
        cancel:
        printf("\n\n\n================ МЕНЮ ================\n");
        printf(":: Организация списка\n");
        printf("   1. Создать таблицу учёных с клавиатуры\n");
        printf("   2. Создать таблицу учёных с файла\n");
        printf("   3. Добавить нового учёного с клавиатуры\n");
        printf("   4. Добавить нового учёного с файла\n");
        printf("   5. Удалить учёного\n");
        
        printf(":: Вывод информации\n");
        printf("   6. Вывести таблицу учёных на экран\n");
        printf("   7. Экспортировать таблицу учёных в файл\n");

        printf("0. Выход из программы\n");
        printf("Выберите действие (0-7) -> ");
        scanf("%d", &choice);
        fflush(stdin);
        
        switch (choice) {  
            case 1:
                head = createListKeyboard(head);
                break;

            case 2:
                break;
            
            case 3:
                if (head != NULL) {
                    int tempChoice;
                    while (1) {
                        printf("================ Считывание данных об учёном с клавиатуры ================\n");
                        printf("   1. Добавить в начало\n");
                        printf("   2. Добавить в конец\n");
                        printf("   0. В меню\n");
                        printf("Выберите действие (0-2) -> ");
                        scanf("%d", &tempChoice);

                        switch (tempChoice) {
                            case 1:
                                head = addFirst(head, readData());
                                goto cancel;
                                break;
                        
                            case 2:
                                head = addLast(head, readData());
                                goto cancel;
                                break;

                            case 0:
                                goto cancel;
                                break;

                            default:
                                printf("Команда не распознана!\n");
                        }
                    }
                }
                else printf("Список пуст!\nРекомендуемое действие: 1.");
                break;

            
            // case 6:
            //     if (head != NULL) viewList(head);
            //     else printf("Список пуст!\nРекомендуемое действие: 1.");
            //     break;
            
            case 7:
                if (head != NULL) {
                    int tempChoice;
                    while (1) {
                        printf("================ Экспорт таблицы в файл ================\n");
                        printf("   1. Экспортировать в текстовый файл (.txt)\n");
                        printf("   2. Экспортировать в бинарный файл (.bin)\n");
                        printf("   0. В меню\n");
                        printf("[ ! ] Внимание! При экспорте в бинарный файл вы сможете взаимодействовать с ним только через данную программу!\n");
                        printf("Выберите действие (0-2) -> ");
                        scanf("%d", &tempChoice);

                        switch (tempChoice) {
                            case 1:
                                exportToTxt(head);
                                goto cancel;
                                break;
                        
                            case 2:
                                exportToBin(head);
                                goto cancel;
                                break;

                            case 0:
                                goto cancel;
                                break;

                            default:
                                printf("Команда не распознана!\n");
                        }
                    }
                }
                else printf("Список пуст!\nРекомендуемое действие: 1.");
                break;
            
            case 0:
                freeMemory(head);
                printf("\n\n\nВыход из программы...");
                return 0;

            default:
                printf("Команда не распознана!\n");
        }
    }
}

/// Функция чтения данных с клавиатуры
struct scientist readData() {
    struct scientist data;
    static int nextID = 1;
    data.id = nextID++;
    while (getchar() != '\n');
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

struct list *addFirst(struct list *s, struct scientist data) {
    struct list *temp;
    temp = (struct list*)malloc(sizeof(struct list));
    temp->info = data;
    temp->next = s;
    temp->prev = NULL;
    return temp;
}

struct list *addLast(struct list *s, struct scientist data) {
    struct list *temp, *e;
    temp = (struct list*)malloc(sizeof(struct list));
    temp->info = data;
    temp->next = NULL;
    if (s == NULL) s = temp;
    else {
        for (e = s; e->next != NULL; e = e->next);
        temp->prev = e;
        e->next = temp;
    }
    return s;
}

/// @brief Функция создания списка с клавиатуры
struct list *createListKeyboard(struct list *head) {
    int choice = 0;
    do {
        if (head == NULL) head = addFirst(head, readData());
        else addLast(head, readData());

        printf("Ввести ещё? \n1. да \n0. нет \n-> ");
        scanf("%d", &choice);
    } while (choice);
    return head;
}

/// Функция вывода таблицы по `STEP` (global variable `int STEP`) элементов на страницу, со скроллингом


/// @brief Функция экспорта таблицы в .txt файл
/// @param head Указатель на начало списка
void exportToTxt(struct list *head) {
    char nameFile[101];
    struct list *temp = head;
    printf("Введите название файла (без указания расширения .txt) -> ");
    fflush(stdin);
    scanf("%s", nameFile);
    strcat(nameFile, ".txt");
    FILE *file = fopen(nameFile, "wt");
    if (file == NULL) {
        printf("Файл %s не найден! Он будет создан в корне текущей папки", nameFile);
    }
    fprintf(file, "+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    fprintf(file, "|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
    fprintf(file, "+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");

    for (; temp != NULL; temp = temp->next) {
        fprintf(file, "| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
         temp->info.id,
         temp->info.name,
         temp->info.degree,
         temp->info.area,
         temp->info.articles,
         temp->info.quotes,
         temp->info.hirshIndex);
        fprintf(file, "+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    }
    printf("Информация записана в файл %s", nameFile);
    fclose(file);
    return;
}

/// @brief Функция экспорта таблицы в .bin файл
/// @param head Указатель на начало списка
void exportToBin(struct list *head) {
    char nameFile[101];
    printf("Введите название файла (без указания расширения .bin) -> ");
    fflush(stdin);
    scanf("%s", nameFile);
    strcat(nameFile, ".bin");
    FILE *file = fopen(nameFile, "wb");
    if (file == NULL) {
        printf("Файл %s не найден! Он будет создан в корне текущей папки", nameFile);
    }

    fclose(file);
    return;
}

struct list *importFromTxt(struct list *head) {
    start:
    char fileName[101];
    printf("Введите названия файла (без указания расширения .txt) -> ");
    fflush(stdin);
    scanf("%s", fileName);
    strcat(fileName, ".txt");
    FILE *file = fopen(fileName, "rt");
    if (file == NULL) {
        printf("Файл %s не найден! Введите корректное название снова", fileName);
        goto start;
    }
    
}

/// @brief Функция очистки памяти
/// @param head Указатель на начало списка
/// @attention Вызывается единожды во время работы программы - пункт "Выход из программы"
void freeMemory(struct list *head) {
    if (head == NULL) {
        printf("Список пуст...\n");
        return;
    }
    struct list *temp=head, *n;
    int c = 0;
    n = temp->next;
    while (temp != NULL) {
        n = temp->next;
        free(temp);
        temp = n;
        c++;
    }
    printf("Было удалено %d записей...\nПамять освобождена...\n", c);
}