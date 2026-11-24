#include <iostream>
#include <typeinfo>
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
        virtual ~device() {
            cout << "  ~device(): " << name << "\n";
        }

        virtual void show() = 0;
};

class Clock : public device {
    protected:
        int hours = 0;

    public:
        Clock() : device("Часы") {
            cout << "  clock(): конструктор по умолчанию\n";
        }
        Clock(const string& n, int h) : device(n) {
            this->hours = h;
            cout << "  clock(\"" << n << "\", " << h << "): конструктор с параметром\n";
        }
        ~Clock() {
            cout << "  ~clock(): " << name << "\n";
        }
        void show() {
            cout << "  [clock]  Показание: " << hours << " ч.\n";
        }
};

class sandClock : public Clock {
    private:
        double sandVolume = 0;

    public:
        sandClock() : Clock("Песочные часы", 0) {
            cout << "  sandClock(): конструктор по умолчанию\n";
        }
        sandClock(const string& n, int h, double v) : Clock(n, h) {
            this->sandVolume = v;
            cout << "  sandClock(\"" << n << "\", " << h << ", " << v
                 << "): конструктор с параметром\n";
        }
        ~sandClock() {
            cout << "  ~sandClock(): " << name << "\n";
        }
        void show() {
            Clock::show();
            cout << "  [sandClock] Объём песка: " << sandVolume << " г.\n";
        }
};

int main() {
    sandClock obj1("Песочные часы на 5 минут", 0, 250.0);
    device *ptObj1 = &obj1;
    
    cout << endl;
    ptObj1->show();
    cout << "Тип объекта - " << typeid(ptObj1).name() << endl << endl;

    Clock obj2("Часики", 15);
    device *ptObj2 = &obj2;

    ptObj2->show();
    cout << "Тип объекта - " << typeid(ptObj2).name() << endl;

    cout << endl << "\n=== Завершение программы ===\n";
    return 0;
}