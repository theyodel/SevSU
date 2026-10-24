#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

class device {
    protected:
        string name = "Устройство";

    public:
        device() {
            cout << "  device(): конструктор по умолчанию\n";
        }
        device(string n) {
            this->name = n;
            cout << "  device(\"" << n << "\"): конструктор с параметром\n";
        }
        ~device() {
            cout << "  ~device(): " << name << "\n";
        }

        void show() {
            cout << "  [device] Название: " << name << "\n";
        }
        string getName() { return name; }
};

class clock : public device {
    protected:
        int hours = 0;

    public:
        clock() : device("Часы") {
            cout << "  clock(): конструктор по умолчанию\n";
        }
        clock(const string& n, int h) : device(n) {
            this->hours = h;
            cout << "  clock(\"" << n << "\", " << h << "): конструктор с параметром\n";
        }
        ~clock() {
            cout << "  ~clock(): " << name << "\n";
        }
        void show() {
            device::show();
            cout << "  [clock]  Показание: " << hours << " ч.\n";
        }
        int getHours() { return hours; }
};

class sandClock : public clock {
    private:
        double sandVolume = 0;

    public:
        sandClock() : clock("Песочные часы", 0) {
            cout << "  sandClock(): конструктор по умолчанию\n";
        }
        sandClock(const string& n, int h, double v) : clock(n, h) {
            this->sandVolume = v;
            cout << "  sandClock(\"" << n << "\", " << h << ", " << v
                 << "): конструктор с параметром\n";
        }
        ~sandClock() {
            cout << "  ~sandClock(): " << name << "\n";
        }
        void show() {
            clock::show();
            cout << "  [sandClock] Объём песка: " << sandVolume << " г.\n";
        }
        double getSandVolume() const { return sandVolume; }
};

int main() {
    cout << "=== 1. Создание объекта obj1 (конструктор по умолчанию) ===\n";
    sandClock obj1;

    cout << "\n=== 2. Создание объекта obj2 (конструктор с параметрами) ===\n";
    sandClock obj2("Песочные часы на 5 минут", 0, 250.0);

    cout << "\n=== 3. Вызов переопределённого метода obj1.show() ===\n";
    obj1.show();

    cout << "\n=== 4. Вызов переопределённого метода obj2.show() ===\n";
    obj2.show();

    cout << "\n=== 5. Явный вызов метода УРОВНЯ 2: obj2.clock::show() ===\n";
    obj2.clock::show();

    cout << "\n=== 6. Явный вызов метода УРОВНЯ 1: obj2.device::show() ===\n";
    obj2.device::show();

    cout << "\n=== 7. Полиморфный вызов через указатель на device* ===\n";
    device* p = &obj2;
    p->show();

    cout << "\n=== 8. Доступ к protected/private через геттеры ===\n";
    cout << "  Имя:   " << obj2.getName()       << "\n";
    cout << "  Часы:  " << obj2.getHours()      << "\n";
    cout << "  Песок: " << obj2.getSandVolume() << " г\n";

    cout << "\n=== Завершение программы ===\n";
    return 0;
}