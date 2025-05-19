#include "../include/Item.h"
#include <iostream>
#include <string>
#include "../../libs/termcolor/termcolor.hpp"

Item::Item(std::string n, std::vector<float> pos, float dmg, float heal)
    : GameEntity(std::move(pos), std::move(n), 1.0f),
      damage(dmg), healAmount(heal), equipped(false) {

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

std::unique_ptr<GameEntity> Item::clone() const {
    return std::make_unique<Item>(*this);
}

void Item::update(float deltaTime) {
    // Items could degrade over time
    degradeItem(0.1f);
}

void Item::interact(GameEntity& other) {
    std::cout << name << " used on " << other.getName() << "\n";
}

bool Item::isEquipped() const {
    return equipped;
}

void Item::equipItem() {
    if (!equipped) {
        equipped = true;
        std::cout << name << " is now equipped!\n";
    } else {
        std::cout << name << " is already equipped!\n";
    }
}

void Item::unequipItem() {
    if (equipped) {
        equipped = false;
        std::cout << name << " is now unequipped!\n";
    } else {
        std::cout << name << " is not equipped!\n";
    }
}

void Item::dropItem(float x, float y) {
    equipped = false;
    position = {x, y};
    std::cout << name << " has been dropped at [" << x << ", " << y << "]\n";
}

void Item::upgradeItem(float percent) {
    if (percent <= 0) {
        std::cout << "Cannot upgrade item!\n";
        return;
    }

    damage += damage * (percent / 100.0f);
    healAmount += healAmount * (percent / 100.0f);

    std::cout << name << " gained an improvement!"
              << termcolor::green << " View stats? (yes/no)" << termcolor::reset;

    std::string response;
    std::cin >> response;
    if (response == "yes") {
        std::cout << "New stats:\n"
                  << termcolor::red << " Damage = " << termcolor::reset << damage
                  << termcolor::green << ", Heal = " << termcolor::reset << healAmount << "\n";
    }
}

void Item::degradeItem(float percent) {
    if (percent <= 0) {
        std::cout << "Error degrading item!\n";
        return;
    }

    damage -= damage * (percent / 100.0f);
    healAmount -= healAmount * (percent / 100.0f);

    std::cout << name << " is wearing out! New stats:\n "
              << termcolor::red << "Damage = " << termcolor::reset << damage
              << termcolor::green << ", Heal = " << termcolor::reset << healAmount << "\n";
}

void Item::specialAttack() const {
    if (name == "Excalibur") {
        float specialDamage = damage * 2.0f;
        std::cout << "Special attack with " << name
                  << " applied! Damage: " << specialDamage << "\n";
    }
    else if (name == "Masamune") {
        float specialDamage = damage * 1.5f;
        std::cout << "Special attack with " << name
                  << " applied! Damage: " << specialDamage << "\n";
    }
    else if (name == "Wallace") {
        float specialDamage = damage * 1.3f;
        std::cout << "Special attack with " << name
                  << " applied! Damage: " << specialDamage << "\n";
    }
    else {
        std::cout << name << " does not have any special attacks!\n";
    }
}

float Item::getHealingAmount() const {
    return healAmount;
}

// Overloaded output operator
std::ostream& operator<<(std::ostream& os, const Item& item) {
    os << "Item -> Name: " << item.name << ", Position: [";
    for (size_t i = 0; i < item.position.size(); i++) {
        os << item.position[i];
        if (i < item.position.size() - 1) os << ", ";
    }
    os << "], Damage: " << item.damage
       << ", HealAmount: " << item.healAmount
       << ", Equipped: " << (item.equipped ? "Yes" : "No");
    return os;
}