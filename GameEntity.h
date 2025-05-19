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

public:
    GameEntity(std::vector<float> pos, std::string n, float s = 1.0f);
    virtual ~GameEntity() = default;

    // Virtual methods
    virtual std::unique_ptr<GameEntity> clone() const = 0;
    virtual void update(float deltaTime) = 0;
    virtual void interact(GameEntity& other) = 0;

    // Common functionality
    virtual void printPosition() const;

    // Getters
    const std::vector<float>& getPosition() const { return position; }
    const std::string& getName() const { return name; }
    float getScale() const { return scale; }

    friend std::ostream& operator<<(std::ostream& os, const GameEntity& entity);
};

#endif // GAME_ENTITY_H