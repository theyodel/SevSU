#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void outputStr(char str);
void replaceDots(char str);
int isSpaces(char str);
void afterQuestion(char str);

int main() {
    int choice;
    char *str=NULL;
    while (1) {
        printf("\n======= М Е Н Ю =======\n");
        printf("1. Ввести строку\n");
        printf("2. Вывести строку\n");
        printf("3. Заменить все многоточия (...) на одну точку\n");
        printf("4. Проверить строку на наличие пробелов\n");
        printf("5. Вывести на экран подстроку после знака \"?\"\n");
        printf("9. Выход\n");
        printf("-> ");
        scanf("%d", &choice);
        printf("\n");
        switch (choice) {
            case 1:
                if (str != NULL) free(str);
                while (getchar() != "\n");
                gets(str);
                break;

            case 2:
                if (str == NULL) {
                    printf("Строка не введена!\n");
                }
                else {
                    outputStr(str);
                }
                break;
            
            case 3:
                if (str == NULL) {
                    printf("Строка не введена!\n");
                }
                else {
                    replaceDots(str);
                }
                break;
            
            case 4:
                if (str == NULL) {
                    printf("Строка не введена!\n");
                }
                else {
                    if (isSpaces(str) != 0) {
                        printf("В строке есть %d пробелов\n", isSpaces(str));
                    }
                    else {
                        printf("В строке не найдены пробелы\n");
                    }
                }
                break;
            
            case 5:
                if (str == NULL) {
                    printf("Строка не введена!\n");          
                }
                else {
                    afterQuestion(str);
                }
                break;

            case 9:
                if (str != NULL) free(str);
                printf("Выход из программы. . .\n");
                return 0;

            default:
                printf("Неверный выбор! Попробуйте снова.\n");
                break;
        }
    }
}

void outputStr(char str) {
    printf("Введённая строка: \n%s\nРазмер = %d\n", str, strlen(str));
}
