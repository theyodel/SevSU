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
для которой указывается ФИО учёного (75 символов), научная область, учёная степень,
количество статей и цитирований, индекс Хирша.
    Основные функции программы:
- вывод базы на экран
- добавление записи об учёном в базу;
- исправление записи об учёном в базе;
- удаление записи об учёном из базы;
- поиск записи об учёном в базе;
- сортировка записей об учёных;

Вариант задания 4. Утверждено 18.02.2026
Среда программирования Visual Studio Code version 1.117.0
Дата последней коррекции: 27.04.2026.
Версия 1.0
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

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
struct list *importFromBin(struct list *);
void findScientist(struct list *, int);
struct list *editElement(struct list *, unsigned int);
int freeMemory(struct list *, int);

//------------------------------главная функция------------------------------
int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    int mainChoice, secondChoice, flag = 1, saved = 0;
    struct list *head;
    head = NULL;
    while (1) {
        flag = 1;

        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n================ МЕНЮ ================\n");
        printf("1. Организация списка\n");
        printf("2. Просмотр таблицы\n");                    // добавить скроллинг
        printf("3. Добавление новой записи в таблицу\n");
        printf("4. Удаление записи из таблицы\n");
        printf("5. Корректировка записи в таблице\n");
        printf("6. Сортировка данных в таблице\n");         // целиком написать
        printf("7. Поиск записи в таблице\n");
        printf("8. Сохранить таблицу в файл\n");
        printf("9. Чтение таблицы из файла\n");             // переделать логику возвращаемого значения (идея - возвращаться будет максимальный ID для обнуления счётчика ID в функции readData())
        printf("10. Обработка таблицы и просмотр результатов обработки в таблицу\n"); // целиком написать
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

            case 4:
                if (head == NULL) printf("Список пуст!\nРекомендуемое действие: 1.");
                else {
                    printf("Введите ID удаляемой записи или 0 если хотите удалить все записи -> ");
                    scanf("%d", &secondChoice);
                    if (freeMemory(head, secondChoice) == -1) {
                        printf("Возникла ошибка при удалении записи!\n");
                    } else {
                        flag = freeMemory(head, secondChoice);
                    }
                }
                break;
          
            case 5:
                if (head == NULL) {
                    printf("Список пуст!");
                } else {
                    printf("Введите ID записи, которую хотите скорректировать -> "); scanf("%d", &secondChoice);
                    head = editElement(head, secondChoice);
                }
                break;
                
            case 6:
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
                    printf("0. В меню\n");
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
                } else printf("Список пуст!\nРекомендуемое действие: 1.");
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

                                case 2:
                                    head = importFromBin(head);
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

                            case 2:
                                head = importFromBin(head);
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
                    saved = 0;
                }
                break;
            
            case 0:
                int count = freeMemory(head, 0);
                printf("Было удалено %d записей", count);
                printf("\nВыход из программы...");
                return 0;

            default:
                printf("Команда не распознана!\n");
                break;
        }
    }
}

/// @brief Функция чтения данных с клавиатуры
/// @return Данные об учёном
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
/// @return Указатель на начало списка (указатель на добавленный в начало элемент)
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
/// @return Указатель на начало списка
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
/// @return Указатель на начало списка
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
    struct list *temp = head;
    int count = 1;
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    while (temp != NULL && count<=15) {
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
        count++;
        printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    }
}

/// @brief Функция экспорта таблицы в текстовый (`.txt`) файл
/// @param head Указатель на начало списка
void exportToTxt(struct list *head) {
    char fileName[101];
    struct list *temp = head;
    printf("Введите название файла (без указания расширения .txt) -> ");
    fflush(stdin);
    scanf("%s", fileName);
    strcat(fileName, ".txt");
    FILE *file = fopen(fileName, "wt");
    if (file == NULL) {
        printf("Файл '%s' не найден! Он будет создан в корне текущей папки", fileName);
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
    printf("Информация записана в файл '%s'", fileName);
    fclose(file);
    return;
}

/// @brief Функция экспорта таблицы в `.bin` файл
/// @param head Указатель на начало списка
void exportToBin(struct list *head) {
    char fileName[101];
    printf("Введите название файла -> ");
    fflush(stdin);
    scanf("%s", fileName);
    if (fileName[strlen(fileName)-4]!='.' && fileName[strlen(fileName)-3]!='b' && fileName[strlen(fileName)-2]!='i' && fileName[strlen(fileName)-1]!='n') strcat(fileName, ".bin");
    FILE *file = fopen(fileName, "ab");
    if (file == NULL) {
        printf("Файл '%s' не найден! Он будет создан в корне текущей папки", fileName);
    }
    for (struct list *temp = head; temp!=NULL; temp=temp->next) {
        fwrite(&temp->info, sizeof(struct scientist), 1, file);
    }
    printf("Таблица успешно экспортирована в файл '%s'", fileName);
    fclose(file);
    return;
}

/// @brief Функция импорта таблицы из текстового (`.txt`) файла
/// @param head Указатель на начало списка
/// @return Указатель на начало списка
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
            } else {
                head = addFirst(head, data);
            }
        }
    }
    return head;
}

/// @brief Функция импорта таблицы из бинарного (`.bin`) файла
/// @param head Указатель на начало списка
/// @return Указатель на начало списка
struct list *importFromBin(struct list *head) {
    char fileName[101];
    printf("Введите название файла -> ");
    fflush(stdin);
    scanf("%s", fileName);
    struct scientist data;
    if (fileName[strlen(fileName)-4]!='.' && fileName[strlen(fileName)-3]!='b' && fileName[strlen(fileName)-2]!='i' && fileName[strlen(fileName)-1]!='n') strcat(fileName, ".bin");
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("Файл '%s' не найден!", fileName);
    } else {
        while (fread(&data, sizeof(struct scientist), 1, file) == 1) {
            if (head == NULL) {
                head = addFirst(head, data);
            } else {
                head = addLast(head, data);
            }
        }
        fclose(file);
    }
    return head;
}

/// @brief Функция очистки памяти
/// @param head Указатель на начало списка
/// @return Кол-во удалённых записей
int freeMemory(struct list *head, int choice) {
    struct list *temp=head;
    switch (choice) {
        case 0:
            if (head == NULL) {
                printf("Список пуст...\n");
                return 0;
            }        
            int c = 0;
            while (head != NULL) {
                head = head->next;
                free(temp);
                temp = NULL; 
                temp = head;
                c++;
            }
            return c;
        
        default:
            struct list* toDel = head;
            while (toDel->info.id != choice)  toDel = toDel->next;
            if (toDel == NULL) return -1;
            temp = toDel->prev;
            temp->next = toDel->next;
            temp->next->prev = temp;
            free(toDel);
            toDel = NULL;
            return 1;
    }
}

/// @brief Функция корректировки данных об учёном по его ID
/// @param head Указатель на начало списка
/// @param id ID записи, которая будет откорректирована
/// @return Указатель на начало списка
struct list *editElement(struct list *head, unsigned int id) {
    int choice, oldInt;
    char oldLine[76];
    struct list *temp = head;
    for (; temp->info.id != id; temp = temp->next);
    if (temp != NULL) {
        printf("Запись с ID '%d' найдена!\n");
        printf("Выберите поле, которое хотите изменить\n");
        printf("1. ФИО\n");
        printf("2. Учёная степень\n");
        printf("3. Область науки\n");
        printf("4. Кол-во статей\n");
        printf("5. Кол-во цитирований\n");
        printf("6. Индекс Хирша\n");
        printf("0. В меню\n");
        printf("Выберите действие (0-6) -> ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("Введите ФИО -> ");
                strcpy(oldLine, temp->info.name);
                fgets(temp->info.name, 75, stdin);
                printf("ФИО успешно изменено!\n");
                printf("%s  ->  %s\n", oldLine, temp->info.name);
                break;
            
            case 2:
                printf("Введите Учёную сепень -> ");
                strcpy(oldLine, temp->info.degree);
                fgets(temp->info.degree, 75, stdin);
                printf("Учёная степень успешно изменена!\n");
                printf("%s  ->  %s\n", oldLine, temp->info.degree);
                break;

            case 3:
                printf("Введите Область наук -> ");
                strcpy(oldLine, temp->info.area);
                fgets(temp->info.area, 75, stdin);
                printf("Учёная степень успешно изменена!\n");
                printf("%s  ->  %s\n", oldLine, temp->info.area);
                break;

            case 4:
                printf("Введите Кол-во статей -> ");
                oldInt = temp->info.articles;
                scanf("%d", &temp->info.articles);
                printf("Кол-во статей успешно изменено!\n");
                printf("%d  ->  %d\n", oldInt, temp->info.articles);
                break;

            case 5:
                printf("Введите Кол-во цитирований -> ");
                oldInt = temp->info.quotes;
                scanf("%d", &temp->info.quotes);
                printf("Кол-во цитирований успешно изменено!\n");
                printf("%d  ->  %d\n", oldInt, temp->info.quotes);
                break;

            case 6:
                printf("Введите Индекс Хирша -> ");
                oldInt = temp->info.hirshIndex;
                scanf("%d", &temp->info.hirshIndex);
                printf("Индекс Хирша успешно изменён!\n");
                printf("%d  ->  %d\n", oldInt, temp->info.hirshIndex);
                break;
        }
    } else {
        printf("Запись с ID '%d' не найдена!", id);
    }
    return head;
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