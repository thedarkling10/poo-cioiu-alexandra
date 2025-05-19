#include "../include/Prop.h"

Prop::Prop(std::vector<float> pos, std::string n, float s)
    : GameEntity(std::move(pos), std::move(n), s) {}

Prop::Prop(const Prop& other)
    : GameEntity(other) {}

Prop& Prop::operator=(const Prop& other) {
    GameEntity::operator=(other);
    return *this;
}

Prop::Prop(Prop&& other) noexcept
    : GameEntity(std::move(other)) {}

Prop& Prop::operator=(Prop&& other) noexcept {
    GameEntity::operator=(std::move(other));
    return *this;
}

std::unique_ptr<GameEntity> Prop::clone() const {
    return std::make_unique<Prop>(*this);
}

void Prop::update(float deltaTime) {
    // Props typically don't update
}

void Prop::interact(GameEntity& other) {
    std::cout << name << " was interacted with by " << other.getName() << "\n";
}

std::ostream& operator<<(std::ostream& os, const Prop& prop) {
    os << prop.name << ", worldPos: [";
    for (size_t i = 0; i < prop.position.size(); i++) {
        os << prop.position[i];
        if (i < prop.position.size() - 1) os << ", ";
    }
    os << "], scale: " << prop.scale;
    return os;
}