#ifndef ITEM_H
#define ITEM_H

#include "GameEntity.h"
#include <string>

class Item : public GameEntity {
private:
    float damage;
    float healAmount;
    bool equipped;

public:
    Item(std::string n, std::vector<float> pos, float dmg, float heal);

    // Virtual methods
    std::unique_ptr<GameEntity> clone() const override;
    void update(float deltaTime) override;
    void interact(GameEntity& other) override;

    // Item-specific methods
    bool isEquipped() const;
    void equipItem();
    void unequipItem();
    void dropItem(float x, float y);
    void upgradeItem(float percent);
    void degradeItem(float percent);
    void specialAttack() const;

    float getHealingAmount() const;

    friend std::ostream& operator<<(std::ostream& os, const Item& item);

};

#endif // ITEM_H//