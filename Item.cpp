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

void Item::interact(GameEntity& other) {
    if (auto* character = dynamic_cast<Character*>(&other)) {
        character->heal(healAmount);
    }
}

void Item::print(std::ostream& os) const {
    GameEntity::print(os);
    os << " Damage: " << damage << " Heal: " << healAmount
       << " Equipped: " << (equipped ? "Yes" : "No");
}

