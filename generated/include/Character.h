#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameEntity.h"
#include "Item.h"
#include <vector>
#include <tuple>
#include <memory>
#include <chrono>

class Character : public GameEntity {
private:
    int windowWidth;
    int windowHeight;
    std::tuple<float, float> screenPos;
    float width;
    float height;
    float health;
    bool alive;
    std::unique_ptr<GameEntity> weapon; // Polymorphic weapon
    std::vector<Item> inventory;
    std::chrono::steady_clock::time_point lastHealTime;
    static const int healIntervalSeconds = 5;

    // Private helper methods
    void swap(Character& other) noexcept;

public:
    // Constructor
    Character(int winWidth, int winHeight, std::unique_ptr<GameEntity> w);

    // Rule of Five
    Character(const Character& other);
    Character(Character&& other) noexcept;
    Character& operator=(Character other);
    ~Character() override = default;

    // Clone method
    std::unique_ptr<GameEntity> clone() const override;

    // Virtual methods from GameEntity
    void update(float deltaTime) override;
    void interact(GameEntity& other) override;

    // Character-specific methods
    void autoHeal();
    void takeDamage(float damage);
    void move(float dx, float dy);
    void heal(float amount);
    void attack() const;
    void addItemToInventory(const Item& item);

    // Getters
    float getHealth() const;
    const std::tuple<float, float>& getPosition() const;
    
    // Static method
    static size_t getMaxInventorySize();

    // Friend operator
    friend std::ostream& operator<<(std::ostream& os, const Character& character);
};

#endif // CHARACTER_H