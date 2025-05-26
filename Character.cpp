#include "Character.h"
#include "Item.h"
#include "Exceptions.h"
#include <algorithm>

const int Character::healIntervalSeconds = 5;

Character::Character(int winWidth, int winHeight, std::unique_ptr<GameEntity> w)
    : GameEntity({}, "Character", 4.0f),
      screenPos(winWidth/2.0f, winHeight/2.0f),
      weapon(std::move(w)),
      lastHealTime(std::chrono::steady_clock::now()) {}

Character::Character(const Character& other)
    : GameEntity(other),
      screenPos(other.screenPos),
      weapon(other.weapon ? other.weapon->clone() : nullptr),
      lastHealTime(other.lastHealTime) {
    inventory.reserve(other.inventory.size());
    for (const auto& item : other.inventory) {
        inventory.push_back(item->clone());
    }
}

Character::Character(Character&& other) noexcept
    : Character(0, 0, nullptr) {
    swap(other);
}

Character& Character::operator=(Character other) {
    swap(other);
    return *this;
}

void Character::swap(Character& other) noexcept {
    using std::swap;
    swap(position, other.position);
    swap(name, other.name);
    swap(scale, other.scale);
    swap(health, other.health);
    swap(alive, other.alive);
    swap(screenPos, other.screenPos);
    swap(weapon, other.weapon);
    swap(inventory, other.inventory);
    swap(lastHealTime, other.lastHealTime);
}

std::unique_ptr<GameEntity> Character::clone() const {
    return std::make_unique<Character>(*this);
}

void Character::update(float deltaTime) {
    (void)deltaTime;
    autoHeal();
}

void Character::removeOldestItem() {
    if (!inventory.empty()) {
        inventory.erase(inventory.begin());
    }
}

void Character::interact(GameEntity& other) {
    if (auto* item = dynamic_cast<Item*>(&other)) {
        if (inventory.size() >= getMaxInventorySize()) {
            removeOldestItem();
        }
        inventory.push_back(std::unique_ptr<GameEntity>(item->clone()));
    }
}

void Character::autoHeal() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastHealTime).count();

    if (elapsed >= healIntervalSeconds) {
        heal(10.0f);
        lastHealTime = now;
    }
}

void Character::move(float dx, float dy) {
    std::get<0>(screenPos) += dx;
    std::get<1>(screenPos) += dy;
}

void Character::addItem(std::unique_ptr<GameEntity> item) {
    if (inventory.size() >= getMaxInventorySize()) {
        removeOldestItem();
    }
    inventory.push_back(std::move(item));
}

void Character::attack() const {
    if (weapon) {
        const_cast<GameEntity*>(weapon.get())->interact(const_cast<Character&>(*this));
    }
}

size_t Character::getMaxInventorySize() {
    return 10;
}

void Character::print(std::ostream& os) const {
    GameEntity::print(os);
    auto [x, y] = getScreenPosition();
    os << " ScreenPos: (" << x << ", " << y << ")";
    if (weapon) {
        os << " Weapon: " << *weapon;
    }
}
