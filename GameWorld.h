#ifndef GAME_WORLD_H
#define GAME_WORLD_H


#include "GameEntity.h"
#include "Character.h"
#include <vector>
#include <memory>
#include <random>

class GameWorld {
    std::unique_ptr<Character> player;
    std::vector<std::unique_ptr<GameEntity>> entities; // Uniform access to all objects
    std::vector<std::unique_ptr<GameEntity>> items;    // Items on the ground
    std::mt19937 rng;
    std::uniform_int_distribution<> chanceDist;
    int turnCount;
    bool gameRunning;
    
    static constexpr float INTERACTION_DISTANCE = 50.0f;
    
    bool areClose(const GameEntity& a, const GameEntity& b) const;
    void spawnRandomItem();
    void updateEntities(float deltaTime);
    void handlePlayerAction(int action);
    void handleMove();
    void handleAttack();
    void handleUseItem();
    void handleTalkToNPC();
    void processNearbyEntities();
    void displayGameState() const;
    void displayMenu() const;

public:
    GameWorld();
    void initialize();
    void run();
    bool isGameRunning() const { return gameRunning && player && player->isAlive(); }
};

#endif // GAME_WORLD_H