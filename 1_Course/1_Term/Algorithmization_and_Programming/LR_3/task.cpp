#include <iostream>
#include <math.h>

using namespace std;

int main()
{
    int N;
    cout << "Введите размер массива N: ";
    cin >> N;
    float mas[N], c;

    // Проверка введённых данных
    if (N <= 0)
    {
        cout << "Число элементов (N) не может быть <= 0" << endl;
        return 1;
    }

    // Ввод значений в массив
    for (int i = 0; i < N; i++)
    {
        cin >> mas[i];
    }

    // 1) Поиск максимального элемента массива
    float max = mas[0];
    for (int i = 1; i < N; i++)
    {
        if (mas[i] > max)
        {
            max = mas[i];
        }
    }
    cout << "Максимальный элемент: " << max << endl;

    // 2) Сумма элементов до последнего позитивного
    int last_positive = -1;
    for (int i = N - 1; i >= 0; i--)
    {
        if (mas[i] >= 0)
        {
            last_positive = i;
        }
    }
    float sum = 0;
    if (last_positive != -1)
    {
        for (int i = 0; i < last_positive; i++)
        {
            sum += mas[i];
        }
        cout << "Сумма элементов до последнего положительного: " << sum << endl;
    }
    else
    {
        cout << "Положительных элементов нет. Сумма: 0" << endl;
    }

    // 3) Замена всех элементов, модуль которых находится внутри отрезка [а, b]
    float a, b;
    cout << "Введите значение A: " << endl;
    cin >> a;
    cout << "Введите значение B: " << endl;
    cin >> b;
    if (a > b)
    {
        cout << "Значение A не может быть больше значения B" << endl;
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

    // 4) Сортировка массива методом вставки:
    float temp;
    int i, j;
    for (i = 1; i < N; i++)
    {
        temp = mas[i];
        for (j = i - 1; (j >= 0) && (mas[j] > temp); j--)
        {
            mas[j + 1] = mas[j];
        }
        mas[j + 1] = temp;
    }
    cout << "Отсортированный массив после замены:" << endl;
    for (int i = 0; i < N; i++)
    {
        cout << mas[i] << ";  ";
    }

    return 0;
}