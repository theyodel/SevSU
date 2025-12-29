#include <stdio.h>
#include <math.h>
#include <locale.h>

int main() {
    int N;
    printf("Введите размер массива N: ");
    scanf("%d \n", &N);
    float mas[N], c;

    // Проверка введённых данных
    if (N <= 0)
    {
        printf("Число элементов (N) не может быть <= 0");
        return 1;
    }
    
    // Ввод значений в массив
    for (int i = 0; i < N; i++) {
        scanf("%f", &mas[i]);
    }

    // 1) Поиск максимального элемента массива
    float max = mas[0];
    for (int i = 1; i < N; i++) {
        if (mas[i] > max) {
            max = mas[i];
        }
    }
    printf("Максимальный элемент: %.2f\n", max);

    // 2) Сумма элементов до последнего позитивного
    int last_positive = -1;
    for (int i = N - 1; i >= 0; i--) {
        if (mas[i] >= 0) {
            last_positive = i;
            break;
        }
    }
    float sum = 0;
    if (last_positive != -1) {
        for (int i = 0; i < last_positive; i++) {
            sum += mas[i];
        }
        printf("Сумма элементов до последнего положительного: %.2f\n", sum);
    } else {
        printf("Положительных элементов нет. Сумма: 0\n");
    }

    // 3) Замена всех элементов, модуль которых находится внутри отрезка [а, b]
    float a, b;
    printf("Введите значение A и B: "); 
    scanf("%f %f", &a, &b);
    if (a > b)
    {
        printf("Значение A не может быть больше значения B");
        return 1;
    }
    for (int i = 0; i < N; i++)
    {   
        float module = fabs(mas[i]);
        if (a <= module && module <= b)
        {
            mas[i] = a;
        }
    }
    printf("Сумма элементов массива, модуль которых входит в отрезок [%.2f; %.2f]: %.2f \n", a, b, sum);

    // 4) Сортировка массива методом вставки:
    float temp;
    int i, j;
    for(i=1; i<N; i++) {
        temp=mas[i];
        for(j=i-1;(j>=0)&&(mas[j]>temp);j--) {
            mas[j+1]=mas[j];
        }
        mas[j+1]=temp;
    }  
    printf("Отсортированный массив после замены:\n");
    for (int i = 0; i < N; i++) {
        printf("%.2f; ", mas[i]);
    }

    return 0;
}