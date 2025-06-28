#include "NPC.h"
#include "Character.h"
#include <iostream>

NPC::NPC(std::vector<float> pos, std::string n, std::string dialog)
    : GameEntity(std::move(pos), std::move(n), 1.0f), dialogue(std::move(dialog)) {}

std::unique_ptr<GameEntity> NPC::clone() const {
    return std::make_unique<NPC>(*this);
}

void NPC::update(float deltaTime) {
    (void)deltaTime;
}

void NPC::onInteractWith(GameEntity& other) {
    std::cout << name << " greets " << other.getName() << "\n";
}

void NPC::onInteractWithCharacter(Character& character) {
    (void)character;
    if (dialogue.empty()) {
        std::cout << "The NPC seems silent...\n";
        return;
    }
    speak();
}

void NPC::speak() const {
    std::cout << name << " says: \"" << dialogue << "\"\n";
}

void NPC::print(std::ostream& os) const {
    GameEntity::print(os);
    os << " Dialogue: \"" << dialogue << "\"";
}