#ifndef PROP_H
#define PROP_H

#include "GameEntity.h"

class Prop : public GameEntity {
public:
    Prop(std::vector<float> pos, std::string n, float s = 4.0f);

    Prop(const Prop& other);
    Prop& operator=(const Prop& other);
    Prop(Prop&& other) noexcept;
    Prop& operator=(Prop&& other) noexcept;
    ~Prop() override = default;

    std::unique_ptr<GameEntity> clone() const override;
    void update(float deltaTime) override;
    void onInteractWith(GameEntity& other) override;
    void onInteractWithCharacter(Character& character) override;

    friend std::ostream& operator<<(std::ostream& os, const Prop& prop);
};

#endif // PROP_H