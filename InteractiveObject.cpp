#include "InteractiveObject.h"
#include "Character.h"

InteractiveObject::InteractiveObject(std::vector<float> pos, std::string n, float s)
    : GameEntity(std::move(pos), std::move(n), s), interactable(true) {}

void InteractiveObject::update(float deltaTime) {
    (void)deltaTime;
}

void InteractiveObject::onInteractWith(GameEntity& other) {
    if (interactable) {
        onUse(other);
        setInteractable(false);
    }
}

void InteractiveObject::onInteractWithCharacter(Character& character) {
    if (interactable) {
        onUseByCharacter(character);
        setInteractable(false);
    }
}