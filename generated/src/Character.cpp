#include "../include/Character.h"
#include "../include/Exceptions.h"
#include <iostream>
#include <utility>

GameEntity::GameEntity(std::vector<float> pos, std::string n, float s)
    : position(std::move(pos)), name(std::move(n)), scale(s) {}

void GameEntity::printPosition() const {
    if (position.size() >= 2) {
        std::cout << name << " position: ["
                  << position[0] << ", " << position[1] << "]\n";
    }
}

std::ostream& operator<<(std::ostream& os, const GameEntity& entity) {
    os << entity.name << " [";
    for (size_t i = 0; i < entity.position.size(); ++i) {
        os << entity.position[i];
        if (i < entity.position.size() - 1) os << ", ";
    }
    os << "] Scale: " << entity.scale;
    return os;
}

// Static member initialization
const int Character::healIntervalSeconds;

// Constructor
Character::Character(int winWidth, int winHeight, std::unique_ptr<GameEntity> w)
    : GameEntity({}, "Character", 4.0f), // Initialize base class
      windowWidth(winWidth),
      windowHeight(winHeight),
      screenPos(static_cast<float>(winWidth) / 2, static_cast<float>(winHeight) / 2),
      width(0.0f),
      height(0.0f),
      health(100.0f),
      alive(true),
      weapon(std::move(w)),
      lastHealTime(std::chrono::steady_clock::now()) {}

// Copy constructor
Character::Character(const Character& other)
    : GameEntity(other),
      windowWidth(other.windowWidth),
      windowHeight(other.windowHeight),
      screenPos(other.screenPos),
      width(other.width),
      height(other.height),
      health(other.health),
      alive(other.alive),
      weapon(other.weapon ? other.weapon->clone() : nullptr),
      inventory(other.inventory),
      lastHealTime(other.lastHealTime) {}

// Move constructor
Character::Character(Character&& other) noexcept
    : Character(0, 0, nullptr) {
    swap(other);
}

// Copy-and-swap assignment operator
Character& Character::operator=(Character other) {
    swap(other);
    return *this;
}

// Swap implementation
void Character::swap(Character& other) noexcept {
    using std::swap;
    swap(windowWidth, other.windowWidth);
    swap(windowHeight, other.windowHeight);
    swap(screenPos, other.screenPos);
    swap(width, other.width);
    swap(height, other.height);
    swap(health, other.health);
    swap(alive, other.alive);
    swap(weapon, other.weapon);
    swap(inventory, other.inventory);
    swap(lastHealTime, other.lastHealTime);
}

// Clone method
std::unique_ptr<GameEntity> Character::clone() const {
    return std::make_unique<Character>(*this);
}

// Virtual methods from  GameEntity
void Character::update(float deltaTime) {
    (void) deltaTime;
    autoHeal();
}

void Character::interact(GameEntity& other) {
    // Example interaction logic
    if (auto* item = dynamic_cast<Item*>(&other)) {
        addItemToInventory(*item);
    }
}

// Character-specific methods
void Character::autoHeal() {
    if (!alive) return;

    auto now = std::chrono::steady_clock::now();
    auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - lastHealTime).count();

    if (elapsedSeconds >= healIntervalSeconds) {
        heal(10.0f);
        lastHealTime = now;
    }
}

void Character::takeDamage(float damage) {
    health -= damage;
    if (health <= 0.f) {
        health = 0.f;
        alive = false;
    }
}

void Character::move(float dx, float dy) {
    std::get<0>(screenPos) += dx;
    std::get<1>(screenPos) += dy;
}

void Character::heal(float amount) {
    health += amount;
    if (health > 100.0f) {
        health = 100.0f;
    }
    std::cout << "Character healed by " << amount << ". Current health: " << health << "\n";
}

void Character::attack() const {
    if (weapon) {
        std::cout << "Character attacks with " << weapon->getName() << "!\n";
        weapon->printPosition();
    } else {
        std::cout << "Character has no weapon to attack with!\n";
    }
}

void Character::addItemToInventory(const Item& item) {
    if (inventory.size() >= getMaxInventorySize()) {
        throw InventoryFullException("Cannot add more items to inventory");
    }
    inventory.push_back(item);
    std::cout << item.getName() << " added to inventory.\n";
}

// Getters
float Character::getHealth() const {
    return health;
}

const std::tuple<float, float>& Character::getPosition() const {
    return screenPos;
}

// Static method
size_t Character::getMaxInventorySize() {
    return 10; // Example value
}

//std::ostream operator<<(const std::ostream & lhs, const std::unique_ptr<GameEntity>::element_type & rhs);

// Friend operator
std::ostream& operator<<(std::ostream& os, const Character& character) {
    os << "Character -> windowWidth: " << character.windowWidth << ", ";
    os << "windowHeight: " << character.windowHeight << ", ";
    os << "height: " << character.height << ", ";
    os << "width: " << character.width << ", ";
    os << "scale: " << character.scale << ", ";
    os << "health: " << character.health << ", ";
    os << "alive: " << (character.alive ? "Yes" : "No") << ", ";

    os << "screenPos: (" << std::get<0>(character.screenPos) << ", "
       << std::get<1>(character.screenPos) << "), ";

    if (character.weapon) {
        os << "weapon: " << *character.weapon << "\n";
    } else {
        os << "weapon: None\n";
    }

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