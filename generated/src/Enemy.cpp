#include "../include/Enemy.h"

Enemy::Enemy(std::vector<float> pos)
    : GameEntity(std::move(pos), "Enemy", 1.0f),
      speed(3.5f), health(100.0f), alive(true),
      damagePerSec(10.0f), radius(50.0f) {}

std::unique_ptr<GameEntity> Enemy::clone() const {
    return std::make_unique<Enemy>(*this);
}

void Enemy::update(float deltaTime) {
    // Enemy AI logic
}

void Enemy::interact(GameEntity& other) {
    if (auto* character = dynamic_cast<Character*>(&other)) {
        attack(*character);
    }
}

void Enemy::attack(Character& target) const {
    if (alive) {
        std::cout << "Enemy attacks! ";
        target.takeDamage(damagePerSec);
    }
}