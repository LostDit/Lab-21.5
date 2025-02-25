#include <iostream>
#include <vector>
#include <string>
#include <limits>

// ���� ������
enum class RoomType { Bedroom, Kitchen, Bathroom, Children, Living };

// �������
struct Room {
    RoomType type;
    double area;
    double ceilingHeight;
};

// ����
struct Floor {
    int number;
    std::vector<Room> rooms;
};

// ���� ��������
enum class BuildingType { House, Garage, Shed, Bathhouse };

// ���������
struct Building {
    BuildingType type;
    double area;
    bool hasStove = false;
    std::vector<Floor> floors;
};

// �������
struct Plot {
    int number;
    double totalArea;
    std::vector<Building> buildings;
};

// �������
struct Village {
    std::vector<Plot> plots;
};

// ������� ��� ����������� ����� �������� ��������
template<typename T>
T getValidatedInput(T min, T max) {
    T value;
    while (true) {
        std::cin >> value;
        if (std::cin.fail() || value < min || value > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "������������ ����! ��������� �������: ";
        }
        else {
            break;
        }
    }
    return value;
}

// ������� ��� ����� �������
Room createRoom() {
    Room room;
    std::cout << "�������� ��� ������� (0: �������, 1: �����, 2: ������, 3: �������, 4: ��������): ";
    int roomType = getValidatedInput(0, 4);
    room.type = static_cast<RoomType>(roomType);

    std::cout << "������� ������� ������� (>= 2): ";
    room.area = getValidatedInput(2.0, 1000.0);

    std::cout << "������� ������ ������� (2.0 - 5.0 �): ";
    room.ceilingHeight = getValidatedInput(2.0, 5.0);

    return room;
}

// ������� ��� ����� �����
Floor createFloor(int floorNumber) {
    Floor floor;
    floor.number = floorNumber;
    std::cout << "������� ���������� ������ �� " << floorNumber << " ����� (2-4): ";
    int roomsCount = getValidatedInput(2, 4);

    for (int j = 0; j < roomsCount; ++j) {
        floor.rooms.push_back(createRoom());
    }
    return floor;
}

// ������� ��� ����� ������
Building createBuilding(BuildingType type) {
    setlocale(LC_ALL, "Russian");
    Building b;
    b.type = type;
    std::cout << "������� ������� ��������� (>= 5): ";
    b.area = getValidatedInput(5.0, 10000.0);

    if (type == BuildingType::House || type == BuildingType::Bathhouse) {
        char hasStove;
        do {
            std::cout << "���� ���� � ������? (y/n): ";
            std::cin >> hasStove;
        } while (hasStove != 'y' && hasStove != 'n');
        b.hasStove = (hasStove == 'y');
    }

    if (type == BuildingType::House) {
        std::cout << "������� ���������� ������ (1-3): ";
        int floorsCount = getValidatedInput(1, 3);
        for (int i = 1; i <= floorsCount; ++i) {
            b.floors.push_back(createFloor(i));
        }
    }
    return b;
}

// ������� ��� �������� �������
Plot createPlot(int number) {
    setlocale(LC_ALL, "Russian");
    Plot p;
    p.number = number;
    std::cout << "������� ����� ������� ������� (>= 50): ";
    p.totalArea = getValidatedInput(50.0, 100000.0);

    std::cout << "������� ���������� �������� �� ������� (1-10): ";
    int buildingsCount = getValidatedInput(1, 10);

    for (int i = 0; i < buildingsCount; ++i) {
        std::cout << "�������� ��� ��������� (0: ���, 1: �����, 2: �����, 3: ����): ";
        int buildingType = getValidatedInput(0, 3);
        p.buildings.push_back(createBuilding(static_cast<BuildingType>(buildingType)));
    }
    return p;
}

int main() {
    setlocale(LC_ALL, "Russian");
    Village village;
    std::cout << "������� ���������� �������� � ������� (1-100): ";
    int plotCount = getValidatedInput(1, 100);

    for (int i = 1; i <= plotCount; ++i) {
        std::cout << "\n���������� ���������� ��� ������� " << i << ":\n";
        village.plots.push_back(createPlot(i));
    }

    std::cout << "\n������ � ������� ���������." << std::endl;
    return 0;
}