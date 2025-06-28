#include "Item.h"
#include "Character.h"
#include <iostream>

Item::Item(std::string n, std::vector<float> pos, float dmg, float heal)
    : GameEntity(std::move(pos), std::move(n), 1.0f),
      damage(dmg), healAmount(heal), equipped(false) {}

std::unique_ptr<GameEntity> Item::clone() const {
    return std::make_unique<Item>(*this);
}

void Item::update(float deltaTime) {
    (void)deltaTime;
}

void Item::onInteractWith(GameEntity& other) {
    std::cout << other.getName() << " interacts with " << name << "\n";
}

void Item::onInteractWithCharacter(Character& character) {
    if (healAmount > 0) {
        character.heal(healAmount);
        std::cout << character.getName() << " healed for " << healAmount << " HP using " << name << "\n";
    }
    if (damage > 0) {
        std::cout << character.getName() << " equipped " << name << " with " << damage << " damage\n";
    }
}

void Item::equipItem() {
    equipped = true;
}

void Item::unequipItem() {
    equipped = false;
}

void Item::upgradeItem(float percent) {
    damage *= (1.0f + percent / 100.0f);
    healAmount *= (1.0f + percent / 100.0f);
}

void Item::specialAttack() const {
    std::cout << "Using special attack with " << name << " dealing " << damage * 1.5f << " damage!\n";
}

void Item::print(std::ostream& os) const {
    GameEntity::print(os);
    os << " Damage: " << damage << " Heal: " << healAmount
       << " Equipped: " << (equipped ? "Yes" : "No");
}