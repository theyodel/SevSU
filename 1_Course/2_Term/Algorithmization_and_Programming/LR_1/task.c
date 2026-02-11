#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

struct scientist {
    int id;
    char name[76];
    char area[26];
    char degree[26];
    int articles;
    int quote;
    int hirshIndex;
};

struct list {
    struct scientist info;
    struct list* next;
};

struct scientist readData();
struct list *createList(struct list *s);
struct list *addFirst(struct list *s, struct scientist d);
struct list *addLast(struct list *s, struct scientist d);
struct list *deleteScientist(struct list *s);
struct list *deleteScndThrdScientist(struct list *s);
void listOutput(struct list *s);
void fmemory(struct list *s);

int main() {
    SetConsoleCP(1125);
    SetConsoleOutputCP(1125);
    setlocale(LC_ALL, "RU");

    int choice;
    struct list *s, *e, *temp;
    s = e = NULL;
    struct scientist data;
    while (1) {
        printf("\n================ МЕНЮ ================\n");
        printf("1. Создать список учёных\n");
        printf("2. Ввести информацию об учёном в начало\n");
        printf("3. Ввести информацию об учёном в конец\n");
        printf("4. Удалить учёного из списка\n");
        printf("5. Вывести список всех учёных\n");
        printf("6. Удалить 2 и 3 запись\n");
        printf("0. Выход из программы\n");
        printf("-> ");
        scanf("%d", &choice);
        
        switch (choice) {  
            case 1:
                s = createList(s);
                break;

            case 2:
                data = readData();
                s = addFirst(s, data);
                break;

            case 3:
                data = readData();
                s = addLast(s, data);
                break;

            case 4:
                s = deleteScientist(s);
                break;

            case 5:
                listOutput(s);
                break;

            case 6:
                s = deleteScndThrdScientist(s);
                break;
            
            case 0:
                fmemory(s);
                printf("Выход из программы...");
                return 0;

            default:
                printf("Команда не распознана!\n");
        }
    }
}

struct scientist readData() {
    struct scientist data;
    static int nextID = 1;
    data.id = nextID++;
    while (getchar() != '\n');
    printf("Введите ФИО: ");
    gets(data.name);
    data.name[strcspn(data.name, "\n")] = '\0';  // Удаляем символ новой строки
    
    printf("Введите научную область: ");
    gets(data.area);
    data.area[strcspn(data.area, "\n")] = '\0';

    printf("Введите учёную степень: ");
    gets(data.degree);
    data.degree[strcspn(data.degree, "\n")] = '\0';

    printf("Введите количество научных статей: "); scanf("%d", &data.articles);
    printf("Введите количество цитирований: "); scanf("%d", &data.quote);
    printf("Введите индекс Хирша: "); scanf("%d", &data.hirshIndex);
    printf("\n");
    return data;
}

struct list *addFirst(struct list *s, struct scientist data) {
    struct list *temp;
    temp = (struct list*)malloc(sizeof(struct list));
    temp->info = data;
    temp->next = s;
    s = temp;
    return s;
}

struct list *addLast(struct list *s, struct scientist data) {
    struct list *temp, *e;
    temp = (struct list*)malloc(sizeof(struct list));
    temp->info = data;
    temp->next = NULL;
    if (s == NULL) s = temp;
    else {
        for (e = s; e->next != NULL; e = e->next);
        e->next = temp;
    }
    return s;
}

struct list *createList(struct list *s) {
    struct scientist data;
    int choice = 0;
    do {
        if (s == NULL) {
            data = readData();
            s = addFirst(s, data);
        }
        else {
            data = readData();
            addLast(s, data);
        }
        printf("Ввести ещё? \n1. да \n0. нет \n-> ");
        scanf("%d", &choice);
    } while (choice);
    return s;
}

struct list *deleteScientist(struct list *s) {
    int id;
    printf("Введите ID учёного, которого хотите удалить: "); scanf("%d", &id);
    struct list *temp = s;
    struct list *prev = NULL;
    while (temp != NULL && temp->info.id != id) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return s;
    if (prev == NULL) {
        s = temp->next;
    } else {
        prev->next = temp->next;
    }
    free(temp);
    return s;
}

struct list *deleteScndThrdScientist(struct list *s) {
    if (s == NULL || s->next == NULL || s->next->next == NULL) {
        printf("В списке меньше трёх учёных! Удаление невозможно.\n");
        return s;
    }
    struct list *second = s->next;
    struct list *third  = second->next;
    s->next = third->next;
    free(second);
    free(third);
    return s;
}

void listOutput(struct list *s) {
    if (s == NULL) {
        printf("Список пуст!\n");
        return;
    }
    
    struct list *temp;
    temp = s;
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
          temp->info.quote,
          temp->info.hirshIndex
        );
        temp = temp->next;
        printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    }
}

void fmemory(struct list *s) {
    int c=0;
    if (s == NULL) {
        printf("Список пуст...\n");
        return;
    }
    else {
        struct list *temp, *n;
        temp = s;
        n = temp->next;
        while (temp != NULL) {
            n = temp->next;
            free(temp);
            temp = n;
            c++;
        }
        printf("Было удалено %d записей...\nСписок пуст...\n", c);
    }
}