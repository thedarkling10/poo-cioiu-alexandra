#include "GameEntity.h"

GameEntity::GameEntity(std::vector<float> pos, std::string n, float s, float h)
    : position(std::move(pos)), name(std::move(n)), scale(s), health(h), alive(true) {}

void GameEntity::takeDamage(float damage) {
    health -= damage;
    if (health <= 0.f) {
        health = 0.f;
        alive = false;
    }
}

void GameEntity::heal(float amount) {
    health = std::min(health + amount, 100.0f);
}

void GameEntity::printPosition() const {
    std::cout << name << " at ";
    for (auto coord : position) {
        std::cout << coord << " ";
    }
    std::cout << "\n";
}

void GameEntity::print(std::ostream& os) const {
    os << name << " [";
    for (size_t i = 0; i < position.size(); ++i) {
        os << position[i];
        if (i < position.size() - 1) os << ", ";
    }
    os << "] Scale: " << scale << " Health: " << health;
}

std::ostream& operator<<(std::ostream& os, const GameEntity& entity) {
    entity.print(os);
    return os;
}