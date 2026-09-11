#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <iomanip>
#include <string>

using namespace std;

struct man {
    string name, surname, sex;
    int age, num, height;
    double weight;
};

man inputData();
void outputData(man, man);
bool processData(man, man, double);

int main() {
    int flag = 0;
    man m1, m2;
    int choice;

    while (true) {
        cout << "======== МЕНЮ =======" << endl;
        cout << "1. Записать значения в переменные" << endl;
        cout << "2. Вывести данные на экран" << endl;
        cout << "3. Обработка согласно варианту" << endl;
        cout << "4. Выход" << endl;
        cout << "-> ";
        cin >> choice;

        switch (choice) {
            case 1:
                for (int i = 0; i < 2; i++) {
                    cout << "Введите значения в " << i << " переменную" << endl;
                    if (i == 0)
                        m1 = inputData();
                    else
                        m2 = inputData();
                }
                flag = 1;
                cout << "Данные введены" << endl;
                break;
            
            case 2:
                if (flag == 1) outputData(m1, m2);
                else cout << "Данные отсутствуют" << endl;
                break;
            
            case 3:
                if (flag == 1) {
                    double ideal;
                    cout << "Введите идеал веса -> ";
                    cin >> ideal;
                    processData(m1, m2, ideal);
                }
                else cout << "Данные отсутствуют" << endl;
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
    cout << endl << "Введите фаимлию -> ";
    cin >> var.surname;
    cout << endl << "Введите возраст -> ";
    cin >> var.age;
    cout << endl << "Введите рост -> ";
    cin >> var.height;
    cout << endl << "Введите вес -> ";
    cin >> var.weight;
    cout << endl << "Введите пол -> ";
    cin >> var.sex;
    var.num = num++;

    return var;
}

void outputData(man m1, man m2) {
    cout << "+---+---------------------------+---------------------------+-------+------+------+------+\n";
    cout << "| # |          Фамилия          |            Имя            |  Пол  | Рост | Вес  | Возр.|\n";
    cout << "+---+---------------------------+---------------------------+-------+------+------+------+\n";

    cout << "| " << setw(1) << m1.num
         << " | " << left << setw(25) << m1.surname
         << " | " << left << setw(25) << m1.name
         << " | " << left << setw(5) << m1.sex
         << " | " << right << setw(5) << m1.height
         << " | " << right << fixed << setprecision(2) << setw(5) << m1.weight
         << " | " << right << setw(5) << m1.age << " |\n";

    cout << "| " << setw(1) << m2.num
         << " | " << left << setw(25) << m2.surname
         << " | " << left << setw(25) << m2.name
         << " | " << left << setw(5) << m2.sex
         << " | " << right << setw(5) << m2.height
         << " | " << right << fixed << setprecision(2) << setw(5) << m2.weight
         << " | " << right << setw(5) << m2.age << " |\n";

    cout << "+---+---------------------------+---------------------------+-------+------+------+------+\n";
}

bool processData(man m1, man m2, double ideal) {
    
}