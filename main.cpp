#include <iostream>
#include <vector>
#include <string>

class Prop {
private:
    std::vector<float> worldPos;
    float scale{4.0f};
    std::string name;

public:

    Prop(std::vector<float> pos, std::string n) : worldPos(pos), name(n) {}

    Prop(const Prop& other) : worldPos(other.worldPos), scale(other.scale), name(other.name) {
        std::cout << "Constr de copiere Prop\n";
    }

    Prop& operator=(const Prop& other) {
        std::cout << "operator = copiere Prop\n";
        if (this != &other) {
            worldPos = other.worldPos;
            scale = other.scale;
            name = other.name;
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Prop& prop) {
        os <<  prop.name;
        os << ", worldPos: [";
        for (size_t i = 0; i < prop.worldPos.size(); i++) {
            os << prop.worldPos[i];
            if (i < prop.worldPos.size() - 1) os << ", ";
        }
        os << "], scale: " << prop.scale;
        return os;
    }

    void printPos() const {
        if (worldPos.size() < 2) {
            std::cout << "Invalid position for " << name << "\n";
            return;
        }
        std::cout << "The tree " << name << " is located at " << worldPos[0] << "," << worldPos[1] << "\n";
    }

    ~Prop() = default;
};

class Character {
private:
    int windowWidth;
    int windowHeight;
    std::vector<float> screenPos;
    float width{}, height{};
    float scale{4.0f};
    float health{100.0f};
    bool alive{true};
    Prop* weapon;

public:
    Character(int winWidth, int winHeight, Prop* weapon) : windowWidth(winWidth), windowHeight(winHeight), weapon(weapon),
                                             screenPos{float(winWidth) / 2, float(winHeight) / 2} {}

    Character(const Character& other) : windowWidth{other.windowWidth}, windowHeight{other.windowHeight},
                                        scale{other.scale}, health{other.health}, alive{other.alive},
                                        screenPos{other.screenPos}, width{other.width}, height{other.height} {
        std::cout << "Constr de copiere Character\n";
    }


    Character& operator=(const Character& other) {
        std::cout << "operator = copiere Character\n";
        if (this != &other) {
            windowWidth = other.windowWidth;
            windowHeight = other.windowHeight;
            scale = other.scale;
            health = other.health;
            alive = other.alive;
            screenPos = other.screenPos;
            width = other.width;
            height = other.height;
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Character& character) {
        os << "Character -> windowWidth: " << character.windowWidth << ", ";
        os << "windowHeight: " << character.windowHeight << ", ";
        os << "height: " << character.height << ", ";
        os << "width: " << character.width << ", ";
        os << "scale: " << character.scale << ", ";
        os << "health: " << character.health << ", ";
        os << "alive: " << (character.alive ? "Yes\n" : "No\n");

        os << "screenPos: [";
        for (size_t i = 0; i < character.screenPos.size(); i++) {
            os << character.screenPos[i];
            if (i < character.screenPos.size() - 1) os << ", ";
        }
        os << "], ";

        if (character.weapon)
            os << ", weapon: " << *character.weapon;
        else
            os << ", weapon: None";
        return os;
    }

    void takeDamage(float amount) {
        health -= amount;
        if (health <= 0) {
            health = 0;
            alive = false;
        }
    }

    void move(float dx, float dy) {
        screenPos[0] += dx;
        screenPos[1] += dy;
    }


    ~Character() = default;
};

class Enemy {
private:
    std::vector<float> worldPos;
    float width{}, height{};
    float speed{3.5f};
    float health{100.0f};
    bool alive{true};
    float damagePerSec{10.0f};
    float radius{50.0f};

public:
    Enemy(std::vector<float> pos) : worldPos(pos) {}

    Enemy(const Enemy& other) : worldPos(other.worldPos), width(other.width), height(other.height),
                                speed(other.speed), health(other.health), alive(other.alive),
                                damagePerSec(other.damagePerSec), radius(other.radius) {
        std::cout << "Constr de copiere Enemy\n";
    }

    Enemy& operator=(const Enemy& other) {
        std::cout << "operator = copiere Enemy\n";
        if (this != &other) {
            worldPos = other.worldPos;
            width = other.width;
            height = other.height;
            speed = other.speed;
            health = other.health;
            alive = other.alive;
            damagePerSec = other.damagePerSec;
            radius = other.radius;
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Enemy& enemy) {
        os << "Enemy -> worldPos: [";
        for (size_t i = 0; i < enemy.worldPos.size(); i++) {
            os << enemy.worldPos[i];
            if (i < enemy.worldPos.size() - 1) os << ", ";
        }
        os << "], width: " << enemy.width;
        os << ", height: " << enemy.height;
        os << ", speed: " << enemy.speed;
        os << ", health: " << enemy.health;
        os << ", alive: " << (enemy.alive ? "Yes" : "No");
        os << ", damagePerSec: " << enemy.damagePerSec;
        os << ", radius: " << enemy.radius;
        return os;
    }

    ~Enemy() = default;
};

int main() {
    Prop* sword = new Prop({50.0f, 60.0f}, "Sword");
    Character knight1{800, 600, sword};
    Character knight2 = knight1;
    Character knight3{1024, 768,sword};
    knight3 = knight1;

    std::cout << knight1 << "\n";

    knight1.takeDamage(20);
    std::cout << knight1 << "\n";

    knight1.move(10, -5);
    std::cout << knight1 << "\n";

    Enemy goblin({10.0f, 20.0f});
    Enemy goblin2 = goblin;
    Enemy goblin3({30.0f, 40.0f});
    goblin3 = goblin;

    std::cout << goblin << "\n";

    Prop tree({100.0f, 200.0f}, "Oak");
    tree.printPos();
    Prop tree2({25.0f, 35.0f}, "Birch");
    tree2 = tree;
    tree2.printPos();

    std::cout << tree << "\n";

    return 0;
}