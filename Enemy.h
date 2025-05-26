#ifndef ENEMY_H
#define ENEMY_H

#include "GameEntity.h"
#include "Character.h"

class Enemy : public GameEntity {
private:
    float speed;
    float damagePerSec;
    float radius;

public:
    Enemy(std::vector<float> pos);

    std::unique_ptr<GameEntity> clone() const override;
    void update(float deltaTime) override;
    void interact(GameEntity& other) override;
    void attack(Character& target) const;

protected:
    void print(std::ostream& os) const override;
};

#endif // ENEMY_H