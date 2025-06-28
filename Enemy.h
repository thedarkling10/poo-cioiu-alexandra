#ifndef ENEMY_H
#define ENEMY_H

#include "GameEntity.h"

class Enemy : public GameEntity {
private:
    float speed;
    float damagePerSec;
    float radius;

protected:
    void onInteractWith(GameEntity& other) override;
    void onInteractWithCharacter(Character& character) override;

public:
    explicit Enemy(std::vector<float> pos);

    [[nodiscard]] std::unique_ptr<GameEntity> clone() const override;
    void update(float deltaTime) override;
    void attack(Character& target) const;

protected:
    void print(std::ostream& os) const override;
};

#endif // ENEMY_H