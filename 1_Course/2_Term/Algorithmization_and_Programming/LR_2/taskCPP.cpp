#include <iostream>
#include <string>
#include <stdlib.h>
#include <limits>
#include <windows.h>

using namespace std;

string readString();
void outputString(string s);
int isByeEnd(string s);
string replaceStars(string s);
void outputBeforeSpace(string s);

int main()
{   
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    int choice;
    string s = "";
    while (1)
    {   
        cout << endl;
        cout << "================ МЕНЮ ================" << endl;
        cout << "1. Записать новую строку" << endl;
        cout << "2. Вывести текущую строку" << endl;
        cout << "3. Проверить окончание строки на \"Bye\"" << endl;
        cout << "4. Заменить все \"***\" на \"/\"" << endl;
        cout << "5. Вывести подстроку до первого символа \" \"" << endl;
        cout << "0. Выход из программы" << endl;
        cout << "-> ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            s = readString();
            break;

        case 2:
            if (s == "")
            {
                cout << "Строка не введена!" << endl;
            }
            else
            {
                outputString(s);
            }
            break;

        case 3:
            if (s == "")
            {
                cout << "Строка не введена!" << endl;
            }
            else
            {
                if (isByeEnd(s))
                {
                    cout << "Текущая строка заканчивается на \"Bye\"" << endl;
                }
                else
                {
                    cout << "Текущая строка НЕ заканчивается на \"Bye\"" << endl;
                }
            }
            break;

        case 4:
            if (s == "")
            {
                cout << "Строка не введена!" << endl;
            }
            else
            {
                s = replaceStars(s);
            }
            break;

        case 5:
            if (s == "")
            {
                cout << "Строка не введена!" << endl;
            }
            else
            {
                outputBeforeSpace(s);
            }
            break;

        case 0:
            return 0;

        default:
            cout << "Команда не распознана!" << endl;
        }
    }
}

string readString()
{
    string s;
    cout << "Введите строку: ";
    getline(cin, s);
    return s;
}

void outputString(string s)
{
    cout << "Введённая вами строка: " << s << endl;
    cout << "Вес строки: " << sizeof(s) << " Байт" << endl;
}

int isByeEnd(string s)
{
    if (s.length() < 3)
    {
        return 0;
    }
    int len = s.length();
    string lastChars = string(1, s[len - 3]) + s[len - 2] + s[len - 1];
    if (lastChars == "Bye")
    {
        return 1;
    }
    return 0;
}

string replaceStars(string s)
{
    string result = s;
    unsigned int pos = 0;
    while (result.find("***", pos) != -1)
    {
        pos = result.find("***", pos);
        result.replace(pos, 3, "/");
        pos++;
    }
    cout << "Полученная строка: " << result << endl;
    cout << "Вес строки: " << result.length() + 1 << " Байт" << endl;

    return result;
}

void outputBeforeSpace(string s)
{
    int flag = s.find(" ");
    for (int i = 0; i < flag; i++)
    {
        cout << s[i];
    }
    cout << endl;
}