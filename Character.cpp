#include "Character.h"
#include "Utiles.h"
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
        if (dynamic_cast<const Item*>(item.get())) {
            inventory.push_back(item->clone());
        }
    }
}

Character::Character(Character&& other) noexcept
    : Character(0, 0, nullptr) {
    swap(other);
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
    if (!alive) {
        throw DeadEntityException("Cannot update dead character");
    }
    (void)deltaTime;
    autoHeal();
}


void Character::onInteractWith(GameEntity& other) {
    std::cout << name << " interacts with " << other.getName() << "\n";
}

void Character::onInteractWithCharacter(Character& character) {
    std::cout << name << " interacts with " << character.getName() << "\n";
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

    if (position.size() < 2) {
        position.resize(2, 0.0f);
    }
    position[0] += dx;
    position[1] += dy;
}

void Character::addItem(std::unique_ptr<GameEntity> item) {
    if (!item) {
        throw TypedGameException<std::string>("Cannot add null item", "Character::addItem");
    }

    if (inventory.size() >= getMaxInventorySize()) {
        throw InventoryFullException("Cannot add item " + item->getName() +
                                   ", inventory is full (" +
                                   std::to_string(inventory.size()) + "/" +
                                   std::to_string(getMaxInventorySize()) + ")");
    }

    try {
        if (auto* const concreteItem = dynamic_cast<const Item*>(item.get())) {
            validateNumericRange(concreteItem->getDamage(), 0.0f, 1000.0f, "item damage");
            validateNumericRange(concreteItem->getHealingAmount(), 0.0f, 500.0f, "item healing");

            inventory.push_back(std::move(item));
            std::cout << "Added item to inventory: " << concreteItem->getName() << "\n";
        } else {
            throw TypedGameException<std::string>("Invalid item type for character inventory",
                                                item->getName());
        }
    } catch (const InvalidValueException& e) {
        std::cout << "Item validation failed: " << e.getDetailedMessage() << "\n";
        throw;
    }
}

void Character::useItemFromInventory(size_t index) {
    try {
        validateNumericRange(index, size_t{0}, inventory.size() - 1, "inventory index");

        auto& item = inventory[index];
        if (!item) {
            throw TypedGameException<size_t>("Null item at inventory index", index);
        }

        std::string itemName = item->getName();
        bool isConsumable = false;

        if (const auto* concreteItem = dynamic_cast<const Item*>(item.get())) {
            const_cast<Item*>(concreteItem)->interact(*this);
            isConsumable = (concreteItem->getHealingAmount() > 0.0f);
        } else {
            item->interact(*this);
        }

        if (isConsumable) {
            inventory.erase(inventory.begin() + index);
            std::cout << "Consumed " << itemName << "\n";  // Use stored name
        }
    }
    catch (const TypedGameException<size_t>& e) {
        std::cout << "Inventory error: " << e.getDetailedMessage() << "\n";
        throw;
    }
    catch (const std::exception& e) {
        std::cout << "Error using item: " << e.what() << "\n";
        throw;
    }
}

void Character::attack() const {
    if (weapon) {
        if (const auto* weaponItem = dynamic_cast<const Item*>(weapon.get())) {
            const_cast<Item*>(weaponItem)->interact(const_cast<Character&>(*this));

            if (weaponItem->isEquipped()) {
                std::cout << "Using equipped weapon\n";
            }
        }
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
    os << " Inventory size: " << inventory.size() << "/" << getMaxInventorySize();
}