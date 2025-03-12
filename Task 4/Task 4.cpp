#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <limits>
#include <cctype>
using namespace std;

const int MAP_SIZE = 20;
const int NUM_ENEMIES = 5;
const char PLAYER_MARK = 'P';
const char ENEMY_MARK = 'E';
const char EMPTY_MARK = '.';

//Структура позиции передвижения
struct Position {
    int x;
    int y;
};

//Обозначил структуру по статам
struct Character {
    string name;
    int health;
    int armor;
    int damage;
    Position pos;
    bool isPlayer;
};

// Функция для генерации случайного числа в диапазоне [min, max]
int randomInt(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Проверка, что позиция внутри карты
bool isInside(const Position& p) {
    return p.x >= 0 && p.x < MAP_SIZE && p.y >= 0 && p.y < MAP_SIZE;
}

// Функция отображения карты и персонажей
void drawMap(const vector<Character>& characters) {
    
    char map[MAP_SIZE][MAP_SIZE];
    for (int i = 0; i < MAP_SIZE; ++i)
        for (int j = 0; j < MAP_SIZE; ++j)
            map[i][j] = EMPTY_MARK;

    
    for (const auto& c : characters) {
        if (c.health <= 0)
            continue;
        
        if (c.isPlayer)
            map[c.pos.y][c.pos.x] = PLAYER_MARK;
        else
            map[c.pos.y][c.pos.x] = ENEMY_MARK;
    }

    // Вывод карты
    for (int i = 0; i < MAP_SIZE; ++i) {
        for (int j = 0; j < MAP_SIZE; ++j) {
            cout << map[i][j] << " ";
        }
        cout << "\n";
    }
    cout << endl;
}

// Функция атаки: цель получает урон, который вычитается сначала из брони, затем из здоровья.
void attack(Character& attacker, Character& target) {
    setlocale(LC_ALL, "Russian");
    
    if (target.health <= 0) return;

    int dmg = attacker.damage;
    if (target.armor >= dmg) {
        target.armor -= dmg;
    }
    else {
        int remaining = dmg - target.armor;
        target.armor = 0;
        target.health -= remaining;
    }
    cout << attacker.name << " атакует " << target.name << " и наносит "
        << dmg << " урона.\n";
    if (target.health <= 0)
        cout << target.name << " уничтожен!\n";
}

// Функция сохранения состояния игры
void saveGame(const vector<Character>& characters) {
    setlocale(LC_ALL, "Russian");
    
    ofstream file("save.bin", ios::binary);
    if (!file) {
        cout << "Ошибка при открытии файла для сохранения.\n";
        return;
    }
    size_t count = characters.size();
    file.write((char*)&count, sizeof(count));
    for (auto& c : characters) {
        file.write((char*)&c.isPlayer, sizeof(c.isPlayer));
        size_t nameLength = c.name.size();
        file.write((char*)&nameLength, sizeof(nameLength));
        file.write(c.name.c_str(), nameLength);
        file.write((char*)&c.health, sizeof(c.health));
        file.write((char*)&c.armor, sizeof(c.armor));
        file.write((char*)&c.damage, sizeof(c.damage));
        file.write((char*)&c.pos, sizeof(c.pos));
    }
    file.close();
    cout << "Игра сохранена.\n";
}

// Функция загрузки состояния игры
bool loadGame(vector<Character>& characters) {
    setlocale(LC_ALL, "Russian");

    ifstream file("save.bin", ios::binary);
    if (!file) {
        cout << "Файл сохранения не найден.\n";
        return false;
    }
    characters.clear();
    size_t count;
    file.read((char*)&count, sizeof(count));
    for (size_t i = 0; i < count; i++) {
        Character c;
        file.read((char*)&c.isPlayer, sizeof(c.isPlayer));
        size_t nameLength;
        file.read((char*)&nameLength, sizeof(nameLength));
        char* buffer = new char[nameLength + 1];
        file.read(buffer, nameLength);
        buffer[nameLength] = '\0';
        c.name = string(buffer);
        delete[] buffer;
        file.read((char*)&c.health, sizeof(c.health));
        file.read((char*)&c.armor, sizeof(c.armor));
        file.read((char*)&c.damage, sizeof(c.damage));
        file.read((char*)&c.pos, sizeof(c.pos));
        characters.push_back(c);
    }
    file.close();
    cout << "Игра загружена.\n";
    return true;
}

// Функция перемещения персонажа с проверкой границ.
// Используем команды: W (вверх), A (влево), S (вниз), D (вправо).
Position movePosition(const Position& current, char direction) {
    Position next = current;
    switch (direction) {
    case 'A': next.x -= 1; break;
    case 'D': next.x += 1; break;
    case 'W': next.y -= 1; break;
    case 'S': next.y += 1; break;
    default: break;
    }
    if (isInside(next))
        return next;
    else
        return current;
}

// Проверка, занята ли клетка другим живым персонажем; возвращает индекс или -1.
int getCharacterAt(const vector<Character>& characters, const Position& pos) {
    for (size_t i = 0; i < characters.size(); i++) {
        if (characters[i].health > 0 &&
            characters[i].pos.x == pos.x &&
            characters[i].pos.y == pos.y)
            return i;
    }
    return -1;
}

// Функция для безопасного ввода целого числа с проверкой. (добавил на всякий)
void inputInt(const string& prompt, int& value) {
    setlocale(LC_ALL, "Russian");

    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cout << "Некорректный ввод. Введите число.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    srand(static_cast<unsigned int>(time(0)));
    vector<Character> characters;

    // Создаем противников
    for (int i = 0; i < NUM_ENEMIES; i++) {
        Character enemy;
        enemy.name = "Enemy #" + to_string(i + 1);
        enemy.health = randomInt(50, 150);
        enemy.armor = randomInt(0, 50);
        enemy.damage = randomInt(15, 30);
        enemy.isPlayer = false;
        while (true) {
            enemy.pos.x = randomInt(0, MAP_SIZE - 1);
            enemy.pos.y = randomInt(0, MAP_SIZE - 1);
            if (getCharacterAt(characters, enemy.pos) == -1)
                break;
        }
        characters.push_back(enemy);
    }

    // Создаем игрока
    Character player;
    player.isPlayer = true;
    cout << "Введите имя игрока: ";
    getline(cin, player.name);

    inputInt("Введите здоровье игрока: ", player.health);
    inputInt("Введите броню игрока: ", player.armor);
    inputInt("Введите урон игрока: ", player.damage);

    while (true) {
        player.pos.x = randomInt(0, MAP_SIZE - 1);
        player.pos.y = randomInt(0, MAP_SIZE - 1);
        if (getCharacterAt(characters, player.pos) == -1)
            break;
    }
    characters.push_back(player);

    bool gameOver = false;
    while (!gameOver) {
        drawMap(characters);
        cout << "\nСтатус игрока: " << player.name
            << " Здоровье: " << player.health
            << " Броня: " << player.armor
            << " Урон: " << player.damage << "\n";
        cout << "Введите команду (W - вверх, A - влево, S - вниз, D - вправо, save, load): ";
        string command;
        getline(cin, command);

        if (command == "save") {
            saveGame(characters);
            continue;
        }
        else if (command == "load") {
            if (loadGame(characters)) {
                // После загрузки обновляем параметры игрока
                for (auto& c : characters) {
                    if (c.isPlayer) {
                        player = c;
                        break;
                    }
                }
            }
            continue;
        }

        char move = toupper(command[0]);
        if (move != 'W' && move != 'A' && move != 'S' && move != 'D') {
            cout << "Неверная команда!\n";
            continue;
        }

        // Находим индекс игрока
        int playerIndex = -1;
        for (size_t i = 0; i < characters.size(); i++) {
            if (characters[i].isPlayer) {
                playerIndex = i;
                break;
            }
        }
        if (playerIndex == -1) {
            cout << "Ошибка: игрок не найден!\n";
            break;
        }

        // Передвижение игрока
        Position newPos = movePosition(characters[playerIndex].pos, move);
        if (newPos.x == characters[playerIndex].pos.x && newPos.y == characters[playerIndex].pos.y) {
            cout << "Ход невозможен (выход за границы)!\n";
        }
        else {
            int otherIndex = getCharacterAt(characters, newPos);
            if (otherIndex != -1) {
                // Если цель - враг, атакуем его
                if (!characters[otherIndex].isPlayer) {
                    attack(characters[playerIndex], characters[otherIndex]);
                    if (characters[otherIndex].health <= 0) {
                        characters[playerIndex].pos = newPos;
                    }
                }
            }
            else {
                characters[playerIndex].pos = newPos;
            }
        }

        // Обновляем игрока после хода
        player = characters[playerIndex];
        if (player.health <= 0) {
            cout << "Ваш персонаж погиб. Игра окончена. Поражение!\n";
            gameOver = true;
            break;
        }

        // Передвижение противников
        for (size_t i = 0; i < characters.size(); i++) {
            if (characters[i].isPlayer || characters[i].health <= 0)
                continue;
            char enemyMove;
            int r = randomInt(0, 3);
            switch (r) {
            case 0: enemyMove = 'A'; break;
            case 1: enemyMove = 'D'; break;
            case 2: enemyMove = 'W'; break;
            case 3: enemyMove = 'S'; break;
            }
            Position enemyNewPos = movePosition(characters[i].pos, enemyMove);
            // Если новое положение совпадает с исходным, ход не выполняется.
            if (enemyNewPos.x == characters[i].pos.x && enemyNewPos.y == characters[i].pos.y)
                continue;

            int targetIndex = getCharacterAt(characters, enemyNewPos);
            // Если новый ход ведет к столкновению с игроком, то враг атакует игрока.
            if (targetIndex != -1 && characters[targetIndex].isPlayer) {
                attack(characters[i], characters[targetIndex]);
                // Если игрок погиб, завершаем игру.
                if (characters[targetIndex].health <= 0) {
                    cout << "Ваш персонаж погиб под атакой " << characters[i].name << ". Игра окончена.\n";
                    gameOver = true;
                    break;
                }
                // В данном случае враг не перемещается, если атаковал игрока.
            }
            // Если клетка занята другим врагом, враг пропускает ход.
            else if (targetIndex != -1) {
                continue;
            }
            else {
                // Если клетка пуста, перемещаем врага.
                characters[i].pos = enemyNewPos;
            }
        }

        if (gameOver)
            break;

        // Проверяем, уничтожены ли все враги
        bool allEnemiesDead = true;
        for (const auto& c : characters) {
            if (!c.isPlayer && c.health > 0) {
                allEnemiesDead = false;
                break;
            }
        }
        if (allEnemiesDead) {
            cout << "Все противники уничтожены. Вы победили!\n";
            gameOver = true;
        }
    }
    return 0;
}
