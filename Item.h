#ifndef ITEM_H
#define ITEM_H

#include "GameEntity.h"
#include <string>

class Item : public GameEntity {
    float damage;
    float healAmount;
    bool equipped;

public:
    Item(std::string n, std::vector<float> pos, float dmg, float heal);

    [[nodiscard]] std::unique_ptr<GameEntity> clone() const override;
    void update(float deltaTime) override;
    void interact(GameEntity& other) override;

    void equipItem();
    void unequipItem();
    void upgradeItem(float percent);
    void specialAttack() const;
    
    [[nodiscard]] float getHealingAmount() const { return healAmount; }

protected:
    void print(std::ostream& os) const override;
};

#endif // ITEM_H