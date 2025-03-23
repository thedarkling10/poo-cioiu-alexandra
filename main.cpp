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

    void takeDamage(float damage) {
        health -= damage;
        if (health <= 0.f) {
            health = 0.f;
            alive = false;
        }
    }

    void move(float dx, float dy) {
        screenPos[0] += dx;
        screenPos[1] += dy;
    }
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
    explicit Enemy(std::vector<float> pos) : worldPos(std::move(pos)) {}

    Enemy(const Enemy& other) : worldPos(other.worldPos), width(other.width), height(other.height),
                                speed(other.speed), health(other.health), alive(other.alive),
                                damagePerSec(other.damagePerSec), radius(other.radius) {
        std::cout << "Constr de copiere Enemy\n";
    }

    Enemy& operator=(const Enemy& other) {
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

    void attack(Character& target) const {
        if (alive) {
            std::cout << "Enemy attacks and deals " << damagePerSec << " damage!\n";
            target.takeDamage(damagePerSec);
        }
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

class Item {
    private:
    std::string name;
    std::vector<float> worldPos;
    float damage;
    float healAmount;
    bool equipped;

    public:
    Item(std::string n, std::vector<float> pos, float dmg, float heal)
        : name(std::move(n)), worldPos(std::move(pos)), damage(dmg), healAmount(heal), equipped(false) {

        std::string response;
        std::cout << "Do you want to equip " << name << "? (yes/no): ";
        std::cin >> response;

        if (response == "yes") {
            equipped = true;
            std::cout << name << " has been equipped!\n";
        } else {
            std::cout << name << " is not equipped.\n";
        }
    }
    Item(const Item& other)
        : name(other.name), worldPos(other.worldPos), damage(other.damage),
          healAmount(other.healAmount), equipped(other.equipped) {
        std::cout << "Constr de copiere Item\n";
    }

    Item& operator=(const Item& other) {
        std::cout << "Operator = copiere Item\n";
        if (this != &other) {
            name = other.name;
            worldPos = other.worldPos;
            damage = other.damage;
            healAmount = other.healAmount;
            equipped = other.equipped;
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Item& item) {
        os << "Item -> Name: " << item.name
           << ", Position: [";
        for (size_t i = 0; i < item.worldPos.size(); i++) {
            os << item.worldPos[i];
            if (i < item.worldPos.size() - 1) os << ", ";
        }
        os << "], Damage: " << item.damage
           << ", HealAmount: " << item.healAmount
           << ", Equipped: " << (item.equipped ? "Yes" : "No");
        return os;
    }

    ~Item() {
        std::cout << "Destructor Item\n";
    }

    void equipItem() {
        if (!equipped) {
            equipped = true;
            std::cout << name << " is now equipped!\n";
        } else {
            std::cout << name << " is already equipped!\n";
        }
    }

    void unequipItem() {
        if (equipped) {
            equipped = false;
            std::cout << name << " is now unequipped!\n";
        } else {
            std::cout << name << " is not equipped!\n";
        }
    }

};

int main() {
    Prop sword({50.0f, 60.0f}, "Sword");
    Character knight1{800, 600, sword};
    Character knight2 = knight1;

    std::cout << knight1 << "\n";
    std::cout << knight2 << "\n";

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

    Enemy goblin({300.0f, 400.0f});
    std::cout << goblin << "\n";

    goblin.attack(knight1);
    std::cout << knight1 << "\n";

    Item superSword("Excalibur", {10.0f, 20.0f}, 50.0f, 0.0f);
    Item potion("XP", {5.0f, 15.0f}, 0.0f, 25.0f);

    std::cout << superSword << "\n";
    std::cout << potion << "\n";

    Item updatedSword = superSword;
    potion = superSword;

    std::cout << potion << "\n";
    std::cout << updatedSword << "\n";

    updatedSword.equipItem();

    return 0;
}
