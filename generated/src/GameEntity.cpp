#include "../include/GameEntity.h"

GameEntity::GameEntity(std::vector<float> pos, std::string n, float s)
    : position(std::move(pos)), name(std::move(n)), scale(s) {}

void GameEntity::printPosition() const {
    if (position.size() >= 2) {
        std::cout << name << " at " << position[0] << "," << position[1] << "\n";
    }
}