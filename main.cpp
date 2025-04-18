#include <iostream>
#include <ranges>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <random>
#include <tuple>
#include "libs/termcolor/termcolor.hpp"

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
        std::cout << name << " is located at " << worldPos[0] << "," << worldPos[1] << "\n";
    }

    [[nodiscard]] const std::string& getName() const { return name; }
    ~Prop() = default;
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
        std::cout << "Do you want to pick up " << name << "? (yes/no): ";
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

    [[nodiscard]]bool isEquipped() const {
        return equipped;
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

    void dropItem(float x, float y) {
        equipped = false;
        worldPos = {x, y};
        std::cout << name << " has been dropped at [" << x << ", " << y << "]\n";
    }

    void upgradeItem(float percent) {
        if (percent <= 0) {
            std::cout << "Cannot upgrade item!";
            return ;
        }

        damage += damage * (percent / 100.0f);
        healAmount += healAmount * (percent / 100.0f);

        std::cout << name << " gained an improvement!" << termcolor::green <<  "View stats?(yes/no)" << termcolor::reset;
        std::string response;
        std::cin >> response;
        if (response == "yes")
            std::cout << "New stats:\n" << termcolor::red << " Damage = " << termcolor::reset << damage << termcolor::green << ", Heal = " << termcolor::reset << healAmount << "\n";
        else
            std::cout << "\n";
    }


    void degradeItem(float percent) {
        if (percent <= 0) {
        std::cout << "Error degrading item!";
        return ;}

        damage -= damage * (percent / 100.0f);
        healAmount -= healAmount * (percent / 100.0f);

        std::cout << name << " is wearing out! New stats:\n " << termcolor::red << "Damage = " << termcolor::reset << damage << termcolor::green << ", Heal = " << termcolor::reset << healAmount << "\n";
    }

    void specialAttack() const {
        if (name == "Excalibur") {
            float specialDamage = damage * 2.0f;
            std::cout << "Special attack with " << name << " applied! Damage: " << specialDamage << "\n";
        }
        else if (name == "Masamune") {
            float specialDamage = damage * 1.5f;
            std::cout << "Special attack with " << name << " applied! Damame: " << specialDamage << "\n";
        }
        else if (name == "Wallace") {
            float specialDamage = damage * 1.3f;
            std::cout << "Special attack with " << name << " applied! Damage: " << specialDamage << "\n";
        }
        else
            std::cout << name << " does not have any special attacks!\n";
    }

    [[nodiscard]] const std::string& getName() const {return name;}

    [[nodiscard]] float getHealingAmount() const { return healAmount;}

};

class Character {
private:
    int windowWidth;
    int windowHeight;
    std::tuple<float, float> screenPos;  // înlocuit vectorul
    float width{}, height{};
    float scale{4.0f};
    float health{100.0f};
    bool alive{true};
    Prop weapon;
    std::vector<Item> inventory;

    std::chrono::steady_clock::time_point lastHealTime;
    const int healIntervalSeconds = 5;

public:
    Character(int winWidth, int winHeight, const Prop& w)
        : windowWidth(winWidth), windowHeight(winHeight),
          screenPos(static_cast<float>(winWidth) / 2, static_cast<float>(winHeight) / 2),
          weapon(w), lastHealTime(std::chrono::steady_clock::now()) {}

    Character(const Character& other)
        : windowWidth(other.windowWidth), windowHeight(other.windowHeight),
          screenPos(other.screenPos), width(other.width), height(other.height),
          scale(other.scale), health(other.health), alive(other.alive),
          weapon(other.weapon), inventory(other.inventory), lastHealTime(other.lastHealTime) {}

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
            inventory = other.inventory;
            lastHealTime = other.lastHealTime;
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

        os << "screenPos: (" << std::get<0>(character.screenPos) << ", "
           << std::get<1>(character.screenPos) << "), ";

        os << "weapon: " << character.weapon << "\n";

        os << "Inventory:\n";
        if (character.inventory.empty()) {
            os << "  No items in inventory.\n";
        } else {
            for (const auto& item : character.inventory) {
                os << "  " << item << "\n";
            }
        }

        return os;
    }

    void autoHeal() {
        if (!alive) return;

        auto now = std::chrono::steady_clock::now();
        auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - lastHealTime).count();

        if (elapsedSeconds >= healIntervalSeconds) {
            heal(10.0f);
            lastHealTime = now;
        }
    }

    [[nodiscard]] float getHealth() const { return health; }

    void takeDamage(float damage) {
        health -= damage;
        if (health <= 0.f) {
            health = 0.f;
            alive = false;
        }
    }

    void move(float dx, float dy) {
        std::get<0>(screenPos) += dx;
        std::get<1>(screenPos) += dy;
    }

    [[nodiscard]] const std::tuple<float, float>& getPosition() const { return screenPos; }


    void heal(float amount) {
        health += amount;
        if (health > 100.0f) {
            health = 100.0f;
        }
        std::cout << "Character healed by " << amount << ". Current health: " << health << "\n";
    }

    void attack() const {
        std::cout << "Knight attacks with " << weapon.getName() << "!\n";
        weapon.printPos();
    }

    void addItemToInventory(const Item& item) {
        inventory.push_back(item);
        std::cout << item.getName() << " added to inventory.\n";
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
            std::cout << "Enemy attacks! You lost " << damagePerSec << " damage diva!\n";
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

class Obstacle {
private:
    std::vector<float> position;
    float width{2.0f};
    float height{2.0f};
    std::string name;

public:
    Obstacle(std::vector<float> pos, std::string n)
        : position(std::move(pos)), name(std::move(n)) {}

    Obstacle(const Obstacle& other)
        : position(other.position), width(other.width), height(other.height), name(other.name) {
        std::cout << "Constr de copiere Obstacol\n";
    }

    Obstacle& operator=(const Obstacle& other) {
        if (this != &other) {
            position = other.position;
            width = other.width;
            height = other.height;
            name = other.name;
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Obstacle& obs) {
        os << obs.name << ", position: [";
        for (size_t i = 0; i < obs.position.size(); i++) {
            os << obs.position[i];
            if (i < obs.position.size() - 1) os << ", ";
        }
        os << "], width: " << obs.width << ", height: " << obs.height;
        return os;
    }

   /* void printPos() const {
        if (position.size() < 2) {
            std::cout << "Invalid position for " << name << "\n";
            return;
        }
        std::cout << name << " is situated at " << position[0] << ", " << position[1] << "\n";
    } */

    [[nodiscard]] bool reach(float x, float y) const {
        return x >= position[0] && x <= position[0] + width && y >= position[1] && y <= position[1] + height;
    }

    void checkDangerZone(Character& character) const {
        auto [x, y] = character.getPosition();
        if (reach(x, y))
        {
            std::cout << "Character approached danger zone of " << name << "!\n";
            character.takeDamage(10.0f);
        }
    }

    ~Obstacle() = default;
};

int randomChoice() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> distrib(0, 1);

    return distrib(gen);
}

int main() {
    Item superSword("Excalibur", {10.0f, 20.0f}, 50.0f, 0.0f);
    Item superSword2("Wallace", {10.0f, 15.0f}, 40.0f, 0.0f);
    Prop sword({50.0f, 60.0f}, "Sword");
    Prop tree({100.0f, 200.0f}, "Oak");
    Prop tree2({25.0f, 35.0f}, "Birch");
    Character knight1{800, 600, sword};
    Enemy goblin({300.0f, 400.0f});
    Item potion("XP", {5.0f, 15.0f}, 0.0f, 25.0f);
    Obstacle obstacle1({8.0f, 8.0f}, "Obstacle1");
    Obstacle obstacle2({12.0f, 12.0f}, "Obstacle2");

    bool running = true;
    int cnt = 0;

    while (running) {
        auto frameStart = std::chrono::steady_clock::now();

        std::cout << "\nChoose an action: (1) Attack (2) Move (3) Equip Item (4) Use Potion (5) Quit\n";
        int choice;
        std::cin >> choice;
        if (cnt == 3)
            cnt = 0;
        else
            cnt++;

        if (choice != 3 and choice != 2) {
            if (knight1.getHealth() > 0) {
                knight1.takeDamage(1.0f);
                std::cout << "Character took damage! Health: " << knight1.getHealth() << "\n";
            }
            knight1.autoHeal();
            goblin.attack(knight1);
        }


        if (knight1.getHealth() <= 0) {
            std::cout << "Game Over!\n";
            //running = false;
            break;
        }

        switch (choice) {
            case 1:
                knight1.attack();
                break;
            case 2: {
                float dx, dy;
                std::cout << "Enter movement (dx dy): ";
                std::cin >> dx >> dy;
                knight1.move(dx, dy);
                std::cout << "New position: " << knight1 << "\n";
                if (randomChoice() == 1) {
                    if (knight1.getHealth() > 0) {
                        knight1.takeDamage(1.0f);
                        std::cout << "Character took damage! Health: " << knight1.getHealth() << "\n";
                    }
                    knight1.autoHeal();
                    goblin.attack(knight1);
                }
                break;
            }
            case 3:
                if (!superSword.isEquipped()) {
                    knight1.addItemToInventory(superSword);
                    std::cout << "Equipped Excalibur!\n";
                }
                break;
            case 4:
                knight1.heal(potion.getHealingAmount());
                std::cout << "Used potion! Health: " << knight1.getHealth() << "\n";
                break;
            case 5:
                running = false;
                break;
            default:
                std::cout << "Invalid choice!\n";
                running = false;
                break;
        }

        if (running == false)
            break;

        if (randomChoice()) obstacle1.checkDangerZone(knight1);
        if (randomChoice()) obstacle2.checkDangerZone(knight1);

        if (cnt == 3 and choice != 5) superSword.upgradeItem(10);
        if (randomChoice()) superSword.equipItem();
        if (randomChoice()) superSword2.degradeItem(5);

        if (randomChoice()) superSword.specialAttack();
        if (randomChoice()) superSword2.specialAttack();

        if (randomChoice()) potion.unequipItem();
        if (randomChoice()) potion.dropItem(10.0f, 20.0f);

        auto frameEnd = std::chrono::steady_clock::now();
        std::chrono::milliseconds frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
        std::this_thread::sleep_for(std::chrono::milliseconds(16) - frameDuration);
    }

    return 0;
}