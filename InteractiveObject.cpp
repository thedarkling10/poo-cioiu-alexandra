#include "InteractiveObject.h"

InteractiveObject::InteractiveObject(std::vector<float> pos, std::string n, float s)
    : GameEntity(std::move(pos), std::move(n), s), interactable(true) {}

void InteractiveObject::update(float deltaTime) {
    (void)deltaTime;
}

void InteractiveObject::interact(GameEntity& other) {
    if (interactable) {
        onUse(other);
        setInteractable(false);
    }
}
