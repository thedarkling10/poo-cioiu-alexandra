#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "GameEntity.h"
#include "Character.h"
#include <vector>
#include <string>

class Obstacle : public GameEntity {
private:
    float width;
    float height;

public:
    Obstacle(std::vector<float> pos, std::string n);
    
    // Virtual methods
    std::unique_ptr<GameEntity> clone() const override;
    void update(float deltaTime) override;
    void interact(GameEntity& other) override;
    
    // Obstacle-specific methods
    bool reach(float x, float y) const;
    void checkDangerZone(Character& character) const;
};

#endif // OBSTACLE_H