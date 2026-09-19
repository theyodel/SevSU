#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

struct clerk {
    int id;
    std::string familiya;
    std::string name;
    std::string job;
    int staje;
    std::string podjob;
    double oklad;
};

// Функция ввода данных
void inputclerk(clerk &c) {
    std::cout << "Порядковый номер: ";
    std::cin >> c.id;
    std::cout << "Фамилия: ";
    std::cin >> c.familiya;
    std::cout << "Имя: ";
    std::cin >> c.name;
    std::cout << "Должность: ";
    std::cin >> c.job;
    std::cout << "Стаж (лет): ";
    std::cin >> c.staje;
    std::cout << "Подразделение: ";
    std::cin >> c.podjob;
    std::cout << "Оклад: ";
    std::cin >> c.oklad;
    std::cout << "------------------------------\n";
}

// Функция вывода данных
void printclerk(const clerk &c) {
    std::cout << "№" << c.id << " | " << c.familiya << " " << c.name
              << " | " << c.job << " | Стаж: " << c.staje
              << " лет | Отдел: " << c.podjob << " | Оклад: " << c.oklad << "\n";
}

// Функция обработки по условию задания
void processclerks(const clerk clerks[], int size, double limit) {
    bool found = false;
    std::cout << "\nСлужащие с окладом меньше " << limit << ":\n";
    for (int i = 0; i < size; ++i) {
        if (clerks[i].oklad < limit) {
            std::cout << "- " << clerks[i].familiya << "\n";
            found = true;
        }
    }
    if (!found) {
        std::cout << "Служащие с окладом меньше " << limit << " не найдены.\n";
    }
}

int main() {
    // Настройка кодировки UTF-8 для вывода и ввода кириллицы
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    clerk c1{}, c2{};
    bool isC1Entered = false;
    bool isC2Entered = false;

    int choice = 0;
    while (true) {
        std::cout << "\n================ МЕНЮ ================\n";
        std::cout << "1. Ввод данных с клавиатуры\n";
        std::cout << "2. Вывод на дисплей введенных данных\n";
        std::cout << "3. Обработка данных (поиск по окладу)\n";
        std::cout << "0. Выход\n";
        std::cout << "Выберите пункт: ";
        std::cin >> choice;
        std::cout << "======================================\n\n";

        if (choice == 1) {
            std::cout << "=== Ввод данных служащего 1 ===\n";
            inputclerk(c1);
            isC1Entered = true;

            std::cout << "=== Ввод данных служащего 2 ===\n";
            inputclerk(c2);
            isC2Entered = true;

            std::cout << "Данные успешно внесены!\n";
        }
        else if (choice == 2) {
            if (!isC1Entered || !isC2Entered) {
                std::cout << "Ошибка: Сначала введите данные (пункт 1)!\n";
            } else {
                std::cout << "=== Введенные данные ===\n";
                printclerk(c1);
                printclerk(c2);
            }
        }
        else if (choice == 3) {
            if (!isC1Entered || !isC2Entered) {
                std::cout << "Ошибка: Сначала введите данные (пункт 1)!\n";
            } else {
                double limit;
                std::cout << "Введите порог оклада для поиска: ";
                std::cin >> limit;

                clerk clerks[2] = {c1, c2};
                processclerks(clerks, 2, limit);
            }
        }
        else if (choice == 0) {
            std::cout << "Завершение работы программы.\n";
            break;
        }
        else {
            std::cout << "Неверный пункт меню. Попробуйте снова.\n";
        }
    }

    return 0;
}