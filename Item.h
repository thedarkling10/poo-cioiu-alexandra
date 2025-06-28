#ifndef ITEM_H
#define ITEM_H

#include "GameEntity.h"
#include <string>

class Item : public GameEntity {
    float damage;
    float healAmount;
    bool equipped;

protected:
    void onInteractWith(GameEntity& other) override;
    void onInteractWithCharacter(Character& character) override;

public:
    Item(std::string n, std::vector<float> pos, float dmg, float heal);

    [[nodiscard]] std::unique_ptr<GameEntity> clone() const override;
    void update(float deltaTime) override;
    void upgradeItem(float percent);
    void specialAttack() const;

    [[nodiscard]] float getHealingAmount() const { return healAmount; }
    [[nodiscard]] float getDamage() const { return damage; }
    [[nodiscard]] bool isEquipped() const { return equipped; }

protected:
    void print(std::ostream& os) const override;
};

#endif // ITEM_H