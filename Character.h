#ifndef CHARACTER_H
#define CHARACTER_H

#include "GameEntity.h"
#include <tuple>
#include <chrono>
#include <vector>

class Character : public GameEntity {
private:
    std::tuple<float, float> screenPos;
    std::unique_ptr<GameEntity> weapon;
    std::vector<std::unique_ptr<GameEntity>> inventory;
    std::chrono::steady_clock::time_point lastHealTime;
    static const int healIntervalSeconds;

    void swap(Character& other) noexcept;

public:
    Character(int winWidth, int winHeight, std::unique_ptr<GameEntity> w);

    Character(const Character& other);
    Character(Character&& other) noexcept;
    Character& operator=(Character other);
    ~Character() override = default;

    [[nodiscard]] std::unique_ptr<GameEntity> clone() const override;
    void update(float deltaTime) override;
    void interact(GameEntity& other) override;

    void removeOldestItem();
    void addItem(std::unique_ptr<GameEntity> item);

    void autoHeal();
    void move(float dx, float dy);
    void attack() const;

    [[nodiscard]] const std::tuple<float, float>& getScreenPosition() const { return screenPos; }
    static size_t getMaxInventorySize();

protected:
    void print(std::ostream& os) const override;
};

#endif // CHARACTER_H
