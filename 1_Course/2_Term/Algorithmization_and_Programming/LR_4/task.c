#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct scientist {
    char name[76], area[26], degree[26];
    int quotes, articles, hirshIndex, id;
};


struct scientist readData();
void createBinFile();
int maxHirsh();
void readFile();
void sortInFile();

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    int choice;
    
    while (1) {
        printf("\n\n\n================ МЕНЮ ================\n");
        printf("1. Запись файла\n"); //
        printf("2. Сортировка по полю \"Кол-во цитирований\"\n");
        printf("3. Вывести учёного с наибольшим индексом Хирша\n"); //
        printf("4. Вывести таблицу из файла на экран\n"); //
        printf("0. Выход\n-> "); //
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            createBinFile();
            break;
        
        case 2:
            break;
        
        case 3:
            maxHirsh();
            break;

        case 4:
            readFile();
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
    static int nextID = 1;
    data.id = nextID++;
    fflush(stdin);
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

void createBinFile() {
    char fileName[101];
    printf("Введите название файла -> ");
    fflush(stdin);
    fgets(fileName, 100, stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    if (fileName[strlen(fileName)-4]!='.' && fileName[strlen(fileName)-3]!='b' && fileName[strlen(fileName)-2]!='i' && fileName[strlen(fileName)-1]!='n') strcat(fileName, ".bin");
    FILE *file = fopen(fileName, "wb");
    if (file == NULL) {
        printf("Файл '%s' будет создан в корне текущей папки.", fileName);
    }
    int flag = 1;
    struct scientist data;
    while (flag) {
        printf("1. Ввести запись\n");
        printf("0. Сохранить файл\n");
        printf("-> ");
        scanf("%d", &flag);
        if (flag == 1) {
            data = readData();
            fwrite(&data, sizeof(struct scientist), 1, file);
        } else break;
    }
    printf("Таблица успешно экспортирована в файл '%s'", fileName);
    fclose(file);
    return;
}

void readFile() {
    char fileName[101];
    printf("Введите название файла -> ");
    fflush(stdin);
    fgets(fileName, 100, stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    if (fileName[strlen(fileName)-4]!='.' && fileName[strlen(fileName)-3]!='b' && fileName[strlen(fileName)-2]!='i' && fileName[strlen(fileName)-1]!='n') strcat(fileName, ".bin");
    FILE *file = fopen(fileName, "rb");
    struct scientist data;

    if (file == NULL) {
        printf("Файл '%s' не найден!\n", fileName);
        return;
    }
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    while (fread(&data, sizeof(struct scientist), 1, file) == 1) {
        printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
            data.id,
            data.name,
            data.degree,
            data.area,
            data.articles,
            data.quotes,
            data.hirshIndex);
        printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    }
}

int maxHirsh() {
    char fileName[101];
    printf("Введите название файла -> ");
    fflush(stdin);
    fgets(fileName, 100, stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    if (fileName[strlen(fileName)-4]!='.' && fileName[strlen(fileName)-3]!='b' && fileName[strlen(fileName)-2]!='i' && fileName[strlen(fileName)-1]!='n') strcat(fileName, ".bin");
    FILE *file = fopen(fileName, "rb");
    struct scientist data, max;
    int maxHirsh = -1;

    if (file == NULL) {
        printf("Файл '%s' не найден!", fileName);
        return -1;
    }

    while (fread(&data, sizeof(struct scientist), 1, file) == 1) {
        if (data.hirshIndex > maxHirsh) {
            maxHirsh = data.hirshIndex;
            max = data;
        }
    }
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
            max.id,
            max.name,
            max.degree,
            max.area,
            max.articles,
            max.quotes,
            max.hirshIndex);
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    return maxHirsh;
}

void sortInFile() {
    long i,j;
    char fileName[101];
    struct scientist f, s;
    printf("Введите название файла: ");
    fgets(fileName, 100, stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    if (fileName[strlen(fileName)-4]!='.' && fileName[strlen(fileName)-3]!='b' && fileName[strlen(fileName)-2]!='i' && fileName[strlen(fileName)-1]!='n') strcat(fileName, ".bin");
    FILE *file = fopen(fileName, "wb+");
    long len=ftell(file)/sizeof(struct scientist), elementSize = sizeof(struct scientist);
    for(i=len-1; i>=1; i--) {
        for (j=0; j<=i-1; j++) {
            fseek(file,j*elementSize, SEEK_SET);
            fread(&f, elementSize, 1, file);
            fread(&s, elementSize, 1, file);
            if (f.quotes > s.quotes) {
                fseek(file, (-2)*elementSize, SEEK_CUR);
                fwrite(&s, elementSize, 1, file);
                fwrite(&f, elementSize, 1, file);
            }
        }
    }
    printf("Сортировка успешна завершена");
    return;
}