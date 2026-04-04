#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// TEMPORARY VARIABLES
int STEP = 2;

struct scientist {
    int id;
    char name[76];
    char area[26];
    char degree[26];
    unsigned int articles;
    unsigned int quotes;
    unsigned int hirshIndex;
};

struct list {
    struct scientist info;
    struct list *prev, *next;
};

// Functions

struct scientist readData();
struct list *createListKeyboard(struct list *s);
struct list *addFirst(struct list *s, struct scientist data);
struct list *addLast(struct list *s, struct scientist data);
// void viewList(struct list *b);
void exportToTxt(struct list *head);
// void exportToBin(struct list *head);
void freeMemory(struct list *s);

/// Main function
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

        printf("\n");
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
                                // head = addLast(head, readData());
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
struct list *createListKeyboard(struct list *s) {
    int choice = 0;
    do {
        if (s == NULL) s = addFirst(s, readData());
        
        else addLast(s, readData());

        printf("Ввести ещё? \n1. да \n0. нет \n-> ");
        scanf("%d", &choice);
    } while (choice);
    return s;
}

/// Функция вывода таблицы по `STEP` (global variable `int STEP`) элементов на страницу, со скроллингом


/// @brief Функция экспорта в .TXT файл
void exportToTxt(struct list *head) {
    int choice;
    static int version = 1;
    struct list *temp = head;
    while (1) {
        printf("\n\n\n================ Экспорт таблицы в текстовый файл ================\n");
        printf("1. Экспортировать в существующий файл\n");
        printf("2. Экспортировать в новый файл\n");
        printf("0. В меню\n->");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                char nameFile1[] = "";
                printf("Введите название файла (с указанием расширения .txt) -> ");
                fflush(stdin);
                scanf("%s", &nameFile1);
                FILE *firstFile = fopen(nameFile1, "wt");
                fprintf(firstFile, "+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
                fprintf(firstFile, "|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
                fprintf(firstFile, "+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");

                for (; temp != NULL; temp = temp->next) {
                    fprintf(firstFile, "| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
                             temp->info.id,
                             temp->info.name,
                             temp->info.degree,
                             temp->info.area,
                             temp->info.articles,
                             temp->info.quotes,
                             temp->info.hirshIndex);
                    fprintf(firstFile, "+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
                }
                printf("Информация записана в файл %s", nameFile1);
                fclose(firstFile);
                return;
            
            case 2:
                char textVersion[] = "";
                itoa(version, textVersion, 10);
                char nameFile2[] = "-SCIENTISTS-LIST.txt";
                strcat(textVersion, nameFile2);

                FILE *file = fopen(textVersion, "wt");
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
                    fprintf(file, "+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n", file);
                }
                printf("Информация записана в файл %s", textVersion);
                fclose(file);
                version++;
                return;
            
            case 0:
                return;
            
            default:
                printf("Команда не распознана!\n");
                break;
        }
    } 
}

void freeMemory(struct list *s) {
    if (s == NULL) {
        printf("Список пуст...\n");
        return;
    }
    struct list *temp=s, *n;
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