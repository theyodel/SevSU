#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>

// Создание структуры "route"
struct route {
    int number;
    int routeNumber;
    char startPoint[100];
    char endPoint[100];
    float price;
    int time;
};

// Объявление функций
struct route* scanRoutes(int n);
void printRoutes(struct route *arr, int n);
int countCircleRoutes(struct route *arr, int n);
void sortByPrice(struct route *arr, int n);

int main() {
    int choice, n = 0;
    struct route *arr=NULL;
    while (1) {
        printf("\n======= М Е Н Ю =======\n");
        printf("1. Ввести все маршруты\n");
        printf("2. Вывести все маршруты\n");
        printf("3. Найти количество круговых маршрутов\n");
        printf("4. Отсортировать маршруты по стоимости проезда\n");
        printf("9. Выход\n");
        printf("-> ");
        scanf("%d", &choice);
        printf("\n");

        switch(choice) {
            case 1:
                if (arr != NULL) {
                    free(arr);
                }
                n = 0;
                printf("Введите количество маршрутов: ");
                scanf("%d", &n);
                if (n <= 0) {
                    printf("Количество маршрутов должно быть > 0");
                    break;
                }
                arr = scanRoutes(n);
                break;
            
            case 2:
                if (arr == NULL) {
                    printf("Маршруты не введены!\n");
                } 
                else {
                    printf("Все маршруты:\n");
                    printRoutes(arr, n);
                }
                break;

            case 3:
                if (arr == NULL) {
                    printf("Маршруты не введены!\n");
                } 
                else {
                    printf("Количество круговых маршрутов: %d\n", countCircleRoutes(arr, n));
                }  
                break;
            
            case 4:
                if (arr == NULL) {
                    printf("Маршруты не введены!\n");
                } 
                else {
                    sortByPrice(arr, n);
                }
                break;
            
            case 9:
                free(arr);
                printf("\nВыход из программы . . .");
                return 0;

            default:
                printf("Неверный выбор! Попробуйте снова.");
                break;
        }
    }
}

struct route* scanRoutes(int n) {
    struct route *arr = (struct route*) malloc(n * sizeof(struct route));
    
    if (arr == NULL) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }
    
    printf("Введите %d маршрутов:\n", n);
    for (int i = 0; i < n; i++) {
       arr[i].number = i+1;
       printf("Введите номер маршрута: ");
       scanf("%d", &arr[i].routeNumber);
       while (getchar() != '\n');

       printf("Введите начальную точку маршрута (до 100 символов): ");
       gets(arr[i].startPoint); 

       printf("Введите конечную точку маршрута (до 100 символов): ");
       gets(arr[i].endPoint);

       printf("Введите цену маршрута: ");
       scanf("%f", &arr[i].price);

       printf("Введите время в пути (в минутах): ");
       scanf("%d", &arr[i].time);

       printf("----------------------------------------------\n");
   }
    
    return arr;
}

void printRoutes(struct route *arr, int n) {
    printf("----------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%d) Номер маршрута: %d\n", arr[i].number, arr[i].routeNumber);
        printf("Начальная точка: %s \nКонечная точка: %s\n", arr[i].startPoint, arr[i].endPoint);
        printf("Цена проезда: %.2f₽\n", arr[i].price);
        if (arr[i].time % 60 == 0) {
            printf("Время в пути: %dч\n", arr[i].time/60);
        }
        else if (arr[i].time < 60) {
            printf("Время в пути: %dмин\n", arr[i].time);
        }
        else {
            printf("Время в пути: %dч %dмин\n", arr[i].time/60, arr[i].time%60);
        }
        printf("----------------------------------------------\n");
    }

    printf("Всего маршрутов: %d\n", n);
}

int countCircleRoutes(struct route *arr, int n) {
    int count = 0;

    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].startPoint, arr[i].endPoint) == 0) {
            count++;
        }
    }

    return count;
}

void sortByPrice(struct route *arr, int n) {
    struct route temp, tarr[n];

    for (int i = 0; i < n; i++) {
        tarr[i] = arr[i];
    }

    for (int j = n; j > 1; j--) {
        for(int i = 0; i < j-1; i++) {
            if(tarr[i].price < tarr[i+1].price) {
                temp=tarr[i];
                tarr[i]=tarr[i+1];
                tarr[i+1]=temp;
            }
        }
    }

    printf("Отсортированный список маршрутов по полю \"Цена проезда\":\n");
    printRoutes(tarr, n);
}
