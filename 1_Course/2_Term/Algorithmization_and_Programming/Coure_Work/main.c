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
Дата последней коррекции: 29.04.2026.
Версия 1.0
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
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
struct list *createListFromKeyboard(struct list *);
struct list *addFirst(struct list *, struct scientist);
struct list *addLast(struct list *, struct scientist);
void viewList(struct list*);
void checkFileExt(char *, const char *);
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
    int mainChoice, secondChoice; 
    int flag, saved = 0;
    struct list *head = NULL;

    while (1) {
        system("cls");   // ← очистка перед меню
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
        printf("0. Обработка таблицы\n");
        printf("ESC. Выход из программы\n");
        printf("Выберите действие -> \n");
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
                    printf("Список пуст!\nРекомендуемое действие: 1.");
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
                flag = freeMemory(head, secondChoice);
                if (flag == -1) printf("Ошибка при удалении!\n");
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
                // TODO: сортировка
                printf("Функция в разработке.\n");
                getch();
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
                    printf("5. По числу статей\n6. По числу цитирований\n7. По индексу Хирша\n0. В меню\n");
                    printf("Выберите действие (0-7) -> ");
                    secondChoice = getch();
                    if (secondChoice == '0') flag = 0;
                    else findScientist(head, secondChoice);
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
                            printf("\nНеверный выбор! Нажмите любую клавишу...");
                            getch();
                    }
                }
                saved = 1;
                break;

            case '9':
                if (head != NULL && saved == 0) {
                    char word[10];
                    system("cls");
                    printf("[!] Внимание! Есть несохранённые данные.\nВведите 'yes' для перезаписи: ");
                    scanf("%s", word);
                    if (strcmp(word, "yes") != 0) break;
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
                            printf("Импорт завершён.\n");
                            flag = 0;
                            break;
                        case '2':
                            head = importFromBin(head);
                            printf("Импорт завершён.\n");
                            flag = 0;
                            break;
                        case '0':
                            flag = 0;
                            break;
                        default:
                            printf("\nНеверный выбор! Нажмите любую клавишу...");
                            getch();
                    }
                }
                saved = 0;
                break;

            case '0':
                break;

            case 27:
                int count = freeMemory(head, 0);
                printf("\nУдалено %d записей. Выход...\n", count);
                return 0;

            default:
                printf("\nКоманда не распознана! Нажмите любую клавишу...");
                getch();
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
struct list *createListFromKeyboard(struct list *head) {
    int choice = 0;
    do {
        if (head == NULL) head = addFirst(head, readData());
        else addLast(head, readData());

        printf("Ввести ещё? \n1. да \n0. нет \n-> ");
        scanf("%d", &choice);
    } while (choice);
    return head;
}

/// @brief Функция вывода таблицы со скроллингом
/// @param head - Указатель на начало списка
void viewList(struct list *head) {
    if (head == NULL) {
        printf("Список не введён!\n");
        return;
    }

    int total = 0;
    struct list *cur = head;
    while (cur) {
        total++;
        cur = cur->next;
    }

    const int PAGE_SIZE = 15;
    int start = 0;
    char key;

    do {
        system("cls");

        printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
        printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
        printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");

        struct list *temp = head;
        int idx = 0;
        while (temp && idx < start) {
            temp = temp->next;
            idx++;
        }

        int printed = 0;
        while (temp != NULL && printed < PAGE_SIZE) {
            printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
                   temp->info.id,
                   temp->info.name,
                   temp->info.degree,
                   temp->info.area,
                   temp->info.articles,
                   temp->info.quotes,
                   temp->info.hirshIndex);
            printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
            temp = temp->next;
            printed++;
        }

        if (printed == 0) break;

        int first = start + 1;
        int last = (start + printed < total) ? start + printed : total;
        printf("\nСтраница: %d-%d из %d. Используйте W (вверх), S (вниз), Q (выход): ", first, last, total);

        key = getch();

        if (key == 'w' || key == 'W') {
            int new_start = start - PAGE_SIZE;
            if (new_start < 0) new_start = 0;
            start = new_start;
        }
        else if (key == 's' || key == 'S') {
            int new_start = start + PAGE_SIZE;
            int max_start = total - PAGE_SIZE;
            if (max_start < 0) max_start = 0;
            if (new_start > max_start) new_start = max_start;
            start = new_start;
        }
        else if (key == 'q' || key == 'Q' || key == 27) {
            break;
        }
    } while (1);
}

/// @brief Проверяет и добавляет расширение файла, если его нет
void checkFileExt(char *filename, const char *ext) {
    int len = strlen(filename);
    if (len < 4 || strcmp(filename + len - 4, ext) != 0) {
        strcat(filename, ext);
    }
}

/// @brief Функция экспорта таблицы в текстовый (`.txt`) файл
/// @param head Указатель на начало списка
void exportToTxt(struct list *head) {
    char fileName[101];
    struct list *temp = head;
    
    printf("\nВведите название файла -> ");
    int c;
    fflush(stdin);
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    
    checkFileExt(fileName, ".txt");
    
    FILE *file = fopen(fileName, "wt");
    if (file == NULL) {
        printf("Ошибка создания файла '%s'!\n", fileName);
        return;
    }
    
    for (; temp != NULL; temp = temp->next) {
        fprintf(file, "%d\t%s\t%s\t%s\t%d\t%d\t%d\n",
                temp->info.id,
                temp->info.name,
                temp->info.degree,
                temp->info.area,
                temp->info.articles,
                temp->info.quotes,
                temp->info.hirshIndex);
    }
    
    printf("Информация записана в файл '%s'\n", fileName);
    fclose(file);
}

/// @brief Функция экспорта таблицы в `.bin` файл
/// @param head Указатель на начало списка
void exportToBin(struct list *head) {
    char fileName[101];
    printf("\nВведите название файла -> ");
    int c;
    fflush(stdin);
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    
    checkFileExt(fileName, ".bin");
    
    FILE *file = fopen(fileName, "wb");
    if (file == NULL) {
        printf("Ошибка создания файла '%s'!\n", fileName);
        return;
    }
    
    for (struct list *temp = head; temp != NULL; temp = temp->next) {
        fwrite(&temp->info, sizeof(struct scientist), 1, file);
    }
    
    printf("Таблица успешно экспортирована в файл '%s'\n", fileName);
    fclose(file);
}

/// @brief Функция импорта таблицы из текстового (`.txt`) файла
/// @param head Указатель на начало списка
/// @return Указатель на начало списка
struct list *importFromTxt(struct list *head) {
    char fileName[101];
    struct scientist data;
    
    printf("\nВведите название файла -> ");
    int c;
    fflush(stdin);
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    
    checkFileExt(fileName, ".txt");
    
    FILE *file = fopen(fileName, "rt");
    if (file == NULL) {
        printf("Файл '%s' не найден!\n", fileName);
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
    return head;
}

/// @brief Функция импорта таблицы из бинарного (`.bin`) файла
/// @param head Указатель на начало списка
/// @return Указатель на начало списка
struct list *importFromBin(struct list *head) {
    char fileName[101];
    struct scientist data;
    
    printf("\nВведите название файла -> ");
    int c;
    fflush(stdin);
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    
    checkFileExt(fileName, ".bin");
    
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("Файл '%s' не найден!\n", fileName);
        return head;
    }
    
    while (fread(&data, sizeof(struct scientist), 1, file) == 1) {
        if (head == NULL)
            head = addFirst(head, data);
        else
            head = addLast(head, data);
    }
    
    fclose(file);
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

        case 1:
            temp = head;
            head = head->next;
            head->prev = NULL;
            free(temp);
            temp = NULL;
            return 1;
        
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
                fgets(temp->info.degree, 75, stdin);
                printf("Учёная степень успешно изменена!\n");
                printf("%s  ->  %s\n", oldLine, temp->info.degree);
                break;

            case '3':
                printf("Введите Область наук -> ");
                strcpy(oldLine, temp->info.area);
                fgets(temp->info.area, 75, stdin);
                printf("Учёная степень успешно изменена!\n");
                printf("%s  ->  %s\n", oldLine, temp->info.area);
                break;

            case '4':
                printf("Введите Кол-во статей -> ");
                oldInt = temp->info.articles;
                scanf("%d", &temp->info.articles);
                printf("Кол-во статей успешно изменено!\n");
                printf("%d  ->  %d\n", oldInt, temp->info.articles);
                break;

            case '5':
                printf("Введите Кол-во цитирований -> ");
                oldInt = temp->info.quotes;
                scanf("%d", &temp->info.quotes);
                printf("Кол-во цитирований успешно изменено!\n");
                printf("%d  ->  %d\n", oldInt, temp->info.quotes);
                break;

            case '6':
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
        case '1':
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
        
        case '2':
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
        
        case '3':
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
    
        case '4':
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

        case '5':
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

        case '6':
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

        case '7':
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