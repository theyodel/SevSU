#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Funcs
int countDots(char *fileName);
void copyWithUppercase(char *fromFile, char *toFile);

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    int choice;
    while (1)
    {
        printf("\n\n\n================ МЕНЮ ================\n");
        printf("1. Подсчитать кол-во многоточий в файле\n");
        printf("2. Скопировать содержимое одного файла в другой с заменой строчных гласных букв на заглавные\n");
        printf("0. Выход\n-> ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            char fileName[100];
            printf("Введите название файла (без указания расширения .txt): ");
            scanf("%s", fileName);
            strcat(fileName, ".txt");
            printf("В файле %d многоточий\n", countDots(fileName));
            break;
        
        case 2:
            char fromFile[100], toFile[100];
            printf("Введите название исходного файла (без указания расширения .txt): ");
            scanf("%s", fromFile);
            printf("Введите название копии файла (без указания расширения .txt): ");
            scanf("%s", toFile);
            strcat(fromFile, ".txt");
            strcat(toFile, ".txt");
            copyWithUppercase(fromFile, toFile);
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

int countDots(char *fileName) {
    int count = 0;
    FILE *file = fopen(fileName, "rt");
    if (file == NULL) {
        printf("Файл '%s' не найден!\n", fileName);
    } else {
        char line[1024];
        while (fgets(line, sizeof(line), file)) {
            int len = strlen(line);
            for (int i = 0; i < len - 2; i++) {
                if (line[i] == '.' && line[i+1] == '.' && line[i+2] == '.') {
                    count++;
                }
            }
        }
    }
    fclose(file);
    return count;
}

void copyWithUppercase(char *fromFile, char *toFile) {
    FILE *sourceFile, *copyFile;
    sourceFile = fopen(fromFile, "rt");
    if (sourceFile == NULL) {
        printf("Файл '%s' не найден!\n", fromFile);
        return;
    }
    copyFile = fopen(toFile, "wt");
    if (copyFile == NULL) {
        printf("Ошибка открытия/создания файла '%s' (файл для копии)", toFile);
        return;
    }
    int now = fgetc(sourceFile);
    while (now != EOF) {
        if (now == 'a' || now == 'e' || now == 'i' || now == 'o' || now == 'u' || now == 'y') now = now - 32;
        fprintf(copyFile, "%c", (char) now);
        now = fgetc(sourceFile);
    }
    fcloseall();
    printf("Копия файла '%s' готова и помещена в '%s'!", fromFile, toFile);
}