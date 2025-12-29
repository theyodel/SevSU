#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <iomanip>

using namespace std;

// Объявление функций
int **matrixInput(int rows, int cols);
void matrixOutput(int **matrix, const int rows, const int cols);
void sumNoPositiveRows(int **matrix, const int rows, const int cols);
void countLocalMin(int **matrix, const int rows, const int cols);

int main()
{
    int **matrix = NULL;
    int rows = 0, cols = 0, choice;

    while (1)
    {
        cout << endl
             << "======= М Е Н Ю =======" << endl;
        cout << "1. Ввести новую матрицу" << endl;
        cout << "2. Вывести текущую матрицу" << endl;
        cout << "3. Найти сумму полностью отрицательных строк" << endl;
        cout << "4. Найти количество локальных минимумов (только для матриц 5x5)" << endl;
        cout << "9. Выход" << endl;
        cout << "-> ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (matrix != NULL)
            {
                for (int i=0; i<rows; i++) {
                    free(matrix[i]);
                }
                free(matrix);
            }
            cout << endl << "Введите количество строк матрицы: ";
            cin >> rows;
            cout << "Введите количество столбцов матрицы: ";
            cin >> cols;
            matrix = matrixInput(rows, cols);
            if (matrix != NULL)
            {
                cout << "Матрица успешно введена." << endl;
            }
            break;

        case 2:
            if (matrix == NULL)
            {
                cout << "Матрица не введена!" << endl;
            }
            else
            {
                matrixOutput(matrix, rows, cols);
            }
            break;

        case 3:
            if (matrix == NULL)
            {
                cout << "Матрица не введена!" << endl;
            }
            else
            {
                sumNoPositiveRows(matrix, rows, cols);
            }
            break;

        case 4:
            if (matrix == NULL)
            {
                cout << "Матрица не введена!" << endl;
            }
            else
            {
                countLocalMin(matrix, rows, cols);
            }
            break;

        case 9:
            cout << "Выход из программы..." << endl;
            if (matrix != NULL)
            {
                free(matrix);
            }
            return 0;
            break;

        default:
            cout << endl
                 << "Неверный выбор! Попробуйте снова." << endl;
        }
    }
}

int **matrixInput(int rows, int cols)
{
    // Проверка корректности входных данных
    if (rows <= 0 || cols <= 0)
    {
        cout << "Ошибка: размеры матрицы должны быть положительными числами" << endl;
        return NULL;
    }

    // Выделение памяти под массив указателей на строки
    int **matrix = (int **)malloc(rows * sizeof(int *));
    if (matrix == NULL)
    {
        cout << "Ошибка выделения памяти для указателей строк" << endl;
        return NULL;
    }

    // Выделение памяти для каждой строки
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (int *)malloc(cols * sizeof(int));
        if (matrix[i] == NULL)
        {
            cout << "Ошибка выделения памяти для строки " << i << "." << endl;

            // Освобождаем уже выделенную память при ошибке
            for (int j = 0; j < i; j++)
            {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }

    // Ввод элементов матрицы с клавиатуры
    cout << "Введите элементы матрицы " << rows << "x" << cols << ":" << endl;
    for (int i = 0; i < rows; i++)
    {
        cout << "Строка " << i + 1 << ":" << endl;
        for (int j = 0; j < cols; j++)
        {
            cout << "Элемент [" << i + 1 << "][" << j + 1 << "]: ";
            cin >> matrix[i][j];
        }
    }
    return matrix;
}

void matrixOutput(int **matrix, const int rows, const int cols)
{
    cout << endl
         << "Введённая матрица размером " << rows << "x" << cols << ":" << endl;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cout << setw(5) << matrix[i][j] << " ";
        }
        cout << endl; // Переход на следущую строку
    }
}

void sumNoPositiveRows(int **matrix, const int rows, const int cols)
{
    int sum = 0, cRows = 0, flag = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (flag == 0 && matrix[i][j] >= 0)
            {
                flag = 1;
            }
        }
        if (flag == 0)
        {
            cRows++; // Добавляем +1 к количеству полностью отрицательных строк
            for (int j = 0; j < cols; j++)
            {
                sum += matrix[i][j];
            }
        }
        else
        {
            flag = 0; // Обнуление флага при переходе на следующую строку
        }
    }
    cout << endl
         << "Сумма полностью отрицательных строк: " << sum << endl;
    cout << "Количество полностью отрицательных строк: " << cRows << endl;
}

void countLocalMin(int **matrix, const int rows, const int cols)
{
    // Проверка размера матрицы
    if (rows != 5 && cols != 5)
    {
        cout << endl
             << "Введённая матрица имеет размер " << rows << "x" << cols << ". Для выполнения данной функции необходима матрица 5x5!" << endl;
        return;
    }

    int cLocalMin = 0;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int current = matrix[i][j];
            int flag = 1;

            // Проверяем всех соседей
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    if (di == 0 && dj == 0)
                    {
                        continue;
                    }

                    int ni = i + di;
                    int nj = j + dj;

                    // Проверяем границы
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols)
                    {
                        if (matrix[ni][nj] <= current)
                        {
                            flag = 0;
                            // Прерываем оба цикла проверки соседей
                            di = 2; // Выход из внешнего цикла
                            break;
                        }
                    }
                }
            }
            if (flag == 1)
            {
                cLocalMin++;
            }
        }
    }

    cout << endl
         << "Количество локальных минимумов: " << cLocalMin << endl;
}