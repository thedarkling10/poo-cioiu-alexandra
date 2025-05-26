#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "GameEntity.h"
#include "Character.h"
#include <vector>
#include <string>

class Obstacle : public GameEntity {
    float width;
    float height;

public:
    Obstacle(std::vector<float> pos, std::string n);
    
    [[nodiscard]] std::unique_ptr<GameEntity> clone() const override;
    void update(float deltaTime) override;
    void interact(GameEntity& other) override;

    [[nodiscard]] bool reach(float x, float y) const;
    void checkDangerZone(Character& character) const;
};

#endif // OBSTACLE_H