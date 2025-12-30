#include <iostream>
#include <stdlib.h>
#include <string>
#include <iomanip>
#include <locale.h>
#include <math.h>

using namespace std;

// Создание структуры "route"
struct route
{
    int number;
    int routeNumber;
    string startPoint;
    string endPoint;
    float price;
    int time;
};

// Объявление функций
route *scanRoutes(int n);
void printRoutes(route *arr, int n);
int countCircleRoutes(route *arr, int n);
void sortByPrice(route *arr, int n);

int main()
{
    char *locale = setlocale(LC_ALL, "");
    int choice, n = 0;
    route *arr = NULL;
    while (1)
    {
        cout << "\n======= М Е Н Ю =======\n";
        cout << "1. Ввести все маршруты" << endl;
        cout << "2. Вывести все маршруты" << endl;
        cout << "3. Найти количество круговых маршрутов" << endl;
        cout << "4. Отсортировать маршруты по стоимости проезда" << endl;
        cout << "9. Выход" << endl;
        cout << "-> ";
        cin >> choice;
        cout
            << endl;

        switch (choice)
        {
        case 1:
            if (arr != NULL)
            {
                delete[] arr;
            }
            n = 0;
            cout << "Введите количество маршрутов: ";
            cin >> n;
            if (n <= 0)
            {
                cout << "Количество маршрутов должно быть > 0" << endl;
                break;
            }
            arr = scanRoutes(n);
            break;

        case 2:
            if (arr == NULL)
            {
                cout << "Маршруты не введены!" << endl;
            }
            else
            {
                cout << "Все маршруты:" << endl;
                printRoutes(arr, n);
            }
            break;

        case 3:
            if (arr == NULL)
            {
                cout << "Маршруты не введены!" << endl;
            }
            else
            {
                cout << "Количество круговых маршрутов: " << countCircleRoutes(arr, n) << endl;
            }
            break;

        case 4:
            if (arr == NULL)
            {
                cout << "Маршруты не введены!" << endl;
            }
            else
            {
                sortByPrice(arr, n);
            }
            break;

        case 9:
            delete[] arr;
            cout << "Выход из программы . . .";
            return 0;

        default:
            cout << "Неверный выбор! Попробуйте снова." << endl;
            break;
        }
    }
}

route *scanRoutes(int n)
{
    route *arr = new route[n];

    if (arr == NULL)
    {
        cout << "Ошибка выделения памяти!" << endl;
        return NULL;
    }

    cout << "Введите " << n << " маршрутов:" << endl;
    for (int i = 0; i < n; i++)
    {
        arr[i].number = i + 1;
        cout << "Введите номер маршрута: ";
        cin >> arr[i].routeNumber;
        while (getchar() != '\n')
            ;

        cout << "Введите начальную точку маршрута (до 100 символов): ";
        getline(cin, arr[i].startPoint);

        cout << "Введите конечную точку маршрута (до 100 символов): ";
        getline(cin, arr[i].endPoint);

        cout << "Введите цену маршрута: ";
        cin >> arr[i].price;

        cout << "Введите время в пути (в минутах): ";
        cin >> arr[i].time;

        cout << "----------------------------------------------" << endl;
    }

    return arr;
}

void printRoutes(route *arr, int n)
{
    cout << "----------------------------------------------" << endl;

    for (int i = 0; i < n; i++)
    {
        cout << fixed << setprecision(2);
        cout << arr[i].number << ") Номер маршрута: " << arr[i].routeNumber << endl;
        cout << "Начальная точка:" << arr[i].startPoint << endl
             << "Конечная точка: " << arr[i].endPoint << endl;
        cout
            << "Цена проезда: " << arr[i].price << "₽" << endl;
        if (arr[i].time % 60 == 0)
        {
            cout << "Время в пути" << arr[i].time / 60 << "ч" << endl;
        }
        else if (arr[i].time < 60)
        {
            cout << "Время в пути" << arr[i].time << "мин" << endl;
        }
        else
        {
            cout << "Время в пути: " << arr[i].time / 60 << "ч " << arr[i].time % 60 << "мин" << endl;
        }
        cout << "----------------------------------------------" << endl;
    }

    cout << "Всего маршрутов: " << n << endl;
}

int countCircleRoutes(route *arr, int n)
{
    int count = 0;

    for (int i = 0; i < n; i++)
    {
        if (arr[i].startPoint == arr[i].endPoint)
        {
            count++;
        }
    }

    return count;
}

void sortByPrice(route *arr, int n)
{
    route temp, tarr[n];

    for (int i = 0; i < n; i++)
    {
        tarr[i] = arr[i];
    }

    for (int j = n; j > 1; j--)
    {
        for (int i = 0; i < j - 1; i++)
        {
            if (tarr[i].price < tarr[i + 1].price)
            {
                temp = tarr[i];
                tarr[i] = tarr[i + 1];
                tarr[i + 1] = temp;
            }
        }
    }

    cout << "Отсортированный список маршрутов по полю \"Цена проезда\":" << endl;
    printRoutes(tarr, n);
}
