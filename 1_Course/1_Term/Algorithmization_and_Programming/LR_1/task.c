#include <stdio.h>
#include <math.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Russian");
    float a, b, x, z;
    printf("Введите параметр a: ");
    scanf("%f", &a);
    printf("Введите параметр b: ");
    scanf("%f", &b);
    printf("Введите аргумент x: ");
    scanf("%f", &x);

    if (b < x && x < a) {
        printf("Аргумент x не может быть в диапазоне b < x < a");
    }

    else if (x <= a) {
        z = log(x) + sin(x);
        printf("Значение функции z: %e\n", z);
    }

    else if (a < x && x < b) { 
        z = log(x) + cos(x);
        printf("Значение функции z: %e\n", z);
    }

    else if (b <= x) {
        z = tan(x);
        printf("Значение функции z: %e\n", z);
    }
    return 0;
}