#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <locale.h>

// Объявление функций
int** matrixInput(int rows, int cols);
void matrixOutput(int** matrix, const int rows, const int cols);
void sumNoPositiveRows(int** matrix, const int rows, const int cols);
void countLocalMin(int** matrix, const int rows, const int cols);

int main() {
    int **matrix = NULL;
    int rows = 0, cols = 0, choice;

    while (1) {
        printf("\n======= М Е Н Ю =======\n");
        printf("1. Ввести новую матрицу\n");
        printf("2. Вывести текущую матрицу\n");
        printf("3. Найти сумму полностью отрицательных строк\n");
        printf("4. Найти количество локальных минимумов (только для матриц 5x5)\n");
        printf("9. Выход\n");
        printf("-> ");
        scanf("%d", &choice);
        printf("\n");

        switch (choice)
        {
        case 1:
            if (matrix != NULL) {
                for (int i=0; i<rows; i++) {
                    free(matrix[i]);
                }
                free(matrix);
            }
            printf("Введите количество строк матрицы: ");
            scanf("%d", &rows);
            printf("Введите количество столбцов матрицы: ");
            scanf("%d", &cols);
            matrix = matrixInput(rows, cols);
            if (matrix != NULL) {
                printf("Матрица успешно введена.\n");
            }
            break;
 
        case 2:
            if (matrix == NULL) {
                printf("Матрица не введена!\n");
            }
            else {
                matrixOutput(matrix, rows, cols);
            }
            break;
        
        case 3:
            if (matrix == NULL) {
                printf("Матрица не введена!\n");
            }
            else {
                sumNoPositiveRows(matrix, rows, cols);
            }
            break;

        case 4:
            if (matrix == NULL) {
                printf("Матрица не введена!\n");
            }
            else {
                countLocalMin(matrix, rows, cols);
            }
            break;

        case 9:
            printf("Выход из программы...\n");
            if (matrix != NULL) {
                for (int i=0; i<rows; i++) {
                    free(matrix[i]);
                }
                free(matrix);
            }
            return 0;
            break;
        
        default:
            printf("Неверный выбор! Попробуйте снова.\n");
        }
    }
}

int** matrixInput(int rows, int cols) {
    // Проверка корректности входных данных
    if (rows <= 0 || cols <= 0) {
        printf("Ошибка: размеры матрицы должны быть положительными числами\n");
        return NULL;
    }
    
    // Выделение памяти под массив указателей на строки
    int** matrix = (int**)malloc(rows * sizeof(int*));
    if (matrix == NULL) {
        printf("Ошибка выделения памяти для указателей строк\n");
        return NULL;
    }
    
    // Выделение памяти для каждой строки
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }
    
    // Ввод элементов матрицы с клавиатуры
    printf("Введите элементы матрицы %dx%d:\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        printf("Строка %d: \n", i + 1);
        for (int j = 0; j < cols; j++) {
            printf("Элемент [%d][%d]: ", i+1, j+1);
            scanf("%d", &matrix[i][j]);
        }
    }
    
    return matrix;
}

void matrixOutput(int** matrix, const int rows, const int cols) {    
    printf("Введённая матрица размером %dx%d:\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%5d ", matrix[i][j]);
        }
        printf("\n"); // Переход на следущую строку
    }
}

void sumNoPositiveRows(int** matrix, const int rows, const int cols) {
    int sum = 0, cRows = 0, flag = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (flag == 0 && matrix[i][j] >= 0) {
                flag = 1;
            }
        }
        if (flag == 0) {
            cRows++; // Добавляем +1 к количеству полностью отрицательных строк
            for (int j = 0; j < cols; j++) {
                sum += matrix[i][j];
            }
        }
        else {
            flag = 0; // Обнуление флага при переходе на следующую строку
        }
    }
    printf("Сумма полностью отрицательных строк: %d\n", sum);
    printf("Количество полностью отрицательных строк: %d\n", cRows);
}

void countLocalMin(int** matrix, const int rows, const int cols) {
    // Проверка размера матрицы
    if (rows != 5 && cols != 5) {
        printf("Введённая матрица имеет размер %dx%d. Для выполнения данной функции необходима матрица 5x5!\n", rows, cols);
        return;
    }

    int cLocalMin = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int current = matrix[i][j];
            int flag = 1;
            
            // Проверяем всех соседей
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    if (di == 0 && dj == 0) {
                        continue;
                    }
                    
                    int ni = i + di;
                    int nj = j + dj;
                    
                    // Проверяем границы
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                        if (matrix[ni][nj] <= current) {
                            flag = 0;
                            // Прерываем оба цикла проверки соседей
                            di = 2; // Выход из внешнего цикла
                            break;
                        }
                    }
                }
            }
            if (flag == 1) {
                cLocalMin++;
            }
        }
    }
    
    printf("Количество локальных минимумов: %d\n", cLocalMin);
}