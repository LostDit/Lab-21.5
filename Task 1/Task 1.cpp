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
        std::cerr << "������ �������� �����!" << std::endl;
        return;
    }

    std::vector<Record> records;
    Record rec;
    while (file >> rec.fName >> rec.lName >> rec.date >> rec.amount) {
        records.push_back(rec);
    }

    for (const auto& r : records) {
        std::cout << r.fName << " " << r.lName << " " << r.date << " "
            << std::fixed << std::setprecision(2) << r.amount << " ���." << std::endl;
    }
}

void addRecord(const std::string& filename) {
    setlocale(LC_ALL, "Russian");
    std::ofstream file(filename, std::ios::app);
    if (!file) {
        std::cerr << "������ �������� �����!" << std::endl;
        return;
    }

    Record rec;
    std::cout << "������� ���: ";
    std::cin >> rec.fName;
    std::cout << "������� �������: ";
    std::cin >> rec.lName;
    std::cout << "������� ���� (��.��.����): ";
    std::cin >> rec.date;
    std::cout << "������� �����: ";
    std::cin >> rec.amount;

    file << rec.fName << " " << rec.lName << " " << rec.date << " " << rec.amount << std::endl;
    std::cout << "������ ���������!" << std::endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::string command;
    std::string filename = "statement.txt";

    while (true) {
        std::cout << "������� ������� (list/add/cls(��� ������� ���������), 0 ��� ������): ";
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
            std::cerr << "����������� �������! ���������� �����." << std::endl;
            std::cin.clear(); // ����� ������ ������
            std::cin.ignore(10000, '\n'); // ������� ������� �����
        }
    }
    return 0;
}
