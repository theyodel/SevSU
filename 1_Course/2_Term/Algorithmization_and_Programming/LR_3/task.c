#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countDots(char *fileName);
void copyWithUppercase(char *fromFile, char *toFile);

/// @brief главная функция
int main() {
    int choice;
    while (1) {
        printf("\n\n\n================ МЕНЮ ================\n");
        printf("1. Подсчитать кол-во многоточий в файле\n");
        printf("2. Скопировать содержимое одного файла в другой с заменой строчных гласных букв на заглавные\n");
        printf("0. Выход\n-> ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            char fileName[100];
            printf("Введите название файла: ");
            scanf("%s", fileName);
            if (fileName[strlen(fileName)-4]!='.' && fileName[strlen(fileName)-3]!='t' && fileName[strlen(fileName)-2]!='x' && fileName[strlen(fileName)-1]!='t') {
                strcat(fileName, ".txt");
            }
            int count = countDots(fileName);
            if (count != -1) printf("В файле %d многоточий\n", count);
            else printf("Произошла непредвиденная ошибка!\n");
            break;
        
        case 2:
            char fromFile[100], toFile[100];
            printf("Введите название исходного файла: ");
            scanf("%s", fromFile);
            if (fromFile[strlen(fromFile)-4]!='.' && fromFile[strlen(fromFile)-3]!='t' && fromFile[strlen(fromFile)-2]!='x' && fromFile[strlen(fromFile)-1]!='t') {
                strcat(fromFile, ".txt");
            }
            printf("Введите название копии файла: ");
            scanf("%s", toFile);
            if (toFile[strlen(toFile)-4]!='.' && toFile[strlen(toFile)-3]!='t' && toFile[strlen(toFile)-2]!='x' && toFile[strlen(toFile)-1]!='t') {
                strcat(toFile, ".txt");
            }
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

/// @brief Функция подсчёта многоточий в каждой строке файла и во всём файле в целом
/// @param fileName Имя файла, в котором будет производится подсчёт многоточий
/// @return Кол-во всех многоточий (`...`) в файле
/// @attention Имя файла должно передаваться в функцию с расширением `.txt`
int countDots(char *fileName) {
    int allDots = 0, line = 1, count = 0, flag = 0;
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Файл '%s' не найден!\n", fileName);
        return -1;
    }
    int chr;
    while ((chr = fgetc(file)) != EOF) {
        if (chr == '.') {
            flag++;
            if (flag == 3) {
                count++;
                flag = 0;
            }
        } else if (chr == '\n') {
            printf("Строка %d: найдено %d многоточий\n", line, count);
            allDots += count;
            count = 0;
            flag = 0;
            line++;
        } else {
            flag = 0;
        }
    }
    if (count > 0 || line == 1) {
        printf("Строка %d: найдено %d многоточий\n", line, count);
        allDots += count;
    }
    fclose(file);
    return allDots;
}

/// @brief Функция копирования содержимого одного текстового файла в другой с изменением регистра гласных букв согласно варианту задания (alAa -> AlAA)
/// @param fromFile Имя исходного файла
/// @param toFile Имя файла, куда будет скопирован исправленный текст исходного файла
/// @attention Имена файлов должны передаваться в функцию с расширением `.txt`
void copyWithUppercase(char *fromFile, char *toFile) {
    FILE *sourceFile, *copyFile;
    sourceFile = fopen(fromFile, "rt");
    copyFile = fopen(toFile, "wt");
    if (sourceFile == NULL || copyFile == NULL) {
        printf("Файл '%s' не найден!\n", fromFile);
        return;
    }
    int now = fgetc(sourceFile);
    while (now != EOF) {
        if (now == 'a' || now == 'e' || now == 'i' || now == 'o' || now == 'u' || now == 'y') now = now - 32;
        fprintf(copyFile, "%c", (char) now);
        now = fgetc(sourceFile);
    }
    fcloseall();
    printf("Копия файла '%s' с заменой прописных гласных букв на заглавные готова и помещена в '%s'!", fromFile, toFile);
}