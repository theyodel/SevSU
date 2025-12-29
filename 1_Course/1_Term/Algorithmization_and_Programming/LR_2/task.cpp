#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

int main()
{
    float a, b, x_start, x_end, dx, z;
    cout << "Введите параметр a: ";
    cin >> a;
    cout << "Введите параметр b: ";
    cin >> b;
    cout << "Введите начальное значение x: ";
    cin >> x_start;
    cout << "Введите конечное значение x: ";
    cin >> x_end;
    cout << "Введите шаг dx: ";
    cin >> dx;
    z = 0;

    if (dx == 0)
    {
        cout << "Ошибка: шаг dx должен быть ненулевым!" << endl;
        return 1;
    }
    if (cos(x_start) == 0)
    {
        cout << "Ошибка: cos(x) = 0, что по условию не может быть!" << endl;
        return 1;
    }
    if (x_start <= 0)
    {
        cout << "Ошибка: x = 0, что по условию не может быть!" << endl;
        return 1;
    }
    if (cos(x_end) == 0)
    {
        cout << "Ошибка: cos(x_end) = 0, что по условию не может быть!" << endl;
        return 1;
    }
    if (x_end <= 0)
    {
        cout << "Ошибка: x_end = 0, что по условию не может быть!" << endl;
        return 1;
    }
    if (x_start < x_end)
    {
        cout << "Ошибка: x_start < x_end, что по условию не может быть!" << endl;
        return 1;
    }

    cout << fixed << setprecision(4);

    cout << "________________________________" << endl;
    cout << "|      x      |      f(x)      |" << endl;
    cout << "|_____________|________________|" << endl;

    for (float x = x_start; x <= x_end; x += dx)
    {
        if (x <= a)
        {
            z = log(x) + sin(x);
        }
        else if (a < x && x < b)
        {
            z = log(x) + cos(x);
        }
        else
        {
            z = tan(x);
        }
        cout << "|  " << setw(9) << x << "  |" << setw(10) << z << "      |" << endl;
        cout << "|_____________|________________|" << endl;
    }
    return 0;
}