#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <iomanip>
#include <string>

using namespace std;

struct man {
    string name, surname;
    char gender;
    int age, num, height;
    double weight;
};

man inputData();
void outputData(man);
bool processData(man, double);

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
                    if (i == 1)
                        m1 = inputData();
                    else
                        m2 = inputData();
                }
                flag = 1;
                cout << "Данные введены" << endl;
                break;
            
            case 2:
                if (flag != 0) {
                    cout << "\n\n+---+---------------------------+---------------------------+-------+-------+-----------+-------+" << endl;
                    cout << "| # |          Фамилия          |            Имя            |  Пол  | Рост  |    Вес    | Возр. |" << endl;
                    cout << "+---+---------------------------+---------------------------+-------+-------+-----------+-------+" << endl;
                    outputData(m1);
                    outputData(m2);
                } else {
                    cout << "Данные не введены!" << endl;
                }
                break;
            
            case 3:
                if (flag == 1) {
                    double ideal;
                    cout << "Введите идеал веса -> ";
                    cin >> ideal;
                    processData(m1, ideal);
                    processData(m2, ideal);
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

man inputData() {
    man var;
    static int num = 1;

    cout << "Введите имя -> ";
    cin >> var.name;

    cout << "Введите фаимлию -> ";
    cin >> var.surname;

    cout << "Введите возраст -> ";
    cin >> var.age;

    cout << "Введите рост -> ";
    cin >> var.height;

    cout << "Введите вес -> ";
    cin >> var.weight;

    cout << "Введите пол -> ";
    cin >> var.gender;

    var.num = num++;

    return var;
}

void outputData(man m) {
    cout << "| " << setw(1) << m.num
         << " | " << left << setw(25) << m.surname
         << " | " << left << setw(25) << m.name
         << " | " << left << setw(5) << m.gender
         << " | " << left << setw(5) << m.height
         << " | " << left << fixed << setprecision(2) << setw(9) << m.weight
         << " | " << left << setw(5) << m.age << " |" << endl;
    cout << "+---+---------------------------+---------------------------+-------+-------+-----------+-------+" << endl;
}

bool processData(man m, double ideal) {
    if (m.weight > ideal) {
        cout << "Вес " << m.surname << " " << m.name << " больше заданного идеала (" << m.weight << " > " << ideal << ")" << endl; 
        return true;
    }
    cout << "Вес " << m.name << " " << m.surname << " меньше заданного идеала" << endl;
    return false;
}