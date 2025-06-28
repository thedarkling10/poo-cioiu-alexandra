#ifndef INTERACTIVE_OBJECT_H
#define INTERACTIVE_OBJECT_H

#include "GameEntity.h"

class InteractiveObject : public GameEntity {
protected:
    bool interactable;

public:
    InteractiveObject(std::vector<float> pos, std::string n, float s = 1.0f);

    virtual void onUse(GameEntity& user) = 0;
    virtual void onUseByCharacter(Character& character) = 0;

    [[nodiscard]] std::unique_ptr<GameEntity> clone() const override = 0;
    void update(float deltaTime) override;
    void onInteractWith(GameEntity& other) override;
    void onInteractWithCharacter(Character& character) override;

    void setInteractable(bool canInteract) { interactable = canInteract; }
};

#endif // INTERACTIVE_OBJECT_H