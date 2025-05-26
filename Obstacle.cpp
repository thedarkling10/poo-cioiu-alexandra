#include "Obstacle.h"

Obstacle::Obstacle(std::vector<float> pos, std::string n)
    : GameEntity(std::move(pos), std::move(n), 1.0f),
      width(2.0f), height(2.0f) {}

std::unique_ptr<GameEntity> Obstacle::clone() const {
    return std::make_unique<Obstacle>(*this);
}

void Obstacle::update(float deltaTime) {
    (void)deltaTime;
}

void Obstacle::interact(GameEntity& other) {
    if (auto* character = dynamic_cast<Character*>(&other)) {
        checkDangerZone(*character);
    }
}

bool Obstacle::reach(float x, float y) const {
    return x >= position[0] && x <= position[0] + width &&
           y >= position[1] && y <= position[1] + height;
}

void Obstacle::checkDangerZone(Character& character) const {
    auto pos = character.getPosition();
    if (pos.size() >= 2 && reach(pos[0], pos[1])) {
        std::cout << "Character in danger zone of " << name << "!\n";
        character.takeDamage(10.0f);
    }
}