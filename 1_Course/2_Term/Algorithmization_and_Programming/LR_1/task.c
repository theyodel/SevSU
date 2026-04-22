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
    unsigned int articles;
    unsigned int quote;
    unsigned int hirshIndex;
};

struct list {
    struct scientist info;
    struct list *next;
};

struct scientist readData();
struct list *createList(struct list *s);
struct list *addFirst(struct list *s, struct scientist d);
struct list *addLast(struct list *s, struct scientist d);
struct list *deleteScientist(struct list *s);
struct list *deleteScndThrdScientist(struct list *s);
void listOutput(struct list *s);
void freeMemory(struct list *s);

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, "RU");

    int choice;
    struct list *s;
    s = NULL;
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
                s = addFirst(s, readData());
                break;

            case 3:
                s = addLast(s, readData());
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
                freeMemory(s);
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

struct list *createList(struct list *s) {
    int choice = 0;
    do {
        if (s == NULL) {
            s = addFirst(s, readData());
        }
        else {
            addLast(s, readData());
        }
        printf("Ввести ещё? \n1. да \n0. нет \n-> ");
        scanf("%d", &choice);
    } while (choice);
    return s;
}

struct list *addFirst(struct list *s, struct scientist data) {
    struct list *temp = (struct list*)malloc(sizeof(struct list));
    temp->info = data;
    temp->next = s;
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
        e->next = temp;
    }
    return s;
}

struct list *deleteScientist(struct list *s) {
    struct list *temp = s, *prev = NULL;
    int id;
    if (temp == NULL) return s;
    printf("Введите ID учёного, которого хотите удалить: ");
    scanf("%d", &id);
    while (temp != NULL && temp->info.id != id) {
        prev = temp;
        temp = temp->next;
    }
    if (prev == NULL) {
        s = temp->next;
    } 
    else {
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
        printf("Список не введён!\n");
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
    printf("Было удалено %d записей...\nСписок пуст...\n", c);
}
