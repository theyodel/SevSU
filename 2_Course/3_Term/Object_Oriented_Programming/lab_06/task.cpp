#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <iomanip>
#include <string>

using namespace std;

class man {
private:
    string name, surname;
    char gender;
    int age, num, height;
    double weight;

public:
    man();
    man(string, string, char, int, int, double);

    bool operator!();
    double operator/(man&);

    friend void operator++(man&, int);
    friend bool operator!=(man&, man&);

    friend ostream& operator<<(ostream&, const man&);
    friend istream& operator>>(istream&, man&);

    ~man();
};

man::man() {
    cout << "[ ! ] Конструктор по умолчанию вызван" << endl;
    num = 1;
    name = "Jane";
    surname = "Doe";
    gender = 'F';
    age = 12;
    height = 190;
    weight = 30;
}

man::man(string n, string s, char g, int a, int h, double w) {
    name = n; surname = s; gender = g;
    age = a; height = h; weight = w;
    num = 0;
    cout << "[ ! ] Конструктор с параметрами вызван" << endl;
}

man::~man() {
    cout << "[ ! ] Деструктор вызван" << endl;
}

bool man::operator!() {
    return this->weight > 80;
}

double man::operator/(man& _s) {
    return (this->height + _s.height) / 2.0f;
}

void operator++(man& _m, int _) {
    _m.age += 1;
}

bool operator!=(man& _m1, man& _m2) {
    return _m1.surname != _m2.surname;
}

istream& operator>>(istream& is, man& _s) {
    static int counter = 1;
    _s.num = counter++;

    cout << "Введите имя -> ";
    is >> _s.name;
    cout << "Введите фамилию -> ";
    is >> _s.surname;
    cout << "Введите возраст -> ";
    is >> _s.age;
    cout << "Введите рост -> ";
    is >> _s.height;
    cout << "Введите вес -> ";
    is >> _s.weight;
    cout << "Введите пол -> ";
    is >> _s.gender;

    return is;
}

ostream& operator<<(ostream& os, const man& _s) {
    os << "| " << setw(1) << _s.num
       << " | " << left << setw(25) << _s.surname
       << " | " << left << setw(25) << _s.name
       << " | " << left << setw(5)  << _s.gender
       << " | " << left << setw(5)  << _s.height
       << " | " << left << fixed << setprecision(2) << setw(9) << _s.weight
       << " | " << left << setw(5)  << _s.age << " |" << endl;
    return os;
}

int main() {
    int choice;
    man m1, m2;

    while (true) {
        cout << "\n======== МЕНЮ =======" << endl;
        cout << "1. Записать значения в переменные" << endl;
        cout << "2. Вывести данные на экран" << endl;
        cout << "3. Проверка на превышение веса" << endl;
        cout << "4. Увеличить возраст" << endl;
        cout << "5. Получить среднее арифметическое по росту" << endl;
        cout << "6. Проверить на однофамильность" << endl;
        cout << "0. Выход" << endl;
        cout << "-> ";
        cin >> choice;

        switch (choice) {

        case 0:
            return 0;

        case 1:
            cout << "--- Объект #1 ---" << endl;
            cin >> m1;
            cout << "--- Объект #2 ---" << endl;
            cin >> m2;
            break;

        case 2:
            cout << "+---+---------------------------+---------------------------+-------+-------+-----------+-------+" << endl;
            cout << m1 << m2;
            cout << "+---+---------------------------+---------------------------+-------+-------+-----------+-------+" << endl;
            break;

        case 3:
            cout << "Объект #1: " << (!m1 ? "вес ПРЕВЫШЕН (>80)" : "вес в норме") << endl;
            cout << "Объект #2: " << (!m2 ? "вес ПРЕВЫШЕН (>80)" : "вес в норме") << endl;
            break;

        case 4:
            m1++;
            m2++;
            cout << "Возраст обоих увеличен на 1" << endl;
            break;

        case 5:        
            double avg = m1 / m2;
            cout << "Средний рост: " << setprecision(2) << avg << " см" << endl;
            break;

        case 6:
            cout << (m1 != m2 ? "Фамилии РАЗНЫЕ" : "Фамилии ОДИНАКОВЫЕ") << endl;
            break;

        default:
            cout << "Неизвестная команда" << endl;
        }
    }
    return 0;
}