#include <iostream>
#include <math.h>

using namespace std;

// Объявление функций
int countCondition(int *arr, int n);
int *inputSequence(int *n);
void printSequence(int *arr, int n);
void partialSort(int *arr, int n);

int main()
{
    int *sequence = NULL;
    int n = 0;
    int choice;

    while (1)
    {
        cout << endl
             << "======= М Е Н Ю =======" << endl;
        cout << "1. Ввести новую последовательность" << endl;
        cout << "2. Вывести текущую последовательность" << endl;
        cout << "3. Найти количество элементов по условию" << endl;
        cout << "4. Отсортировать часть последовательности" << endl;
        cout << "9. Выход" << endl;
        cout << "-> ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            // Освобождаем старую память, если она была выделена
            if (sequence != NULL)
            {
                free(sequence);
            }

            sequence = inputSequence(&n);
            if (sequence != NULL)
            {
                cout << "Последовательность успешно введена." << endl;
            }
            break;

        case 2:
            if (sequence == NULL)
            {
                cout << "Последовательность не введена!" << endl;
            }
            else
            {
                printSequence(sequence, n);
            }
            break;

        case 3:
            if (sequence == NULL)
            {
                cout << "Последовательность не введена!" << endl;
            }
            else
            {
                int count = countCondition(sequence, n);
                cout << "Количество элементов, удовлетворяющих условию ai-1 < ai > ai+1: " << count << endl;
            }
            break;

        case 4:
            if (sequence == NULL)
            {
                cout << "Последовательность не введена!" << endl;
            }
            else
            {
                partialSort(sequence, n);
            }
            break;

        case 9:
            cout << "Выход из программы..." << endl;
            // Освобождаем память перед выходом
            if (sequence != NULL)
            {
                free(sequence);
            }
            return 0;
            break;

        default:
            cout << "Неверный выбор! Попробуйте снова." << endl;
        }
    }
}

// Функция для подсчета элементов, удовлетворяющих условию ai-1 < ai > ai+1
int countCondition(int *arr, int n)
{
    if (n < 3)
        return 0;

    int count = 0;
    for (int i = 1; i < n - 1; i++)
    {
        if (arr[i - 1] < arr[i] && arr[i] > arr[i + 1])
        {
            count++;
        }
    }
    return count;
}

// Функция для ввода последовательности
int *inputSequence(int *n)
{
    cout << "Введите количество элементов последовательности: ";
    cin >> *n;

    if (*n <= 0)
    {
        cout << "Количество элементов должно быть положительным!" << endl;
        return NULL;
    }

    // Выделяем память с использованием malloc
    int *arr = (int *)malloc(*n * sizeof(int));

    if (arr == NULL)
    {
        cout << "Ошибка выделения памяти!" << endl;
        return NULL;
    }

    cout << "Введите " << *n << " элементов последовательности: " << endl;
    for (int i = 0; i < *n; i++)
    {
        cout << "Элемент " << i + 1 << ": ";
        cin >> arr[i];
    }

    return arr;
}

// Функция для вывода последовательности
void printSequence(int *arr, int n)
{
    cout << "Последовательность: ";
    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
    cout << "" << endl;
}

// Функция для обработки частичной сортировки
void partialSort(int *arr, int n)
{
    int p, q;

    cout << "Введите индекс p (от 1 до " << n << "): ";
    cin >> p;

    cout << "Введите индекс q (от " << p + 1 << " до " << n << "): ";
    cin >> q;

    q--;
    p--; // Корректировка значений (т.к. нумерация начинается с 0)

    // Проверка корректности индексов
    if (p < 0 || q >= n || p > q)
    {
        cout << "Некорректные индексы!" << endl;
        return;
    }

    // Сортируем часть массива
    for (int i = p; i <= q; i++)
    {
        int maxIndex = i, temp = arr[i];
        // Находим максимальный элемент в оставшейся части
        for (int j = i + 1; j <= q; j++)
        {
            if (arr[j] > arr[maxIndex])
            {
                maxIndex = j;
                temp = arr[maxIndex];
            }
            arr[maxIndex] = arr[i];
            arr[i] = temp;
        }
    }

    cout << "Последовательность после сортировки элементов с " << p + 1 << " по " << q + 1 << ":" << endl;
    printSequence(arr, n);
}