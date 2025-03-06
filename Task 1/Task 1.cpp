#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

struct Record {
    std::string fName;
    std::string lName;
    std::string date;
    double amount;
};

void listRecords(const std::string& filename) {
    setlocale(LC_ALL, "Russian");
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return;
    }

    std::vector<Record> records;
    Record rec;
    while (file >> rec.fName >> rec.lName >> rec.date >> rec.amount) {
        records.push_back(rec);
    }

    for (const auto& r : records) {
        std::cout << r.fName << " " << r.lName << " " << r.date << " "
            << std::fixed << std::setprecision(2) << r.amount << " руб." << std::endl;
    }
}

void addRecord(const std::string& filename) {
    setlocale(LC_ALL, "Russian");
    std::ofstream file(filename, std::ios::app);
    if (!file) {
        std::cerr << "Ошибка открытия файла!" << std::endl;
        return;
    }

    Record rec;
    std::cout << "Введите имя:";
    std::cin >> rec.fName;
    std::cout << "Введите фамилию: ";
    std::cin >> rec.lName;
    std::cout << "Введите дату (ДД.ММ.ГГГГ): ";
    std::cin >> rec.date;
    std::cout << "Введите сумму: ";
    std::cin >> rec.amount;

    file << rec.fName << " " << rec.lName << " " << rec.date << " " << rec.amount << std::endl;
    std::cout << "Запись добавлена!" << std::endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::string command;
    std::string filename = "statement.txt";

    while (true) {
        std::cout << "Введите команду (list/add/cls(для очистки терминала), 0 для выхода): ";
        std::cin >> command;

        if (command == "0") break;

        if (command == "list") {
            listRecords(filename);
        }
        else if (command == "add") {
            addRecord(filename);
        }
        else if (command == "cls") {
            system("cls");
        }
        else {
            std::cerr << "Неизвестная команда! Попробуйте снова." << std::endl;
            std::cin.clear(); // Сброс флагов ошибки
            std::cin.ignore(10000, '\n'); // Очистка лишнего ввода
        }
    }
    return 0;
}
