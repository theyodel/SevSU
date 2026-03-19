#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TEMPORARY VARIABLES
int STEP = 2;
//

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
struct list *createList(struct list *s);
void viewList(struct list *b);
void freeMemory(struct list *s);

// Main
int main() {
    int choice;
    struct list *s;
    s = NULL;
    while (1) {
        printf("\n================ МЕНЮ ================\n");
        printf(":: Организация списка\n");
        printf("   1. Создать список учёных\n");

        printf(":: Вывод информации\n");
        printf("   2. Вывести таблицу всех учёных\n");

        printf("\n");
        printf("0. Выход из программы\n");
        printf("Выберите действие (0-2) -> ");
        scanf("%d", &choice);
        
        switch (choice) {  
            case 1:
                s = createList(s);
                break;

            case 2:
                if (s!=NULL) viewList(s);
                else printf("Список пуст!\nРекомендуемое действие: 1.");
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

// Функция вывода таблицы по 15 элементов на страницу, со скроллингом
void viewList(struct list *beg) {
    struct list *temp = beg;
    int page = 1;
    char act = ' ';
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    for (int c = 0; (temp->next != NULL) & c<STEP; c++, temp = temp->next) {
        printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
          temp->info.id,
          temp->info.name,
          temp->info.degree,
          temp->info.area,
          temp->info.articles,
          temp->info.quotes,
          temp->info.hirshIndex
        );
        printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    }

    again:
        printf("\nСтраница: %d\n", page);
        printf("\nДля навигации:\n`<` и `>`\nДля выхода:\n 0\t");
        scanf("%c", &act);
    if (act == 60) {
        printf("Страниц до 1 нет!");
        goto again;
    }

    while (act != '0') {
        switch (act) {
            case 60:
                if (temp->next == NULL) break;
                page++;
                printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
                printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
                printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
                for (int c = 0; (temp->next != NULL) & c<STEP; c++, temp = temp->next) {
                    printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
                      temp->info.id,
                      temp->info.name,
                      temp->info.degree,
                      temp->info.area,
                      temp->info.articles,
                      temp->info.quotes,
                      temp->info.hirshIndex
                    );
                    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
                }
                break;
            
            case 48:
                return;
            
            case 62:
                if (temp->prev == NULL) break;
                page--;
                for (int c=0; (temp->prev != NULL) & c<STEP; c++, temp = temp->prev); // Перемещаемся на 15 записей назад, чтобы не терять последовательность
                printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
                printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
                printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
                for (int c = 0; (temp->next != NULL) & c<STEP; c++, temp = temp->next) {
                    printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
                      temp->info.id,
                      temp->info.name,
                      temp->info.degree,
                      temp->info.area,
                      temp->info.articles,
                      temp->info.quotes,
                      temp->info.hirshIndex
                    );
                    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
                }
                break;
            
            default:
                printf("Командна не распознана!\n");
                break;
        }
        printf("\nСтраница: %d\n", page);
        printf("\nДля навигации:\n`<` и `>`\nДля выхода:\n 0\t");
        scanf("%c", &act);
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