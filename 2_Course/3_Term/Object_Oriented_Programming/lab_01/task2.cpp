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
        void input();
        void output();
        bool process(double);
        ~man();
};

man::man() {
    cout << "[ ! ] Конструктор по умолчанию вызван" << endl;
}

man::man(string n, string s, char g, int a, int h, double w) {
    name = n;
    surname = s;
    gender = g;
    age = a;
    height = h;
    weight = w;
    cout << "[ ! ] Конструктор с параметрами вызван" << endl;
}

man::~man() {
    cout << "[ ! ] Деструктор вызван" << endl;
}

void man::input() {
    static int counter = 1;

    cout << "Введите имя -> ";
    cin >> name;
    cout << "Введите фамилию -> ";
    cin >> surname;
    cout << "Введите возраст -> ";
    cin >> age;
    cout << "Введите рост -> ";
    cin >> height;
    cout << "Введите вес -> ";
    cin >> weight;
    cout << "Введите пол -> ";
    cin >> gender;

    num = counter++;
}

void man::output() {
    cout << "| " << setw(1) << num
         << " | " << left << setw(25) << surname
         << " | " << left << setw(25) << name
         << " | " << left << setw(5) << gender
         << " | " << left << setw(5) << height
         << " | " << left << fixed << setprecision(2) << setw(9) << weight
         << " | " << left << setw(5) << age << " |" << endl;
    cout << "+---+---------------------------+---------------------------+-------+-------+-----------+-------+" << endl;
}

bool man::process(double ideal) {
    if (weight > ideal) {
        cout << "Вес " << surname << " " << name << " больше заданного идеала (" << weight << " > " << ideal << ")" << endl; 
        return true;
    }
    cout << "Вес " << name << " " << surname << " меньше заданного идеала" << endl;
    return false;
}

int main() {
    int flag = 0, choice;
    man m1, m2;

    while (true) {
        cout << endl << "\n\n======== МЕНЮ =======" << endl;
        cout << "1. Записать значения в переменные" << endl;
        cout << "2. Вывести данные на экран" << endl;
        cout << "3. Обработка согласно варианту" << endl;
        cout << "4. Выход" << endl;
        cout << "-> ";
        cin >> choice;

        switch (choice) {
            case 1:
                for (int i = 1; i < 3; i++) {
                    cout << "Введите значения в " << i << " переменную" << endl;
                    if (i == 1) {
                        m1.input();
                    } else { 
                        m2.input();
                    }
                }
                flag = 1;
                cout << "Данные введены" << endl;
                break;
            
            case 2:
                if (flag != 0) {
                    cout << "\n\n+---+---------------------------+---------------------------+-------+-------+-----------+-------+" << endl;
                    cout << "| # |          Фамилия          |            Имя            |  Пол  | Рост  |    Вес    | Возр. |" << endl;
                    cout << "+---+---------------------------+---------------------------+-------+-------+-----------+-------+" << endl;
                    m1.output();
                    m2.output();
                } else {
                    cout << "Данные не введены!" << endl;
                }
                break;
            
            case 3:
                if (flag == 1) {
                    double ideal;
                    cout << "Введите идеал веса -> ";
                    cin >> ideal;
                    m1.process(ideal);
                    m2.process(ideal);
                } else {
                    cout << "Данные не введены!" << endl;
                }
                break;
            
            case 4:
                cout << "Выход из программы..." << endl;
                return 0;
        }
    }
}
