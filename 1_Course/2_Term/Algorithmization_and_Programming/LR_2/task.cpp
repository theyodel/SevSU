#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

int main()
{
    int choice; /// Переменная для храниения выбора пользователя
    string s = "";
    while (1)
    {
        cout << endl
             << "================ МЕНЮ ================" << endl;
        cout << "1. Записать новую строку" << endl;
        cout << "2. Вывести текущую строку" << endl;
        cout << "3. Проверить окончание строки на \"Bye\"" << endl;
        cout << "4. Заменить все \"***\" на \"/\"" << endl;
        cout << "5. Вывести подстроку до первого символа \" \"" << endl;
        cout << "0. Выход из программы" << endl;
        cout << "->";
        cin >> choice;
    }
}