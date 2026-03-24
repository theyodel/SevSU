#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

char *readString();
void outputString(char *s);
void replaceStars(char *s);
void outputBeforeSpace(char *s);
int isByeEnd(char *s);

void main() {
    int choice;
    char *s = NULL;
    while (1) {
        printf("\n================ МЕНЮ ================\n");
        printf("1. Записать новую строку\n");
        printf("2. Вывести текущую строку\n");
        printf("3. Проверить окончание строки на \"Bye\"\n");
        printf("4. Заменить все \"***\" на \"/\"\n");
        printf("5. Вывести подстроку до первого символа \" \"\n");
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
                if (s == NULL) printf("Строка не введена!\n");
                else replaceStars(s);
                break;

            case 5:
                if (s == NULL) printf("Строка не введена!\n");
                else outputBeforeSpace(s);
                break;

            case 0:
                if (s != NULL) free(s);
                return; 

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
    char *s = (char*)calloc(strlen(temp)+1, 1);
    strcpy(s, temp);
    return s;
}

void outputString(char *s) {
    printf("Введённая строка: %s\n", s);
    printf("Вес строки: %d Байт\n", strlen(s)+1);
}

int isByeEnd(char *s) {
    if (strlen(s) < 3) return 0;
    if (strcmp(&s[strlen(s)-3] , "Bye")==0) return 1;
    return 0;
}

/*!
	\brief Функция замены "***" на "/"
	\author yodel
	\version 1.0
	\date 23 Марта 2026
	\warning Это моя функция!

	Бля круто, теперь буду этим 24/7 пользоваться ыыы
*/
void replaceStars(char *s) {
    if (s == NULL) return;
    char *pos;
    while ((pos = strstr(s, "***")) != NULL) {
        *pos = '/';
        memmove(pos + 1, pos + 3, strlen(pos + 3) + 1);
    }
    printf("Полученная строка: %s\n", s);
    printf("Вес строки: %dБайт", strlen(s)+1);
}

void outputBeforeSpace(char *s) {
    int b = strcspn(s, " ");
    for (int i=0; i<b; i++) {
        printf("%c", s[i]);
    }
    printf("\n");
}