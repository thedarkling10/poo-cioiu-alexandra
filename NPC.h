#ifndef NPC_H
#define NPC_H

#include "GameEntity.h"
#include <string>

class NPC : public GameEntity {
    std::string dialogue;

public:
    NPC(std::vector<float> pos, std::string n, std::string dialog);

    [[nodiscard]] std::unique_ptr<GameEntity> clone() const override;
    void update(float deltaTime) override;
    void interact(GameEntity& other) override;

    void speak() const;

protected:
    void print(std::ostream& os) const override;
};

#endif // NPC_H