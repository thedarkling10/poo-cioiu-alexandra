//
// Created by cioiu on 6/28/2025.
//

#ifndef UTILES_H
#define UTILES_H
#include "GameEntity.h"
#include "Exceptions.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <type_traits>

template<typename TargetType, typename SourceType>
TargetType* safeCast(SourceType* source) {
    if (!source) {
        throw TypedGameException<std::string>("Null pointer in safeCast", "SourceType");
    }

    TargetType* result = dynamic_cast<TargetType*>(source);
    if (!result) {
        throw TypedGameException<std::string>("Invalid cast in safeCast", typeid(TargetType).name());
    }

    return result;
}

template<typename EntityType>
std::vector<EntityType*> findEntitiesByType(const std::vector<std::unique_ptr<GameEntity>>& entities) {
    std::vector<EntityType*> found;

    for (const auto& entity : entities) {
        if (entity) {
            EntityType* typed = dynamic_cast<EntityType*>(entity.get());
            if (typed) {
                found.push_back(typed);
            }
        }
    }

    return found;
}

template<typename EntityType, typename Operation>
void applyToEntitiesOfType(const std::vector<std::unique_ptr<GameEntity>>& entities,
                          Operation op) {
    try {
        for (const auto& entity : entities) {
            if (entity) {
                EntityType* typed = dynamic_cast<EntityType*>(entity.get());
                if (typed) {
                    op(*typed);
                }
            }
        }
    } catch (const std::exception& e) {
        throw TypedGameException<std::string>("Error in applyToEntitiesOfType", e.what());
    }
}

template<typename T>
void validateNumericRange(const T& value, const T& min, const T& max, const std::string& context) {
    static_assert(std::is_arithmetic_v<T>, "validateNumericRange requires arithmetic type");

    if (value < min || value > max) {
        throw InvalidValueException("Value out of range in " + context, static_cast<float>(value));
    }
}

template<typename T, typename... Args>
std::unique_ptr<T> createValidatedObject(Args&&... args) {
    try {
        auto obj = std::make_unique<T>(std::forward<Args>(args)...);

        if constexpr (requires { obj->validate(); }) {
            obj->validate();
        }

        return obj;
    } catch (const std::exception& e) {
        throw TypedGameException<std::string>("Failed to create validated object", e.what());
    }
}
#endif //UTILES_H
