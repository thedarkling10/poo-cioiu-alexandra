#pragma once
#include <stdexcept>
#include <string>

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