#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>

class GameEntity {
protected:
    std::vector<float> position;
    std::string name;
    float scale;
    float health;
    bool alive;

public:
    GameEntity(std::vector<float> pos, std::string n, float s = 1.0f, float h = 100.0f);
    virtual ~GameEntity() = default; // fct virtuala pura

    // Pure virtual functions (clone is virtual constructor)
    virtual std::unique_ptr<GameEntity> clone() const = 0;  // Virtual constructor
    virtual void update(float deltaTime) = 0;               // Game-specific virtual function
    virtual void interact(GameEntity& other) = 0;           // Game-specific virtual function

    // Functionalitati
    virtual void takeDamage(float damage);
    virtual void heal(float amount);
    void printPosition() const;
    bool isAlive() const { return alive; }

    // Const getters
    const std::vector<float>& getPosition() const { return position; }
    const std::string& getName() const { return name; }
    float getScale() const { return scale; }
    float getHealth() const { return health; }

    // op virtual
    virtual void print(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const GameEntity& entity);
};

#endif // GAME_ENTITY_H