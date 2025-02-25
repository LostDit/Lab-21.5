#include <iostream>
#include <vector>
#include <string>
#include <limits>

// Типы комнат
enum class RoomType { Bedroom, Kitchen, Bathroom, Children, Living };

// Комната
struct Room {
    RoomType type;
    double area;
    double ceilingHeight;
};

// Этаж
struct Floor {
    int number;
    std::vector<Room> rooms;
};

// Типы построек
enum class BuildingType { House, Garage, Shed, Bathhouse };

// Постройка
struct Building {
    BuildingType type;
    double area;
    bool hasStove = false;
    std::vector<Floor> floors;
};

// Участок
struct Plot {
    int number;
    double totalArea;
    std::vector<Building> buildings;
};

// Поселок
struct Village {
    std::vector<Plot> plots;
};

// Функция для безопасного ввода числовых значений
template<typename T>
T getValidatedInput(T min, T max) {
    T value;
    while (true) {
        std::cin >> value;
        if (std::cin.fail() || value < min || value > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Некорректный ввод! Повторите попытку: ";
        }
        else {
            break;
        }
    }
    return value;
}

// Функция для ввода комнаты
Room createRoom() {
    Room room;
    std::cout << "Выберите тип комнаты (0: Спальня, 1: Кухня, 2: Ванная, 3: Детская, 4: Гостиная): ";
    int roomType = getValidatedInput(0, 4);
    room.type = static_cast<RoomType>(roomType);

    std::cout << "Введите площадь комнаты (>= 2): ";
    room.area = getValidatedInput(2.0, 1000.0);

    std::cout << "Введите высоту потолка (2.0 - 5.0 м): ";
    room.ceilingHeight = getValidatedInput(2.0, 5.0);

    return room;
}

// Функция для ввода этажа
Floor createFloor(int floorNumber) {
    Floor floor;
    floor.number = floorNumber;
    std::cout << "Введите количество комнат на " << floorNumber << " этаже (2-4): ";
    int roomsCount = getValidatedInput(2, 4);

    for (int j = 0; j < roomsCount; ++j) {
        floor.rooms.push_back(createRoom());
    }
    return floor;
}

// Функция для ввода здания
Building createBuilding(BuildingType type) {
    setlocale(LC_ALL, "Russian");
    Building b;
    b.type = type;
    std::cout << "Введите площадь постройки (>= 5): ";
    b.area = getValidatedInput(5.0, 10000.0);

    if (type == BuildingType::House || type == BuildingType::Bathhouse) {
        char hasStove;
        do {
            std::cout << "Есть печь с трубой? (y/n): ";
            std::cin >> hasStove;
        } while (hasStove != 'y' && hasStove != 'n');
        b.hasStove = (hasStove == 'y');
    }

    if (type == BuildingType::House) {
        std::cout << "Введите количество этажей (1-3): ";
        int floorsCount = getValidatedInput(1, 3);
        for (int i = 1; i <= floorsCount; ++i) {
            b.floors.push_back(createFloor(i));
        }
    }
    return b;
}

// Функция для создания участка
Plot createPlot(int number) {
    setlocale(LC_ALL, "Russian");
    Plot p;
    p.number = number;
    std::cout << "Введите общую площадь участка (>= 50): ";
    p.totalArea = getValidatedInput(50.0, 100000.0);

    std::cout << "Введите количество построек на участке (1-10): ";
    int buildingsCount = getValidatedInput(1, 10);

    for (int i = 0; i < buildingsCount; ++i) {
        std::cout << "Выберите тип постройки (0: Дом, 1: Гараж, 2: Сарай, 3: Баня): ";
        int buildingType = getValidatedInput(0, 3);
        p.buildings.push_back(createBuilding(static_cast<BuildingType>(buildingType)));
    }
    return p;
}

int main() {
    setlocale(LC_ALL, "Russian");
    Village village;
    std::cout << "Введите количество участков в поселке (1-100): ";
    int plotCount = getValidatedInput(1, 100);

    for (int i = 1; i <= plotCount; ++i) {
        std::cout << "\nЗаполнение информации для участка " << i << ":\n";
        village.plots.push_back(createPlot(i));
    }

    std::cout << "\nДанные о поселке сохранены." << std::endl;
    return 0;
}
