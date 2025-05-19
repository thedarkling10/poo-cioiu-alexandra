#ifndef ENEMY_H
#define ENEMY_H

#include "GameEntity.h"
#include "Character.h"
#include <vector>

class Enemy : public GameEntity {
private:
    float speed;
    float health;
    bool alive;
    float damagePerSec;
    float radius;

public:
    explicit Enemy(std::vector<float> pos);
    
    // Virtual methods
    std::unique_ptr<GameEntity> clone() const override;

    bool checkPlayerInRadius() const;

    void update(float deltaTime) override;
    void interact(GameEntity& other) override;
    
    // Enemy-specific methods
    void attack(Character& target) const;
};

#endif // ENEMY_H