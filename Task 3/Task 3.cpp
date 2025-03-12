#include <iostream>
#include <string>
#include <cmath>
#include <limits>

struct Vector {
    double x;
    double y;

    Vector(double x = 0, double y = 0) : x(x), y(y) {}
};

Vector add(const Vector& a, const Vector& b) {
    return Vector(a.x + b.x, a.y + b.y);
}

Vector subtract(const Vector& a, const Vector& b) {
    return Vector(a.x - b.x, a.y - b.y);
}

Vector scale(const Vector& v, double scalar) {
    return Vector(v.x * scalar, v.y * scalar);
}

double length(const Vector& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

Vector normalize(const Vector& v) {
    double len = length(v);
    if (len == 0) {
        std::cerr << "������� ������ ������������� �� ���������";
        return Vector(0, 0);
    }
    return Vector(v.x / len, v.y / len);
}


template <typename T>
T safeInput(const std::string& prompt) {
    T value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        else {
            system("cls");
            std::cout << "�������� ����, ���������� ��� ���." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}


Vector inputVector(const std::string& prompt) {
    double x = safeInput<double>(prompt + " (���������� x): ");
    double y = safeInput<double>(prompt + " (���������� y): ");
    return Vector(x, y);
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::string operation;
    Vector v1, v2;
    double scalar;

    std::cout << "�������� ���� �������� - ������� 'add'" << "\n��������� ���� �������� - ������� 'subtract'"
        << "\n��������� ������� �� ������ - ������� 'scale'" << "\n���������� ����� ������� - ������� 'length'"
        << "\n������������ ������� - ������� 'normalize'";
    std::cout << "\n��� ������ �������� 'exit'" << std::endl;

    while (true) {
        std::cout << "\n������� �������� ��� ���������: ";
        std::getline(std::cin, operation);

        if (operation == "add") {

            v1 = inputVector("������� ������ 1");
            v2 = inputVector("������� ������ 2");

            Vector result = add(v1, v2);
            std::cout << "\nResult: (" << result.x << ", " << result.y << ")" << std::endl;

        }else if (operation == "subtract") {

            v1 = inputVector("������� ������ 1");
            v2 = inputVector("������� ������ 2");

            Vector result = subtract(v1, v2);
            std::cout << "\nResult: (" << result.x << ", " << result.y << ")" << std::endl;

        }else if (operation == "scale") {
            v1 = inputVector("������� ������");

            scalar = safeInput<double>("\n������� ������: ");
            Vector result = scale(v1, scalar);
            std::cout << "\nResult: (" << result.x << ", " << result.y << ")" << std::endl;

        }else if (operation == "length") {

            v1 = inputVector("������� ������");
            double result = length(v1);
            std::cout << "\n�����: " << result << std::endl;

        }else if (operation == "normalize") {

            v1 = inputVector("������� ������");
            Vector result = normalize(v1);
            std::cout << "\n��������������� ������: (" << result.x << ", " << result.y << ")" << std::endl;

        }else if (operation == "exit") {
            
            std::cout << "\n�� ����� �� ���������." << std::endl;
            break;

        }else {
            system("cls");
            std::cout << "�������� �������!" << std::endl;
            std::cout << "\n�������� ���� �������� - ������� 'add'" << "\n��������� ���� �������� - ������� 'subtract'"
                << "\n��������� ������� �� ������ - ������� 'scale'" << "\n���������� ����� ������� - ������� 'length'"
                << "\n������������ ������� - ������� 'normalize'" << "\n��� ������ �������� 'exit'" << std::endl;
        }
    }
    return 0;
}