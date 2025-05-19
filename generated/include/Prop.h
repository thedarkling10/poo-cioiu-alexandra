#ifndef PROP_H
#define PROP_H

#include "GameEntity.h"
#include <iostream>
#include <utility>

class Prop : public GameEntity {
public:
    Prop(std::vector<float> pos, std::string n, float s = 4.0f);

    // Rule of Five
    Prop(const Prop& other);
    Prop& operator=(const Prop& other);
    Prop(Prop&& other) noexcept;
    Prop& operator=(Prop&& other) noexcept;
    ~Prop() override = default;

    // Virtual methods
    std::unique_ptr<GameEntity> clone() const override;
    void update(float deltaTime) override;
    void interact(GameEntity& other) override;

    friend std::ostream& operator<<(std::ostream& os, const Prop& prop);
};

#endif // PROP_H