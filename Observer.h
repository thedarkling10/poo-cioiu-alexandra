#ifndef GAME_EVENT_OBSERVER_H
#define GAME_EVENT_OBSERVER_H

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <iostream>

enum class GameEventType {
    PLAYER_DAMAGED,
    PLAYER_HEALED,
    ITEM_USED,
    ENEMY_DEFEATED,
    LEVEL_UP
};

struct GameEvent {
    GameEventType type;
    std::string description;
    float value;

    GameEvent(GameEventType t, std::string desc, float v = 0.0f)
        : type(t), description(std::move(desc)), value(v) {}
};

class GameEventObserver {
public:
    virtual ~GameEventObserver() = default;
    virtual void onGameEvent(const GameEvent& event) = 0;
};

class GameEventSubject {
    std::vector<std::weak_ptr<GameEventObserver>> observers;

public:
    void addObserver(std::weak_ptr<GameEventObserver> observer) {
        observers.push_back(observer);
    }

    void removeObserver(std::weak_ptr<GameEventObserver> observer) {
        auto it = std::find_if(observers.begin(), observers.end(),
            [&observer](const std::weak_ptr<GameEventObserver>& obs) {
                return !obs.expired() && obs.lock() == observer.lock();
            });
        if (it != observers.end()) {
            observers.erase(it);
        }
    }

protected:
    void notifyObservers(const GameEvent& event) {
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [&event](const std::weak_ptr<GameEventObserver>& obs) {
                    if (auto observer = obs.lock()) {
                        observer->onGameEvent(event);
                        return false;
                    }
                    return true;
                }),
            observers.end());
    }
};

class HealthLogger : public GameEventObserver {
public:
    void onGameEvent(const GameEvent& event) override {
        if (event.type == GameEventType::PLAYER_DAMAGED) {
            std::cout << "[HEALTH] Player took " << event.value << " damage: " << event.description << "\n";
        } else if (event.type == GameEventType::PLAYER_HEALED) {
            std::cout << "[HEALTH] Player healed " << event.value << " HP: " << event.description << "\n";
        }
    }
};

class ScoreTracker : public GameEventObserver {
    int score = 0;

public:
    void onGameEvent(const GameEvent& event) override {
        switch (event.type) {
            case GameEventType::ENEMY_DEFEATED:
                score += 100;
                std::cout << "[SCORE] Enemy defeated! Score: " << score << "\n";
                break;
            case GameEventType::ITEM_USED:
                score += 10;
                std::cout << "[SCORE] Item used! Score: " << score << "\n";
                break;
            default:
                break;
        }
    }

    int getScore() const { return score; }
};

class AchievementTracker : public GameEventObserver {
    int itemsUsed = 0;
    int enemiesDefeated = 0;

public:
    void onGameEvent(const GameEvent& event) override {
        switch (event.type) {
            case GameEventType::ITEM_USED:
                itemsUsed++;
                if (itemsUsed == 10) {
                    std::cout << "[ACHIEVEMENT] Potion Master - Used 10 items!\n";
                }
                break;
            case GameEventType::ENEMY_DEFEATED:
                enemiesDefeated++;
                if (enemiesDefeated == 5) {
                    std::cout << "[ACHIEVEMENT] Warrior - Defeated 5 enemies!\n";
                }
                break;
            default:
                break;
        }
    }
};

class ObserverCharacter : public Character, public GameEventSubject {
public:
    ObserverCharacter(int winWidth, int winHeight, std::unique_ptr<GameEntity> w)
        : Character(winWidth, winHeight, std::move(w)) {}

    void takeDamage(float damage) override {
        Character::takeDamage(damage);
        notifyObservers(GameEvent(GameEventType::PLAYER_DAMAGED,
                                "Player took damage", damage));
    }

    void heal(float amount) override {
        Character::heal(amount);
        notifyObservers(GameEvent(GameEventType::PLAYER_HEALED,
                                "Player healed", amount));
    }

    void useItemFromInventory(size_t index) override {
        std::string itemName;
        try {
            itemName = inventory.at(index)->getName();
            Character::useItemFromInventory(index);
            notifyObservers(GameEvent(GameEventType::ITEM_USED,
                                    "Used " + itemName));
        } catch (const std::exception& e) {
            throw;
        }
    }
};

#endif // GAME_EVENT_OBSERVER_H