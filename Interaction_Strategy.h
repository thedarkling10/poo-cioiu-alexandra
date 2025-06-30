#ifndef INTERACTION_STRATEGY_H
#define INTERACTION_STRATEGY_H

#include <memory>
#include <iostream>
#include <string>

class Character;
class GameEntity;

class InteractionStrategy {
public:
    virtual ~InteractionStrategy() = default;
    virtual void execute(GameEntity& source, Character& target) = 0;
    virtual std::string getDescription() const = 0;
};

class HealingStrategy : public InteractionStrategy {
    float healAmount;
public:
    explicit HealingStrategy(float amount) : healAmount(amount) {}

    void execute(GameEntity& source, Character& target) override {
        target.heal(healAmount);
        std::cout << target.getName() << " healed for " << healAmount
                  << " HP using " << source.getName() << "\n";
    }

    std::string getDescription() const override {
        return "Heals for " + std::to_string(healAmount) + " HP";
    }
};

class WeaponStrategy : public InteractionStrategy {
    float damage;
public:
    explicit WeaponStrategy(float dmg) : damage(dmg) {}

    void execute(GameEntity& source, Character& target) override {
        std::cout << target.getName() << " equipped " << source.getName()
                  << " with " << damage << " damage\n";
    }

    std::string getDescription() const override {
        return "Weapon with " + std::to_string(damage) + " damage";
    }
};

class DialogueStrategy : public InteractionStrategy {
    std::string dialogue;
public:
    explicit DialogueStrategy(std::string msg) : dialogue(std::move(msg)) {}

    void execute(GameEntity& source, Character& target) override {
        std::cout << source.getName() << " says: \"" << dialogue << "\"\n";
    }

    std::string getDescription() const override {
        return "Says: \"" + dialogue + "\"";
    }
};

class StrategyGameEntity : public GameEntity {
protected:
    std::unique_ptr<InteractionStrategy> interactionStrategy;

public:
    StrategyGameEntity(std::vector<float> pos, std::string n,
                      std::unique_ptr<InteractionStrategy> strategy,
                      float s = 1.0f, float h = 100.0f)
        : GameEntity(std::move(pos), std::move(n), s, h),
          interactionStrategy(std::move(strategy)) {}

    void setInteractionStrategy(std::unique_ptr<InteractionStrategy> strategy) {
        interactionStrategy = std::move(strategy);
    }

    bool useOn(Character& character) override {
        if (interactionStrategy) {
            interactionStrategy->execute(*this, character);
            return dynamic_cast<HealingStrategy*>(interactionStrategy.get()) != nullptr;
        }
        return false;
    }

    std::string getInteractionDescription() const {
        return interactionStrategy ? interactionStrategy->getDescription() : "No interaction available";
    }
};

#endif // INTERACTION_STRATEGY_H