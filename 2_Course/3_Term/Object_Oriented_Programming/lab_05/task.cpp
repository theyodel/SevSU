#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// Дерево
class Tree {
protected:
    int height;
    string type;

public:
    Tree(int, string);
    Tree();
    ~Tree();
    void outputData();
    int getHeight();
};

Tree::Tree(int _height, string _type) {
    this->height = _height;
    this->type = _type;

    cout << " [ Tree ] Конструктор с параметрами" << endl;
}

Tree::Tree() {
    this->height = 100;
    this->type = "oak";

    cout << " [ Tree ] Конструктор по умолчанию" << endl;
}

Tree::~Tree() {
    cout << " [ ~Tree ] Деструктор" << endl;
}

void Tree::outputData() {
    cout << "Древесина " << this->type
         << " высотою " << this->height << " метров!" << endl;
}

int Tree::getHeight() {
    return this->height;
}

// Человек
class Human {
protected:
    string name;
    int age;

public:
    Human(int, string);
    Human();
    ~Human();
    void outputData();
    int getAge();
};

Human::Human(int _age, string _name) {
    this->name = _name;
    this->age = _age;

    cout << " [ Human ]: Конструктор с параметрами" << endl;
}

Human::Human() {
    this->name = "James";
    this->age = 25;

    cout << " [ Human ] Конструктор по умолчанию" << endl;
}

Human::~Human() {
    cout << " [ ~Human ] Деструктор" << endl;
}

int Human::getAge() {
    return this->age;
}

// Энт
class Ent : public Tree, public Human {
private:
    string role;
    bool friendly;

    // Дружественная функция
    friend void outputData(const Ent& ent);

public:
    Ent();
    Ent(int, string, int, string, string, bool);
    ~Ent();
    bool isFriendly();
};

Ent::Ent(int _age, string _name, int _height, string _type, string _role, bool _friendly)
    : Tree(_height, _type), Human(_age, _name)
{
    this->role = _role;
    this->friendly = _friendly;

    cout << " [ Ent ]: Конструктор с параметрами" << endl;
}

Ent::Ent()
    : Tree(), Human()
{
    this->role = "social";
    this->friendly = false;

    cout << " [ Ent ]: Конструктор по умолчанию" << endl;
}

Ent::~Ent() {
    cout << " [ ~Ent ] Деструктор" << endl;
}

void outputData(const Ent& ent) {
    cout << "Мифическое существо \"Энт\" по имени " << ent.name
         << " высотой " << ent.height << " метров, в возрасте " << ent.age
         << " из древесины " << ent.type
         << ", роль: " << ent.role
         << ", дружелюбный: " << (ent.friendly ? "да" : "нет") << endl;
}

bool Ent::isFriendly() {
    return this->friendly;
}

// main
int main() {
    Ent ent1; // конструктор по умолчанию
    Ent ent2(1021, "Трибород", 15, "дуб", "leader", true); // конструктор с параметрами
    int choice;

    while (true) {
        cout << "\n--=== МЕНЮ ===---" << endl;
        cout << "1. Вызов дружественной функции outputData(...)" << endl;
        cout << "2. Вызов метода .getHeight()" << endl;
        cout << "3. Вызов метода .getAge()" << endl;
        cout << "4. Вызов метода .isFriendly()" << endl;
        cout << "5. Выход" << endl << "-> ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Для объекта ent1:" << endl;
            outputData(ent1);
            cout << "\nДля объекта ent2:" << endl;
            outputData(ent2);
            break;

        case 2:
            cout << "Для объекта ent1:" << endl;
            cout << ent1.getHeight() << endl;
            cout << "\nДля объекта ent2:" << endl;
            cout << ent2.getHeight() << endl;
            break;

        case 3:
            cout << "Для объекта ent1:" << endl;
            cout << ent1.getAge() << endl;
            cout << "\nДля объекта ent2:" << endl;
            cout << ent2.getAge() << endl;
            break;

        case 4:
            cout << "Для объекта ent1:" << endl;
            cout << ((ent1.isFriendly() == true) ? "true" : "false") << endl;
            cout << "\nДля объекта ent2:" << endl;
            cout << ((ent2.isFriendly() == true) ? "true" : "false") << endl;
            break;

        case 5:
            cout << "Завершение программы" << endl;
            return 0;

        default:
            cout << "Неизвестная команда" << endl;
            break;
        }
    }
}