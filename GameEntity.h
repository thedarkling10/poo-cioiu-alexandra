#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>

class Character;

class GameEntity {
protected:
    std::vector<float> position;
    std::string name;
    float scale;
    float health;
    bool alive;

public:
    GameEntity(std::vector<float> pos, std::string n, float s = 1.0f, float h = 100.0f);
    virtual ~GameEntity() = default;

    [[nodiscard]] virtual std::unique_ptr<GameEntity> clone() const = 0;
    virtual void update(float deltaTime) = 0;
    virtual void interact(GameEntity& other);

protected:
    virtual void onInteractWith(GameEntity& other) = 0;
    virtual void onInteractWithCharacter(Character& character) = 0;

public:
    virtual void takeDamage(float damage);
    virtual void heal(float amount);
    void printPosition() const;
    [[nodiscard]] bool isAlive() const { return alive; }

    [[nodiscard]] const std::vector<float>& getPosition() const { return position; }
    [[nodiscard]] const std::string& getName() const { return name; }
    [[nodiscard]] float getScale() const { return scale; }
    [[nodiscard]] float getHealth() const { return health; }

    virtual void print(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const GameEntity& entity);
};

#endif // GAME_ENTITY_H