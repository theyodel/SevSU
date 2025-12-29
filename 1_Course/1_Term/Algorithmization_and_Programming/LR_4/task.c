#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>

// Объявление функций
int countCondition(int *arr, int n);
int *inputSequence(int *n);
void printSequence(int *arr, int n);
void partialSort(int *arr, int n);

int main() {
    int *arr = NULL, n = 0, choice;
    
    while (1) {
        printf("\n======= М Е Н Ю =======\n");
        printf("1. Ввести новую последовательность\n");
        printf("2. Вывести текущую последовательность\n");
        printf("3. Найти количество элементов по условию\n");
        printf("4. Отсортировать часть последовательности\n");
        printf("9. Выход\n");
        printf("-> ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                // Освобождаем старую память, если она была выделена
                if (arr != NULL) {
                    free(arr);
                }
                
                arr = inputSequence(&n);
                if (arr != NULL) {
                    printf("Последовательность успешно введена.\n");
                }
                break;
                
            case 2:
                if (arr == NULL) {
                    printf("Последовательность не введена!\n");
                } 
                else {
                    printSequence(arr, n);
                }
                break;
                
            case 3:
                if (arr == NULL) {
                    printf("Последовательность не введена!\n");
                } 
                else {
                    int count = countCondition(arr, n);
                    printf("Количество элементов, удовлетворяющих условию ai-1 < ai > ai+1: %d\n", count);
                }
                break;
                
            case 4:
                if (arr == NULL) {
                    printf("Последовательность не введена!\n");
                } 
                else {
                    partialSort(arr, n);
                }
                break;
                
            case 9:
                printf("Выход из программы...\n");
                // Освобождаем память перед выходом
                if (arr != NULL) {
                    free(arr);
                }
                return 0;
                break;
                
            default:
                printf("Неверный выбор! Попробуйте снова.\n");
        }
    }
}

// Функция для подсчета элементов, удовлетворяющих условию ai-1 < ai > ai+1
int countCondition(int *arr, int n) {
    if (n < 3) return 0;
    
    int count = 0;
    for (int i = 1; i < n - 1; i++) {
        if (arr[i-1] < arr[i] && arr[i] > arr[i+1]) {
            count++;
        }
    }
    return count;
}

// Функция для ввода последовательности
int *inputSequence(int *n) {
    printf("Введите количество элементов последовательности: ");
    scanf("%d", n);
    
    if (*n <= 0) {
        printf("Количество элементов должно быть положительным!\n");
        return NULL;
    }
    
    // Выделяем память с использованием malloc
    int *arr = (int*) malloc(*n * sizeof(int));
    
    if (arr == NULL) {
        printf("Ошибка выделения памяти!\n");
        return NULL;
    }
    
    printf("Введите %d элементов последовательности:\n", *n);
    for (int i = 0; i < *n; i++) {
        printf("Элемент %d: ", i + 1);
        scanf("%d", &arr[i]);
    }
    
    return arr;
}

// Функция для вывода последовательности
void printSequence(int *arr, int n) {
    printf("Последовательность: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Функция для обработки частичной сортировки
void partialSort(int *arr, int n) {
    int p, q;
    
    printf("Введите индекс p (от 1 до %d): ", n);
    scanf("%d", &p);
    
    printf("Введите индекс q (от %d до %d): ", p+1, n);
    scanf("%d", &q);

    q--; p--; // Корректировка значений (т.к. нумерация начинается с 0)
    
    // Проверка корректности индексов
    if (p < 0 || q >= n || p > q) {
        printf("Некорректные индексы!\n");
        return;
    }
    
    // Сортируем часть массива
    for (int i = p; i <= q; i++) {
        int maxIndex = i, temp = arr[i];
        // Находим максимальный элемент в оставшейся части
        for (int j = i + 1; j <= q; j++) {
            if (arr[j] > arr[maxIndex]) {
                maxIndex = j;
                temp = arr[maxIndex];
            }
            arr[maxIndex] = arr[i];
            arr[i] = temp;
        }
    }
    
    printf("Последовательность после сортировки элементов с %d по %d:\n", p + 1, q + 1);
    printSequence(arr, n);
}