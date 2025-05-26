#include "Enemy.h"

Enemy::Enemy(std::vector<float> pos)
    : GameEntity(std::move(pos), "Enemy", 1.0f, 100.0f),
      speed(3.5f), damagePerSec(10.0f), radius(50.0f) {}

std::unique_ptr<GameEntity> Enemy::clone() const {
    return std::make_unique<Enemy>(*this);
}

void Enemy::update(float deltaTime) {
    position[0] += speed * deltaTime;
    if (health <= 0) {
        alive = false;
    }
}

void Enemy::interact(GameEntity& other) {
    if (auto* character = dynamic_cast<Character*>(&other)) {
        attack(*character);
    }
}

void Enemy::attack(Character& target) const {
    if (alive) {
        target.takeDamage(damagePerSec);
    }
}

void Enemy::print(std::ostream& os) const {
    GameEntity::print(os);
    os << " Speed: " << speed << " Damage: " << damagePerSec;
}