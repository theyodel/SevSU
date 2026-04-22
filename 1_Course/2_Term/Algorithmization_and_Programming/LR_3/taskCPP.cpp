#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

int countDots(string);
void copyWithUppercase(string, string);

int main()
{
    int choice, count;
    string fileName, fromFile, toFile;
    while (1)
    {
        cout << endl
             << endl
             << endl
             << "================ МЕНЮ ================" << endl;
        cout << "1. Подсчитать кол-во многоточий в файле" << endl;
        cout << "2. Скопировать содержимое одного файла в другой с заменой строчных гласных букв на заглавные" << endl;
        cout << "0. Выход" << endl;
        cout << "-> ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cout << "Введите название файла: ";
            cin >> fileName;
            if (fileName[fileName.length() - 4] != '.' && fileName[fileName.length() - 3] != 't' && fileName[fileName.length() - 2] != 'x' && fileName[fileName.length() - 1] != 't')
            {
                fileName += ".txt";
            }
            count = countDots(fileName);
            cout << "В файле " << count << " многоточий" << endl;
            break;

        case 2:
            cout << "Введите название исходного файла: ";
            cin >> fromFile;
            if (fromFile[fromFile.length() - 4] != '.' && fromFile[fromFile.length() - 3] != 't' && fromFile[fromFile.length() - 2] != 'x' && fromFile[fromFile.length() - 1] != 't')
            {
                fromFile += ".txt";
            }
            cout << "Введите название копии файла: ";
            cin >> toFile;
            if (toFile[toFile.length() - 4] != '.' && toFile[toFile.length() - 3] != 't' && toFile[toFile.length() - 2] != 'x' && toFile[toFile.length() - 1] != 't')
            {
                toFile += ".txt";
            }

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

/// @brief Функция подсчёта многоточий в каждой строке файла и во всём файле в целом
/// @param fileName Имя файла, в котором будет производится подсчёт многоточий
/// @return Кол-во всех многоточий (`...`) в файле
/// @attention Имя файла должно передаваться в функцию с расширением `.txt`
int countDots(string fileName)
{
    ifstream file(fileName, ios::in);
    int allDots = 0,
        line = 1,
        count = 0,
        flag = 0;
    if (!file)
    {
        cout << "Файл '" << fileName << "' не найден!" << endl;
    }
    else
    {
        int chr;
        while ((chr = file.get()) != EOF)
        {
            if (chr == '.')
            {
                flag++;
                if (flag == 3)
                {
                    count++;
                    flag = 0;
                }
            }
            else if (chr == '\n')
            {
                std::cout << "Строка " << line << ": найдено " << count << " многоточий\n";
                allDots += count;
                count = 0;
                flag = 0;
                line++;
            }
            else
            {
                flag = 0;
            }
        }
    }

    if (count > 0 || line == 1)
    {
        cout << "Строка " << line << ": найдено " << count << " многоточий" << endl;
        allDots += count;
    }

    file.close();
    return allDots;
}

/// @brief Функция копирования содержимого одного текстового файла в другой с изменением регистра гласных букв согласно варианту задания (alAa -> AlAA)
/// @param fromFile Имя исходного файла
/// @param toFile Имя файла, куда будет скопирован исправленный текст исходного файла
/// @attention Имена файлов должны передаваться в функцию с расширением `.txt`
void copyWithUppercase(string fromFile, string toFile = "copy.txt")
{
    ifstream sourceFile(fromFile, ios::in);
    ofstream copyFile(toFile, ios::out);
    if (!sourceFile)
    {
        cout << "Файл '" << fromFile << "' не найден!" << endl;
        return;
    }
    if (!copyFile)
    {
        cout << "Ошибка открытия/создания файла '" << toFile << "' (файл для копии)" << endl;
        return;
    }
    char now;
    while (sourceFile.get(now))
    {
        if (now == 'a' || now == 'e' || now == 'i' || now == 'o' || now == 'u' || now == 'y')
            now = now - 32;
        copyFile.put(now);
    }
    sourceFile.close();
    copyFile.close();
    cout << "Копия файла '" << fromFile << "' готова и помещена в файл '" << toFile << "'!" << endl;
}