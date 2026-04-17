#include <iostream>
#include <fstream>
#include <string>
// #include <locale>
#include <windows.h>

using namespace std;

// Funcs
int countDots(string);
void copyWithUppercase(string, string);

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    int choice;
    string fileName, fromFile, toFile;
    while (1) {
        cout << endl << endl << endl << "================ МЕНЮ ================" << endl;
        cout << "1. Подсчитать кол-во многоточий в файле" << endl;
        cout << "2. Скопировать содержимое одного файла в другой с заменой строчных гласных букв на заглавные" << endl;
        cout << "0. Выход" << endl;
        cout << "-> ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Введите название файла (без указания расширения .txt): ";
            cin >> fileName;
            fileName += ".txt";
            cout << "В файле " << countDots(fileName) << " многоточий" << endl;
            break;

        case 2:
            cout << "Введите название исходного файла (без указания расширения .txt): ";
            cin >> fromFile;
            cout << "Введите название копии файла (без указания расширения .txt): ";
            cin >> toFile;
            fromFile += ".txt";
            toFile += ".txt";
            copyWithUppercase(fromFile, toFile);
            break;

        case 0:
            cout << "Выход из программы...";
            return 0;
        
        default:
            cout << "Команда не распознана!" << endl;
            break;
        }
    }
}

int countDots(string fileName) {
    ifstream file(fileName, ios::in);
    int count = 0;
    if (!file) {
        cout << "Файл '" << fileName << "' не найден!" << endl;
    } else {
        string line;
        while (getline(file, line)) {
            int len = line.length();
            for (int i = 0; i < len - 2; i++) {
                if (line[i] == '.' && line[i+1] == '.' && line[i+2] == '.') count++;
            }
        }
    }
    file.close();
    return count;
}

void copyWithUppercase(string fromFile, string toFile = "copy.txt") {
    ifstream sourceFile(fromFile, ios::in);
    ofstream copyFile(toFile, ios::out);
    if (!sourceFile) {
        cout << "Файл '" << fromFile << "' не найден!" << endl;
        return;
    }
    if (!copyFile) {
        cout << "Ошибка открытия/создания файла '" << toFile << "' (файл для копии)" << endl;
        return;
    }
    char now;
    while (sourceFile.get(now)) {
        if (now == 'a' || now == 'e' || now == 'i' || now == 'o' || now == 'u' || now == 'y') now = now - 32;
        copyFile.put(now);
        
    }
    sourceFile.close();
    copyFile.close();
    cout << "Копия файла '" << fromFile << "' готова и помещена в файл '" << toFile << "'!" << endl;
}