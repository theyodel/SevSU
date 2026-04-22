#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct scientist {
    char name[76];
    char area[26];
    char degree[26];
    int quotes;
    int articles;
    int hirshIndex;
};

struct scientist readData();
FILE *createBinFile(char *, struct scientist);

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    FILE *file;
    int choice;
    while (1)
    {
        printf("\n\n\n================ МЕНЮ ================\n");
        printf("1. Запись файла\n");
        printf("2. Сортировка по полю \"Кол-во цитирований\"\n");
        printf("3. Вывести учёного с наибольшим индексом Хирша\n");
        printf("4. Вывести таблицу на экран\n");
        printf("0. Выход\n-> ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            char fileName[100];
            printf("Введите название файла (без расширения .bin): ");
            scanf("%s", fileName);
            strcat(fileName, ".bin");
            file = createBinFile(fileName, readData());
            break;
        
        case 2:
            
            break;
        
        case 0:
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

FILE *createBinFile(char *fileName, struct scientist data) {
    FILE *file = open(fileName, "wb");
    
}