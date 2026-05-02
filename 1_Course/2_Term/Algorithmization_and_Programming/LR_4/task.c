#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct scientist {
    char name[76], area[26], degree[26];
    int quotes, articles, hirshIndex, id;
};

struct list {
    struct scientist info;
    struct list *prev, *next;
};

struct scientist readData();
struct list *createListFromKeyboard(struct list *);
void createBinFile(struct list *);
int maxHirsh(struct list *);
void viewList(struct list *);
struct list *importFromBin(struct list *);
struct list *sortList(struct list *);
void freeMemory(struct list *);

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    int choice, flag;
    struct list *head = NULL;
    
    while (1) {
        printf("\n\n\n================ МЕНЮ ================\n");
        printf("1. Запись файла\n"); //
        printf("2. Сортировка по полю \"Кол-во цитирований\"\n");
        printf("3. Вывести учёного с наибольшим индексом Хирша\n"); //
        printf("4. Вывести таблицу на экран\n"); //
        printf("5, Чтение таблицы из файла\n"); //
        printf("0. Выход\n-> "); //
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            createBinFile(head);
            break;
        
        case 2:
            break;
        
        case 3:
            flag = maxHirsh(head);
            break;

        case 4:
            viewList(head);
            break;

        case 5:
            head = importFromBin(head);
            break;
        
        case 0:
            freeMemory(head);
            printf("\n\n\nВыход из программы...");
            return 0;

        default:
            printf("Команда не распознана!\n");
            break;
        }
    }
}

struct scientist readData() {
    struct scientist data;
    printf("\nВведите ФИО учёного: ");
    fgets(data.name, 75, stdin);
    printf("\nВведите научную область: ");
    fgets(data.area, 25, stdin);
    printf("\nВведите учёную степень: ");
    fgets(data.degree, 25, stdin);

    printf("\nВведите кол-во научных статей: ");
    scanf("%d", &data.articles);
    printf("\nВведите кол-во цитирований: ");
    scanf("%d", &data.quotes);
    printf("\nВведите индекс Хирша: ");
    scanf("%d", &data.hirshIndex);

    return data;
}

struct list *addFirst(struct list *head, struct scientist data) {
    struct list *temp;
    temp = (struct list*)malloc(sizeof(struct list));
    temp->info = data;
    temp->next = head;
    temp->prev = NULL;
    return temp;
}

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

struct list *createListFromKeyboard(struct list *head) {
    int choice = 0;
    do {
        if (head == NULL) head = addFirst(head, readData());
        else addLast(head, readData());

        printf("Ввести ещё? \n1. да \n0. нет \n-> ");
        choice = getch();
        if (choice == '0') choice = 0;
    } while (choice);
    return head;
}

void createBinFile(struct list *head) {
    char fileName[101];
    struct list *temp = head;
    printf("Введите название файла -> ");
    fflush(stdin);
    scanf("%s", fileName);
    if (fileName[strlen(fileName)-4]!='.' && fileName[strlen(fileName)-3]!='b' && fileName[strlen(fileName)-2]!='i' && fileName[strlen(fileName)-1]!='n') strcat(fileName, ".bin");
    FILE *file = fopen(fileName, "wbb");
    if (file == NULL) {
        printf("Файл '%s' будет создан в корне текущей папки.", fileName);
    }
    for (struct list *temp = head; temp!=NULL; temp=temp->next) {
        fwrite(&temp->info, sizeof(struct scientist), 1, file);
    }
    printf("Таблица успешно экспортирована в файл '%s'", fileName);
    fclose(file);
    return;
}

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

void viewList(struct list *head) {
    if (head == NULL) {
        printf("Список не введён!\n");
        return;
    }
    struct list *temp = head;
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

int maxHirsh(struct list *head) {
    if (head == NULL) {
        printf("Спиок пуст!\n");
        return 0;
    }
    struct list *temp = head;
    int maxHirsh = 0, id = -1;
    for (; temp != NULL; temp = temp->next) {
        if (temp->info.hirshIndex > maxHirsh) {
            maxHirsh = temp->info.hirshIndex;
            id = temp->info.id;
        }
    }
    printf("Учёный с максимальным индексом хирша:\n");
    for (; temp!=NULL && temp->info.id!=id; temp = temp->next);
    viewList(temp);
    return maxHirsh;
}

struct list *sortList(struct list *head) {
    struct list *all = head->next;
    struct scientist data = head->info;
    for (; all != NULL; all = all->next) {
        for (struct list *sorted = all->next; sorted != NULL; sorted = sorted->next) {
            if (sorted->info.quotes > all->info.quotes) {
                data = sorted->info;
                sorted->info = all->info;
                all->info = data;
            }
        }
    }
}

void freeMemory(struct list *head) {
    struct list *temp = head;
    for (; temp != NULL; temp = temp->next) {
        temp = head;
        head = temp->next;
        free(temp);
        temp = NULL;
    }
}