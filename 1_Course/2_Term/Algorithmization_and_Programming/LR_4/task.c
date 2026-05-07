#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

struct scientist {
    char name[76], area[26], degree[26];
    int quotes, articles, hirshIndex, id;
};

struct scientist readData();
void checkFileExt(char *);
void createBinFile();
void maxHirsh();
void readFile();
void sortInFile();

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    int choice;
    
    while (1) {
        printf("\n\n\n================ МЕНЮ ================\n");
        printf("1. Запись файла\n");
        printf("2. Сортировка по полю \"Кол-во цитирований\"\n");
        printf("3. Вывести учёного с наибольшим индексом Хирша\n");
        printf("4. Вывести таблицу из файла на экран\n");
        printf("0. Выход\n-> ");
        scanf("%d", &choice);
        while (getchar() != '\n'); // очистка буфера после scanf

        switch (choice) {
        case 1:
            createBinFile();
            break;
        case 2:
            sortInFile();
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

void checkFileExt(char *fileName) {
    int len = strlen(fileName);
    if (len < 4 || strcmp(fileName + len - 4, ".bin") != 0) {
        strcat(fileName, ".bin");
    }
}

void createBinFile() {
    char fileName[101];
    printf("Введите название файла -> ");
    fflush(stdin);
    fgets(fileName, 100, stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    
    // Добавляем .bin, если нет расширения
    checkFileExt(fileName);
    
    FILE *file = fopen(fileName, "wb");
    if (file == NULL) {
        printf("Ошибка: не удалось создать файл '%s'!\n", fileName);
        return;
    }
    
    int choice = 1;
    struct scientist data;
    while (1) {
        printf("1. Ввести запись\n");
        printf("0. Сохранить файл\n");
        printf("-> ");
        scanf("%d", &choice);
        while (getchar() != '\n');
        switch (choice) {
        case 1:
            data = readData();
            fwrite(&data, sizeof(struct scientist), 1, file);
            break;

        case 0:
            printf("Таблица успешно экспортирована в файл '%s'", fileName);
            fclose(file);
            return;

        default:
            printf("Команда не распознана!\n");
        }
    }
}

void readFile() {
    char fileName[101];
    printf("Введите название файла -> ");
    fflush(stdin);
    fgets(fileName, 100, stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    
    checkFileExt(fileName);
    
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
            data.id, data.name, data.degree, data.area,
            data.articles, data.quotes, data.hirshIndex);
        printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    }
    fclose(file);
}

void maxHirsh() {
    char fileName[101];
    printf("Введите название файла -> ");
    fflush(stdin);
    fgets(fileName, 100, stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    
    checkFileExt(fileName);
    
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("Файл '%s' не найден!\n", fileName);
        return;
    }
    
    struct scientist data, max;
    int maxHirshVal = -1, recordsFound = 0;

    while (fread(&data, sizeof(struct scientist), 1, file) == 1) {
        if (data.hirshIndex > maxHirshVal) {
            maxHirshVal = data.hirshIndex;
            max = data;
            recordsFound = 1;
        }
    }

    fclose(file);

    if (!recordsFound) {
        printf("Файл '%s' не содержит записей.\n", fileName);
        return;
    }
    
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    printf("|  ID  |                             Фамилия Имя Отчество                            |       Учёная Степень      |        Область Науки      | Кол-во статей | Кол-во цитирований | Индекс Хирша |\n");
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    printf("| %-4d | %-75s | %-25s | %-25s | %-13d | %-18d | %-12d |\n",
            max.id, max.name, max.degree, max.area,
            max.articles, max.quotes, max.hirshIndex);
    printf("+------+-----------------------------------------------------------------------------+---------------------------+---------------------------+---------------+--------------------+--------------+\n");
    
}

void sortInFile() {
    char fileName[101];
    printf("Введите название файла: ");
    fflush(stdin);
    fgets(fileName, 100, stdin);
    fileName[strcspn(fileName, "\n")] = '\0';
    
    checkFileExt(fileName);
    
    FILE *file = fopen(fileName, "rb+");
    if (file == NULL) {
        printf("Файл '%s' не найден!\n", fileName);
        return;
    }
    
    // Определяем количество записей
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    long elementSize = sizeof(struct scientist);
    long len = fileSize / elementSize;
    
    if (len <= 1) {
        printf("Файл пуст или содержит только одну запись. Сортировка не нужна.\n");
        fclose(file);
        return;
    }
    
    // Пузырьковая сортировка
    struct scientist f, s;
    for (long i = len - 1; i >= 1; i--) {
        for (long j = 0; j <= i - 1; j++) {
            fseek(file, j * elementSize, SEEK_SET);
            if (fread(&f, elementSize, 1, file) != 1 || fread(&s, elementSize, 1, file) != 1) {
                printf("Ошибка чтения из файла.\n");
                fclose(file);
                return;
            }
            if (f.quotes < s.quotes) { // от большего к меньшему
                fseek(file, j * elementSize, SEEK_SET);
                fwrite(&s, elementSize, 1, file);
                fwrite(&f, elementSize, 1, file);
            }
        }
    }
    
    printf("Сортировка успешно завершена\n");
    fclose(file);
}