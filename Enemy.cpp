#include "Enemy.h"
#include "Character.h"

Enemy::Enemy(std::vector<float> pos)
    : GameEntity(std::move(pos), "Enemy", 1.0f, 100.0f),
      speed(3.5f), damagePerSec(10.0f), radius(50.0f) {}

std::unique_ptr<GameEntity> Enemy::clone() const {
    return std::make_unique<Enemy>(*this);
}

void Enemy::update(float deltaTime) {
    position[0] += speed * deltaTime;
    printPosition();
    if (health <= 0) {
        alive = false;
    }
}

void Enemy::onInteractWith(GameEntity& other) {
    std::cout << name << " encounters " << other.getName() << "\n";
}

void Enemy::onInteractWithCharacter(Character& character) {
    attack(character);
}

void Enemy::attack(Character& target) const {
    float dx = target.getPosition()[0] - position[0];
    float dy = target.getPosition()[1] - position[1];
    float distanceSquared = dx * dx + dy * dy;

    if (alive && distanceSquared <= radius * radius) {
        target.takeDamage(damagePerSec);
        std::cout << name << " attacks " << target.getName() << " for " << damagePerSec << " damage!\n";
    }
}

void Enemy::print(std::ostream& os) const {
    GameEntity::print(os);
    os << " Speed: " << speed << " Damage: " << damagePerSec;
    os << " Scale: " << getScale() << " HP: " << getHealth();
}