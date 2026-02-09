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
void listOutput(struct list *s);
void fmemory(struct list *s);

int main() {
    system("chcp 1251 > nul");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    system("chcp 1251");
    setlocale(LC_ALL, "Russian");

    int choice;
    struct list *s, *e, *t;
    s = e = NULL;
    struct scientist data;
    while (1) {
        printf("======= МЕНЮ =======\n");
        printf("1. Создать список учёных\n");
        printf("2. Ввести информацию об учёном в начало\n");
        printf("3. Ввести информацию об учёном в конец\n");
        printf("4. Удалить учёного из списка\n");
        printf("5. Вывести список всех учёных\n");
        printf("0. Выход из программы\n");
        printf("-> ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 0:
                fmemory(s);
                printf("Выход из программы...");
                return 0;
            
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
                break;

            case 5:
                listOutput(s);
                break;
            
            default:
                printf("Команда не распознана!\n");
        }
    }
}

struct scientist readData() {
    struct scientist data;
    printf("Введите ID: "); scanf("%d", &data.id);
    fflush(stdin);

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
    struct list *t;
    t = (struct list*)malloc(sizeof(struct list));
    t->info = data;
    t->next = s;
    s = t;
    return s;
}

struct list *addLast(struct list *s, struct scientist data) {
    struct list *t, *e;
    t = (struct list*)malloc(sizeof(struct list));
    t->info = data;
    t->next = NULL;
    if (s == NULL) s = t;
    else {
        for (e = s; e->next != NULL; e = e->next);
        e->next = t;
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
        printf("Ввести ещё? \n1. да \n0. нет \n->");
        scanf("%d", &choice);
    } while (choice);
    return s;
}

struct list *deleteScientist(struct list *s) {
    int id;
    printf("Введите ID учёного, которого хотите удалить: "); scanf("%d", &id);
    struct list* current = s;
    struct list* previous = NULL;
    while (current != NULL && current->info.id != id) {
        previous = current;
        current = current->next;
    }
    if (current == NULL) return s;
    if (previous == NULL) {
        s = current->next;
    } else {
        previous->next = current->next;
    }
    free(current);
    return s;
}

void listOutput(struct list *s) {
    if (s == NULL) {
        printf("Список пуст!\n");
        return;
    }
    
    struct list *t;
    t = s;
    printf("+------+----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    printf("|  ID  |                            Фамилия Имя Отчество                            |       Учёная степень      |        Область науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
    printf("+------+----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    while (t != NULL) {
        printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
          t->info.id,
          t->info.name,
          t->info.degree,
          t->info.area,
          t->info.articles,
          t->info.quote,
          t->info.hirshIndex
        );
        t = t->next;
    }
    printf("+------+----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
}

void fmemory(struct list *s) {
    int c=0;
    if (s == NULL) {
        printf("Список пуст...\n");
        return;
    }
    else {
        struct list *t, *n;
        t = s;
        n = t->next;
        while (t != NULL) {
            n = t->next;
            free(t);
            t = n;
            c++;
        }
        printf("Было удалено %d записей...\nСписок пуст...\n", c);
    }
}