#include "GameWorld.h"
#include "Item.h"
#include "Enemy.h"
#include "NPC.h"
#include "Obstacle.h"
#include "Prop.h"
#include "Exceptions.h"
#include "libs/termcolor/termcolor.hpp"
#include <iostream>
#include <chrono>
#include <thread>

GameWorld::GameWorld()
    : rng(std::random_device{}()), chanceDist(0, 1), turnCount(0), gameRunning(true) {}

void GameWorld::initialize() {
    try {
        auto startingWeapon = std::make_unique<Item>("Basic Sword", std::vector<float>{0.0f, 0.0f}, 10.0f, 0.0f);
        player = std::make_unique<Character>(800, 600, std::move(startingWeapon));

        auto potion1 = std::make_unique<Item>("Health Potion", std::vector<float>{0.0f, 0.0f}, 0.0f, 25.0f);
        auto potion2 = std::make_unique<Item>("Health Potion", std::vector<float>{0.0f, 0.0f}, 0.0f, 25.0f);

        player->addItem(std::move(potion1));
        player->addItem(std::move(potion2));

        entities.push_back(std::make_unique<Enemy>(std::vector<float>{300.0f, 400.0f}));
        entities.push_back(std::make_unique<Enemy>(std::vector<float>{200.0f, 300.0f}));
        entities.push_back(std::make_unique<NPC>(std::vector<float>{100.0f, 100.0f}, "Merchant", "Welcome traveler! Buy my wares!"));
        entities.push_back(std::make_unique<Obstacle>(std::vector<float>{150.0f, 150.0f}, "Dangerous Rock"));
        entities.push_back(std::make_unique<Prop>(std::vector<float>{50.0f, 50.0f}, "Decorative Statue"));

        items.push_back(std::make_unique<Item>("Excalibur", std::vector<float>{10.0f, 20.0f}, 50.0f, 0.0f));
        items.push_back(std::make_unique<Item>("Health Potion", std::vector<float>{15.0f, 25.0f}, 0.0f, 25.0f));

        std::cout << termcolor::cyan << "Game World initialized successfully!\n" << termcolor::reset;

    } catch (const std::exception& e) {
        std::cerr << termcolor::red << "Failed to initialize game: " << e.what() << termcolor::reset << "\n";
        gameRunning = false;
    }
}

void GameWorld::run() {
    if (!player) {
        std::cerr << termcolor::red << "Game not properly initialized!\n" << termcolor::reset;
        return;
    }

    std::cout << termcolor::cyan << "Welcome to the Adventure Game!\n" << termcolor::reset;

    while (isGameRunning()) {
        try {
            std::cout << termcolor::bold << "\n=== Turn " << ++turnCount << " ===\n" << termcolor::reset;
            displayGameState();
            displayMenu();

            int choice;
            std::cin >> choice;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Invalid input");
            }

            if (turnCount >= 500) {
                choice = 5;
            }

            handlePlayerAction(choice);

            if (isGameRunning()) {
                updateEntities(0.1f);
                processNearbyEntities();

                if (chanceDist(rng) == 1) {
                    spawnRandomItem();
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

        } catch (const DeadEntityException& e) {
            std::cerr << termcolor::red << "Fatal Error: " << e.what() << termcolor::reset << "\n";
            gameRunning = false;
        } catch (const InventoryFullException& e) {
            std::cerr << termcolor::yellow << "Inventory Error: " << e.what() << termcolor::reset << "\n";
        } catch (const std::exception& e) {
            std::cerr << termcolor::red << "Error: " << e.what() << termcolor::reset << "\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    if (!player->isAlive()) {
        std::cout << termcolor::red << "\nGAME OVER! You have perished on your journey.\n" << termcolor::reset;
    }

    std::cout << termcolor::cyan << "Thanks for playing this tiny adventure!\n" << termcolor::reset;
}

bool GameWorld::areClose(const GameEntity& a, const GameEntity& b) const {
    const auto& posA = a.getPosition();
    const auto& posB = b.getPosition();

    if (posA.size() < 2 || posB.size() < 2) return false;

    const float dx = posA[0] - posB[0];
    const float dy = posA[1] - posB[1];

    return (dx*dx + dy*dy) <= (INTERACTION_DISTANCE * INTERACTION_DISTANCE);
}

void GameWorld::spawnRandomItem() {
    if (items.size() < 5) {
        auto newItem = std::make_unique<Item>(
            "Random Health Potion",
            std::vector<float>{
                player->getPosition()[0] + static_cast<float>(std::uniform_int_distribution<>(-20, 20)(rng)),
                player->getPosition()[1] + static_cast<float>(std::uniform_int_distribution<>(-20, 20)(rng))
            },
            0.0f, 25.0f
        );
        items.push_back(std::move(newItem));
        std::cout << termcolor::green << "A shimmering potion materializes nearby!\n" << termcolor::reset;
    }
}

void GameWorld::updateEntities(float deltaTime) {
    try {
        player->update(deltaTime);

        for (auto& entity : entities) {
            if (entity && entity->isAlive()) {
                entity->update(deltaTime);
            }
        }

        entities.erase(
            std::remove_if(entities.begin(), entities.end(),
                [](const std::unique_ptr<GameEntity>& entity) {
                    return !entity || !entity->isAlive();
                }),
            entities.end()
        );

    } catch (const DeadEntityException& e) {
        throw;
    }
}

void GameWorld::handlePlayerAction(int action) {
    switch (action) {
        case 1: handleMove(); break;
        case 2: handleAttack(); break;
        case 3: handleUseItem(); break;
        case 4: handleTalkToNPC(); break;
        case 5:
            std::cout << "You chose to end your journey. Goodbye!\n";
            gameRunning = false;
            break;
        default:
            throw std::invalid_argument("Invalid choice. Please select between 1-5.");
    }
}

void GameWorld::handleMove() {
    float dx, dy;
    std::cout << "Enter direction (x y): ";

    std::cin.clear();

    if (!(std::cin >> dx >> dy)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Invalid movement input");
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    player->move(dx, dy);

    const auto& pos = player->getPosition();
    if (pos.size() >= 2) {
        std::cout << "Moved to position: (" << pos[0] << ", " << pos[1] << ")\n";
    } else {
        std::cout << "Moved, but position tracking is invalid\n";
    }

    // Random encounter while moving
    if (chanceDist(rng) == 1) {
        std::cout << termcolor::yellow << "A wild enemy appears and slashes you!\n" << termcolor::reset;
        player->takeDamage(5.0f);
    }
}

void GameWorld::handleAttack() {
    std::cout << "You swing your weapon in front of you...\n";
    bool enemyHit = false;

    for (auto& entity : entities) {
        if (entity && areClose(*player, *entity)) {
            if (auto* enemy = dynamic_cast<Enemy*>(entity.get())) {
                std::cout << "You engage in combat with " << entity->getName() << "!\n";
                player->attack();
                entity->interact(*player);
                enemyHit = true;
            }
        }
    }

    if (!enemyHit) {
        std::cout << "You swing at the air - no enemies in range!\n";
    }
}

void GameWorld::handleUseItem() {
    const auto& inventory = player->getInventory();

    if (inventory.empty()) {
        std::cout << "Your inventory is empty.\n";
        return;
    }

    std::cout << "Your inventory:\n";
    for (size_t i = 0; i < inventory.size(); ++i) {
        std::cout << i + 1 << ". " << *inventory[i] << "\n";
    }

    std::cout << "Enter item number to use (0 to cancel): ";
    size_t choice;
    std::cin >> choice;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw std::invalid_argument("Invalid item selection");
    }

    if (choice == 0) {
        std::cout << "Cancelled.\n";
        return;
    }

    if (choice > inventory.size()) {
        throw std::out_of_range("Invalid item index");
    }

    player->useItemFromInventory(choice - 1);
}

void GameWorld::handleTalkToNPC() {
    bool foundNPC = false;

    for (auto& entity : entities) {
        if (entity && areClose(*player, *entity)) {
            if (auto* npc = dynamic_cast<NPC*>(entity.get())) {
                foundNPC = true;
                entity->interact(*player);

                if (npc->getName() == "Merchant") {
                    std::cout << "Merchant: I can offer you a Health Potion for free, would you like to take it? (y/n): ";
                    char response;
                    std::cin >> response;

                    if (response == 'y' || response == 'Y') {
                        try {
                            auto giftedPotion = std::make_unique<Item>(
                                "Merchant's Health Potion",
                                player->getPosition(),
                                0.0f, 30.0f
                            );
                            float healAmount = giftedPotion->getHealingAmount();
                            player->addItem(std::move(giftedPotion));
                            std::cout << "You received a Health Potion from the merchant that restores "
                                      << healAmount << " HP!\n";
                        } catch (const InventoryFullException& e) {
                            std::cout << "Your inventory is full. Merchant frowns and puts the potion away.\n";
                            throw;
                        }
                    } else {
                        std::cout << "Merchant: MMMmaybe next time. (angry stare)\n";
                    }
                }
                break;
            }
        }
    }

    if (!foundNPC) {
        std::cout << termcolor::dark << "There are no NPCs nearby to talk to.\n" << termcolor::reset;
    }
}

void GameWorld::processNearbyEntities() {
    for (auto it = items.begin(); it != items.end();) {
        if (*it && areClose(*player, **it)) {
            std::cout << termcolor::magenta << "You lucky! You randomly discovered " << (*it)->getName() << "!\n" << termcolor::reset;
            try {
                (*it)->interact(*player);
                it = items.erase(it);
            } catch (const InventoryFullException& e) {
                std::cout << termcolor::yellow << "Your inventory is full! " << e.what() << termcolor::reset << "\n";
                ++it;
            }
        } else {
            ++it;
        }
    }

    for (auto& entity : entities) {
        if (entity && areClose(*player, *entity)) {
            if (dynamic_cast<NPC*>(entity.get())) {
                std::cout << termcolor::blue << "You see " << entity->getName() << " nearby...\n" << termcolor::reset;
            } else if (dynamic_cast<Obstacle*>(entity.get())) {
                entity->interact(*player);
            }
        }
    }
}

void GameWorld::displayGameState() const {
    std::cout << *player << "\n";

    std::cout << "Nearby entities: ";
    bool hasNearby = false;
    for (const auto& entity : entities) {
        if (entity && areClose(*player, *entity)) {
            std::cout << entity->getName() << " ";
            hasNearby = true;
        }
    }
    for (const auto& item : items) {
        if (item && areClose(*player, *item)) {
            std::cout << item->getName() << " ";
            hasNearby = true;
        }
    }
    if (!hasNearby) {
        std::cout << "None";
    }
    std::cout << "\n";
}

void GameWorld::displayMenu() const {
    std::cout << "\nActions:\n"
              << "1. Move\n2. Attack\n3. Use Item from Inventory\n4. Talk to NPC\n5. Quit\n"
              << "Choose: ";
}
