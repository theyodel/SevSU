#include <stdio.h>
#include <math.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Russian");
    float a, b, x_start, x_end, dx, x, z;
    
    // Ввод параметров
    printf("Введите параметр a: ");
    scanf("%f", &a);
    printf("Введите параметр b: ");
    scanf("%f", &b);
    printf("Введите начальное значение x: ");
    scanf("%f", &x);
    printf("Введите конечное значение x: ");
    scanf("%f", &x_end);
    printf("Введите шаг dx: ");
    scanf("%f", &dx);
    
    // if (dx <= 0) {
    //     printf("Ошибка: шаг dx должен быть положительным!\n");
    //     return 1;
    // }
    // if (cos(x) == 0) {
    //     printf("Ошибка: cos(x) = 0, что по условию не может быть!\n");
    //     return 1;
    // }
    // if (x <= 0) {
    //     printf("Ошибка: x = 0, что по условию не может быть!\n");
    //     return 1;
    // }
    if (x_start < x_end)
    {
        printf("Ошибка: x_start < x_end, что по условию не может быть!\n");
        return 1;
    }
    
    printf("\n");
    printf("________________________________\n");
    printf("|      x      |    z = f(x)    |\n");
    printf("|_____________|________________|\n");
    
    x = x_start;
    while (x <= x_end) {
        printf("| %9.4f |",x);
        if (x <= a) {
            z = log(x) + sin(x);
        }
        else if (a < x && x < b) {
            z = log(x) + cos(x);
        }
        else {
            z = tan(x);
        }
        printf(" %-11.4f |\n",z);
        x += dx;
        printf("|_____________|________________|\n");
    };
    return 0;
}