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
#include <windows.h>
#define STEP 2

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
struct list *createListKeyboard(struct list *);
struct list *addFirst(struct list *, struct scientist);
struct list *addLast(struct list *, struct scientist);
void viewList(struct list*);
void exportToTxt(struct list *);
void exportToBin(struct list *);
struct list *importFromTxt(struct list *);
void findScientist(struct list *, int);
int freeMemory(struct list *);

//------------------------------главная функция------------------------------
int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    int mainChoice, secondChoice, flag = 1, saved = 0;
    struct list *head;
    head = NULL;
    while (1) {
        flag = 1; // возобновляем флаг для циклов подменю

        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n================ МЕНЮ ================\n");
        printf("1. Организация списка\n");
        printf("2. Просмотр таблицы\n");
        printf("3. Добавление новой записи в таблицу\n");
        printf("4. Удаление записи из таблицы\n");
        printf("5. Корректировка записи в таблице\n");
        printf("6. Сортировка данных в таблице\n");
        printf("7. Поиск записи в таблице\n");
        printf("8. Сохранить таблицу в файл\n");
        printf("9. Чтение таблицы из файла\n");
        printf("10. Обработка таблицы и просмотр результатов обработки в таблицу\n");
        printf("0. Выход из программы\n");
        printf("Выберите действие (0-10) -> ");
        scanf("%d", &mainChoice);
        
        switch (mainChoice) {  
            case 1:
                head = createListKeyboard(head);
                saved = 0;
                break;

            case 2:
                if (head != NULL) viewList(head);
                else printf("Список пуст!\nРекомендуемое действие: 1.");
                break;
                
            
            case 3:
                if (head != NULL) {
                    while (flag) {
                        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n================ Считывание данных об учёном с клавиатуры ================\n");
                        printf("1. Добавить в начало\n");
                        printf("2. Добавить в конец\n");
                        printf("0. В меню\n");
                        printf("Выберите действие (0-2) -> ");
                        scanf("%d", &secondChoice);

                        switch (secondChoice) {
                            case 1:
                                head = addFirst(head, readData());
                                flag = 0;
                                break;
                        
                            case 2:
                                head = addLast(head, readData());
                                flag = 0;
                                break;

                            case 0:
                                flag = 0;
                                break;

                            default:
                                printf("Команда не распознана!\n");
                        }
                    }
                    saved = 0;
                }
                else printf("Список пуст!\nРекомендуемое действие: 1.");
                break;

            
            case 5:
                if (head == NULL) {
                    printf("Список пуст!");
                } else {
                    printf("Введите ID записи, которую хотите скорректировать -> "); scanf("%d", &secondChoice);
                }
                break;
                
            
            case 7:
                if (head == NULL) {
                    printf("Таблица пуста!\n");
                } else {
                    int c;
                    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n================ Экспорт таблицы в файл ================\n");
                    printf("1. Поиск по ID\n");
                    printf("2. Поиск по ФИО\n");
                    printf("3. Поиск по Учёной степени\n");
                    printf("4. Поиск по Области наук\n");
                    printf("5. Поиск по Количеству статей\n");
                    printf("6. Поиск по Количеству цитирований\n");
                    printf("7. Поиск по Индексу Хирша\n");
                    printf("0. Выход\n");
                    printf("Выберите действие (0-7) -> ");
                    scanf("%d", &c);
                    if (c == 0) flag = 0;
                    findScientist(head, c);
                    saved = 0;
                }
                break;
            
            case 8:
                if (head != NULL) {
                    while (flag) {
                        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n================ Экспорт таблицы в файл ================\n");
                        printf("1. Экспортировать в текстовый файл (.txt)\n");
                        printf("2. Экспортировать в бинарный файл (.bin)\n");
                        printf("0. В меню\n");
                        printf("[ ! ] Внимание! При экспорте в бинарный файл вы сможете взаимодействовать с ним только через данную программу!\n");
                        printf("Выберите действие (0-2) -> ");
                        scanf("%d", &secondChoice);

                        switch (secondChoice) {
                            case 1:
                                exportToTxt(head);
                                flag = 0;
                                break;
                        
                            case 2:
                                exportToBin(head);
                                flag = 0;
                                break;

                            case 0:
                                flag = 0;
                                break;

                            default:
                                printf("Команда не распознана!\n");
                        }
                    }
                    saved = 1;
                }
                else printf("Список пуст!\nРекомендуемое действие: 1.");
                break;

            case 9:
                if (head != NULL && saved == 0) {
                    char word[10];
                    printf("[ ! ] Внимание! У Вас есть несохранённый список! Если Вы хотите перезаписать данные, то напишите слово 'yes'");
                    scanf("%s", word);
                    if (strcmp(word, "yes")==0) {
                        while (flag) {
                            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n================ Импорт таблицы из файла ================\n");
                            printf("1. Импортировать из текстового файла (.txt)\n");
                            printf("2. Импортировать из бинарного файла (.bin)\n");
                            printf("0. В меню\nВыберите действие (0-2) -> ");
                            scanf("%d", &secondChoice);
                            switch (secondChoice) {
                                case 1:
                                    head = importFromTxt(head);
                                    printf("Таблица успешно импортирована!\n");
                                    flag = 0;
                                    break;

                                case 0:
                                    flag = 0;
                                    break;

                                default:
                                    printf("Команда не распознана!");
                            }
                        }
                    }
                } else {
                    while (flag) {
                        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n================ Импорт таблицы из файла ================\n");
                        printf("1. Импортировать из текстового файла (.txt)\n");
                        printf("2. Импортировать из бинарного файла (.bin)\n");
                        printf("0. В меню\nВыберите действие (0-2) -> ");
                        scanf("%d", &secondChoice);
                        switch (secondChoice) {
                            case 1:
                                head = importFromTxt(head);
                                printf("Таблица успешно экспортирована!\n");
                                flag = 0;
                                break;

                            case 0:
                                flag = 0;
                                break;
                                default:
                                    printf("Команда не распознана!");
                        }
                    }
                    saved = 0;
                }
                break;
            
            case 0:
                int count = freeMemory(head);
                printf("Было удалено %d записей", count);
                printf("\nВыход из программы...");
                return 0;

            default:
                printf("Команда не распознана!\n");
        }
    }
}

/// @brief Функция чтения данных с клавиатуры
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

/// @brief Функция добавления в начало списка
/// @param head начало списка
/// @param data данные об учённом
struct list *addFirst(struct list *head, struct scientist data) {
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
struct list *addLast(struct list *head, struct scientist data) {
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

/// @brief Функция вывода таблицы по `STEP` (global variable `int STEP`) элементов на страницу, со скроллингом
/// @param head - Указатель на начало списка
void viewList(struct list *head) {
    if (head == NULL) {
        printf("Список не введён!\n");
        return;
    }
    struct list *temp;
    temp = head;
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    while (temp != NULL) {
        printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
          temp->info.id,
          temp->info.name,
          temp->info.degree,
          temp->info.area,
          temp->info.articles,
          temp->info.quotes,
          temp->info.hirshIndex
        );
        temp = temp->next;
        printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    }
}

/// @brief Функция экспорта таблицы в текстовый (`.txt`) файл
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
        printf("Файл '%s' не найден! Он будет создан в корне текущей папки", nameFile);
    }
    for (; temp != NULL; temp = temp->next) {
        fprintf(file, "%d %s %s %s %d %d %d\n",
         temp->info.id,
         temp->info.name,
         temp->info.degree,
         temp->info.area,
         temp->info.articles,
         temp->info.quotes,
         temp->info.hirshIndex);
    }
    printf("Информация записана в файл '%s'", nameFile);
    fclose(file);
    return;
}

/// @brief Функция экспорта таблицы в `.bin` файл
/// @param head Указатель на начало списка
void exportToBin(struct list *head) {
    char nameFile[101];
    printf("Введите название файла (без указания расширения .bin) -> ");
    fflush(stdin);
    scanf("%s", nameFile);
    strcat(nameFile, ".bin");
    FILE *file = fopen(nameFile, "wb");
    if (file == NULL) {
        printf("Файл '%s' не найден! Он будет создан в корне текущей папки", nameFile);
    }
    fclose(file);
    return;
}

/// @brief Функция импорта таблицы из текстового (`.txt`) файла
/// @param head Указатель на начало списка
struct list *importFromTxt(struct list *head) {
    char fileName[101], name[25], otch[25];
    struct scientist data;
    
    printf("Введите названия файла (без указания расширения .txt) -> ");
    scanf("%s", fileName);
    strcat(fileName, ".txt");
    FILE *file = fopen(fileName, "rt");
    if (file == NULL) {
        printf("Файл '%s' не найден! Введите корректное название снова", fileName);
        return head;
    } else {
        while (fscanf(file, "%d %s %s %s %s %s %u %u %u", &data.id, data.name, name, otch, data.degree, data.area, &data.articles, &data.quotes, &data.hirshIndex) != EOF) {
            strcat(data.name, " ");
            strcat(data.name, name);
            strcat(data.name, " ");
            strcat(data.name, otch);
            if (head != NULL) {
                head = addLast(head, data);
            }
            else {
                head = addFirst(head, data);
            }
        }
    }
    return head;
}

/// @brief Функция очистки памяти
/// @param head Указатель на начало списка
int freeMemory(struct list *head) {
    if (head == NULL) {
        printf("Список пуст...\n");
        return 0;
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
    return c;
}

/// @brief Функция поиска данных в таблице
/// @param head указатель на начало списка
/// @param choice поле для поиска
void findScientist(struct list *head, int choice) {
    struct list *temp = head;
    int key, count = 0;
    char kWord[75];
    switch (choice) {
        case 1:
            printf("Введите ID -> "); scanf("%d", &key);
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            for (; temp != NULL; temp = temp->next, count++) {
                if (temp->info.id == key) {
                    printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
                        temp->info.id, temp->info.name, temp->info.degree, temp->info.area, temp->info.articles, temp->info.quotes, temp->info.hirshIndex
                    );
                   printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n"); 
                }
            }
            printf("Найдено %d записей с ID '%d'", count, key);
            break;
        
        case 2:
            printf("Введите ФИО -> "); fgets(kWord, sizeof(kWord), stdin);
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            for (; temp != NULL; temp = temp->next, count++) {
                if (strcmp(temp->info.name, kWord) == 0) {
                    printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
                        temp->info.id, temp->info.name, temp->info.degree, temp->info.area, temp->info.articles, temp->info.quotes, temp->info.hirshIndex
                    );
                   printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n"); 
                }
            }
            printf("Найдено %d записей с ФИО '%s'", count, kWord);
            break;
        
        case 3:
            printf("Введите Учёную степень -> "); fgets(kWord, sizeof(kWord), stdin);
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            for (; temp != NULL; temp = temp->next, count++) {
                if (strcmp(temp->info.degree, kWord) == 0) {
                    printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
                        temp->info.id, temp->info.name, temp->info.degree, temp->info.area, temp->info.articles, temp->info.quotes, temp->info.hirshIndex
                    );
                   printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n"); 
                }
            }
            printf("Найдено %d записей с Учёной степенью '%s'", count, kWord);
            break;
    
        case 4:
            printf("Введите Область наук -> "); fgets(kWord, sizeof(kWord), stdin);
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            for (; temp != NULL; temp = temp->next, count++) {
                if (strcmp(temp->info.degree, kWord) == 0) {
                    printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
                        temp->info.id, temp->info.name, temp->info.degree, temp->info.area, temp->info.articles, temp->info.quotes, temp->info.hirshIndex
                    );
                   printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n"); 
                }
            }
            printf("Найдено %d записей с Областью наук '%s'", count, kWord);
            break;

        case 5:
            printf("Введите Кол-во статей -> "); scanf("%d", &key);
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            for (; temp != NULL; temp = temp->next, count++) {
                if (temp->info.articles == key) {
                    printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
                        temp->info.id, temp->info.name, temp->info.degree, temp->info.area, temp->info.articles, temp->info.quotes, temp->info.hirshIndex
                    );
                   printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n"); 
                }
            }
            printf("Найдено %d записей с Кол-вом статей '%d'", count, key);
            break;

        case 6:
            printf("Введите Кол-во цитирований -> "); scanf("%d", &key);
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            for (; temp != NULL; temp = temp->next, count++) {
                if (temp->info.articles == key) {
                    printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
                        temp->info.id, temp->info.name, temp->info.degree, temp->info.area, temp->info.articles, temp->info.quotes, temp->info.hirshIndex
                    );
                   printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n"); 
                }
            }
            printf("Найдено %d записей с Кол-вом цитирований '%d'", count, key);
            break;

        case 7:
            printf("Введите Индекс Хирша -> "); scanf("%d", &key);
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            for (; temp != NULL; temp = temp->next, count++) {
                if (temp->info.articles == key) {
                    printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
                        temp->info.id, temp->info.name, temp->info.degree, temp->info.area, temp->info.articles, temp->info.quotes, temp->info.hirshIndex
                    );
                   printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n"); 
                }
            }
            printf("Найдено %d записей с Индексом Хирша '%d'", count, key);
            break;

        default:
            printf("Ошибка выбора!\n");
            break;
    }
}