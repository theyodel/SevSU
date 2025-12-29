#include <iostream>
#include <math.h>

using namespace std;

int main()
{
    float a, b, x, z;
    cout << "Введите параметр a: ";
    cin >> a;
    cout << "Введите параметр b: ";
    cin >> b;
    cout << "Введите аргумент x: ";
    cin >> x;

    if (b < x && x < a)
    {
        cout << "Аргумент x не может быть в диапазоне b < x < a";
    }

    else if (x <= a)
    {
        z = log(x) + sin(x);
        cout << "Значение функции z = " << z << endl;
    }

    else if (a < x && x < b)
    {
        z = log(x) + cos(x);
        cout << "Значение функции z = " << z << endl;
    }

    else
    {
        z = tan(x);
        cout << "Значение функции z = " << z << endl;
    }

    return 0;
}
