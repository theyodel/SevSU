#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

char *readString();
void outputString(char *s);
// char replaceStars();
// void outputBeforeSpace(char *s);
int isByeEnd(char *s);

int main() {
    // SetConsoleCP(65001);
    // SetConsoleOutputCP(65001);

    int choice;
    char *s = NULL;
    while (1) {
        printf("\n================ МЕНЮ ================\n");
        printf("1. Записать новую строку\n");
        printf("2. Вывести текущую строку\n");
        printf("3. Проверить окончание строки на \"Bye\"\n");
        printf("4. Заменить все \"***\" на \"/\"\n");
        printf("5. Вывести подстроку до первого символа \' \'\n");
        printf("0. Выход из программы\n");
        printf("-> "); 
        fflush(stdin);
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                if (s != NULL) free(s);
                s = readString();
                break;

            case 2:
                if (s == NULL) printf("Строка не введена!\n");
                else outputString(s);
                break;

            case 3:
                if (s == NULL) printf("Строка не введена!\n");
                else {
                    if (isByeEnd(s)) {
                        printf("Текущая строка заканчивается на \"Bye\"\n");
                    }
                    else {
                        printf("Текущая строка НЕ заканчивается на \"Bye\"\n");
                    }
                }
                break;

            case 4:
                break;

            case 5:
                // if (s == NULL) printf("Строка не введена!\n");
                // else outputBeforeSpace(s);
                break;

            case 0:
                if (s != NULL) free(s);
                return 0; 

            default:
                printf("Команда не распознана!\n");
        }
    }
}

char *readString() {
    char temp[100];
    printf("Введите строку: "); 
    while (getchar() != '\n');
    gets(temp);
    char *s = (char*)malloc(strlen(temp)+1);
    strcpy(s, temp);
    return s;
}

void outputString(char *s) {
    printf("Введённая строка: %s\n", s);
    printf("Вес строки: %dБайт\n", strlen(s)+1);
}

int StringCompare(char *n, char *s) {
    char *Ftemp = (char*)calloc(4, 4);
    char *Stemp = (char*)calloc(4, 4);
    *Ftemp = n[strlen(n)-3]; *Stemp = s[strlen(s)-3];
    
    while (*Ftemp == *Stemp) {
        printf("%d | %d\n", *Ftemp, *Stemp);
        Ftemp++; Stemp++;
    }
    return *Ftemp-*Stemp;
}

// int StringCompare(char *n, char *s) {
//     if (*s==*n) return 0;
//     return *n-*s;
// }

int isByeEnd(char *s) {
    int flag = 0;
    printf("%d", StringCompare(s, "Bye"));
    if (StringCompare(s, "Bye")==0) flag = 1;
    return flag;
}

// void outputBeforeSpace(char *s) {
//     char temp[strlen(s)+1];
//     int i = 0;
//     while (strcmp(s[i], ' ') != 0) {
//         strcat(temp, s[i]);
//         i++;
//     }
//     printf("Строка до первого знака \' \': %s\n", temp);
// }