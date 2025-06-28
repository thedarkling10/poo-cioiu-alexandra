#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>
#include <sstream>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& msg) : std::runtime_error(msg) {}
};

class InvalidPositionException : public GameException {
public:
    explicit InvalidPositionException(const std::string& msg)
        : GameException("Invalid position: " + msg) {}
};

class InventoryFullException : public GameException {
public:
    explicit InventoryFullException(const std::string& msg)
        : GameException("Inventory full: " + msg) {}
};

class DeadEntityException : public GameException {
public:
    explicit DeadEntityException(const std::string& msg)
        : GameException("Entity is dead: " + msg) {}
};

template<typename T>
class TypedGameException : public GameException {
    T errorData;

public:
    explicit TypedGameException(const std::string& msg, const T& data)
        : GameException(msg), errorData(data) {}

    const T& getErrorData() const { return errorData; }

    std::string getDetailedMessage() const {
        std::ostringstream oss;
        oss << what() << " [Error Data: " << errorData << "]";
        return oss.str();
    }
};

using InvalidValueException = TypedGameException<float>;
using InvalidIndexException = TypedGameException<size_t>;

#endif // EXCEPTIONS_H