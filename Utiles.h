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


template<typename T>
void validateNumericRange(const T& value, const T& min, const T& max, const std::string& context) {
    static_assert(std::is_arithmetic_v<T>, "validateNumericRange requires arithmetic type");

    if (value < min || value > max) {
        throw InvalidValueException("Value out of range in " + context, static_cast<float>(value));
    }
}


#endif //UTILES_H
