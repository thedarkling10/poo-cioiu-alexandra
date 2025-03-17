#include <iostream>
#include <vector>
#include <string>

class Prop {
private:
    std::vector<float> worldPos;
    float scale{4.0f};
    std::string name;

public:
    Prop(std::vector<float> pos, std::string n)
        : worldPos(std::move(pos)), name(std::move(n)) {}

    Prop(const Prop& other) : worldPos(other.worldPos), scale(other.scale), name(other.name) {
        std::cout << "Constr de copiere Prop\n";
    }

    Prop& operator=(const Prop& other) {
        if (this != &other) {
            worldPos = other.worldPos;
            scale = other.scale;
            name = other.name;
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Prop& prop) {
        os << prop.name << ", worldPos: [";
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
    Prop weapon;

public:
    Character(int winWidth, int winHeight, const Prop& w)
        : windowWidth(winWidth), windowHeight(winHeight),
          screenPos{static_cast<float>(winWidth) / 2, static_cast<float>(winHeight) / 2},
          weapon(w) {}

    Character(const Character& other)
        : windowWidth(other.windowWidth), windowHeight(other.windowHeight),
          screenPos(other.screenPos), width(other.width), height(other.height),
          scale(other.scale), health(other.health), alive(other.alive),
          weapon(other.weapon) {
        std::cout << "Constr de copiere Character\n";
    }

    Character& operator=(const Character& other) {
        if (this != &other) {
            windowWidth = other.windowWidth;
            windowHeight = other.windowHeight;
            screenPos = other.screenPos;
            width = other.width;
            height = other.height;
            scale = other.scale;
            health = other.health;
            alive = other.alive;
            weapon = other.weapon;
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
        os << "alive: " << (character.alive ? "Yes" : "No") << ", ";

        os << "screenPos: [";
        for (size_t i = 0; i < character.screenPos.size(); i++) {
            os << character.screenPos[i];
            if (i < character.screenPos.size() - 1) os << ", ";
        }
        os << "], weapon: " << character.weapon;
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
};

int main() {
    Prop sword({50.0f, 60.0f}, "Sword");
    Character knight1{800, 600, sword};
    Character knight2 = knight1;

    std::cout << knight1 << "\n";

    knight1.takeDamage(20);
    std::cout << knight1 << "\n";

    knight1.move(10, -5);
    std::cout << knight1 << "\n";

    Prop tree({100.0f, 200.0f}, "Oak");
    tree.printPos();
    Prop tree2({25.0f, 35.0f}, "Birch");
    tree2 = tree;
    tree2.printPos();

    std::cout << tree << "\n";

    return 0;
}
