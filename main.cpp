#include "libs/termcolor/termcolor.hpp"
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <random>
#include "GameEntity.h"
#include "Character.h"
#include "Item.h"
#include "Enemy.h"
#include "NPC.h"
#include "Exceptions.h"

bool areClose(const GameEntity& a, const GameEntity& b) {
    const auto& posA = a.getPosition();
    const auto& posB = b.getPosition();

    if (posA.size() < 2 || posB.size() < 2) return false;

    constexpr float threshold = 50.0f;
    const float dx = posA[0] - posB[0];
    const float dy = posA[1] - posB[1];

    return (dx*dx + dy*dy) <= (threshold * threshold);
}

int randomChance(int min = 0, int max = 1) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

int main() {
    try {
        // === Inițializare atribute din joc ===
        auto excalibur = std::make_unique<Item>("Excalibur", std::vector<float>{10.0f, 20.0f}, 50.0f, 0.0f);
        auto healthPotion = std::make_unique<Item>("Health Potion", std::vector<float>{15.0f, 25.0f}, 0.0f, 25.0f);

        Character player(800, 600, std::make_unique<Item>("Basic Sword", std::vector<float>{0.0f, 0.0f}, 10.0f, 0.0f));

        Enemy goblin({300.0f, 400.0f});
        Enemy orc({200.0f, 300.0f});

        NPC merchant({100.0f, 100.0f}, "Merchant", "Welcome traveler! Buy my wares!");

        bool gameRunning = true;
        int turnCount = 0;

        std::cout << termcolor::cyan << "Welcome to the Adventure Game!\n" << termcolor::reset;

        while (gameRunning && player.isAlive()) {
            std::cout << termcolor::bold << "\n=== Turn " << ++turnCount << " ===\n" << termcolor::reset;
            std::cout << player << "\n";

            std::cout << "\nActions:\n"
                      << "1. Move\n2. Attack\n3. Use Item\n4. Talk to NPC\n5. Quit\n"
                      << "Choose: ";

            int choice;
            if (turnCount >= 500)
                choice = 5;
            else
                std::cin >> choice;

            switch (choice) {
                case 1: { // Move
                    float dx, dy;
                    std::cout << "Enter direction (x y): ";
                    std::cin >> dx >> dy;
                    player.move(dx, dy);

                    if (randomChance() == 1) {
                        std::cout << termcolor::yellow << "A wild enemy appears and slashes you!\n" << termcolor::reset;
                        player.takeDamage(5.0f);
                    }

                    if (areClose(player, *excalibur)) {
                        std::cout << termcolor::magenta << "You discovered the legendary Excalibur!\n" << termcolor::reset;
                        player.interact(*excalibur);
                        excalibur.reset(); // consumat
                    }

                    if (healthPotion && areClose(player, *healthPotion)) {
                        std::cout << termcolor::green << "You found a health potion on the ground.\n" << termcolor::reset;
                        player.interact(*healthPotion);
                        healthPotion.reset();
                    }

                    if (areClose(player, merchant)) {
                        std::cout << termcolor::blue << "You see a mysterious figure nearby... It's the "
                                  << merchant.getName() << "!\n"
                                  << "He looks like he has something to offer.\n" << termcolor::reset;
                    }
                    break;
                }

                case 2: // Attack
                    std::cout << "You swing your weapon in front of you...\n";
                    player.attack();

                    if (areClose(player, goblin)) {
                        std::cout << "A goblin jumps in to fight!\n";
                        goblin.interact(player);
                    }

                    if (areClose(player, orc)) {
                        std::cout << "An orc growls and charges at you!\n";
                        orc.interact(player);
                    }
                    break;

                case 3: // Use Item
                    if (healthPotion && areClose(player, *healthPotion)) {
                        std::cout << "You reach out for the health potion...\n";
                        player.interact(*healthPotion);
                        healthPotion.reset();
                    } else {
                        std::cout << "There are no usable items nearby.\n";
                    }
                    break;

                case 4: // Talk to NPC
                    if (areClose(player, merchant)) {
                        merchant.interact(player);

                        // Dialog:
                        std::cout << "Merchant: I can offer you a Health Potion for free, would you like to take it? (y/n): ";
                        char response;
                        std::cin >> response;
                        if (response == 'y' || response == 'Y') {
                            try {
                                auto giftedPotion = std::make_unique<Item>("Health Potion", player.getPosition(), 0.0f, 25.0f);
                                float healAmount = giftedPotion->getHealingAmount(); // Store healing amount before moving
                                player.addItem(std::move(giftedPotion));
                                player.heal(25.0f);
                                std::cout << "You received a Health Potion from the merchant that restores "
                                          << healAmount << " HP!\n";

                            } catch (const InventoryFullException&) {
                                std::cout << "Your inventory is full. Merchant frowns and puts the potion away.\n";
                            }
                        } else {
                            std::cout << "Merchant: Very well, maybe next time.\n";
                        }

                    } else {
                        std::cout << termcolor::dark << "There are no NPCs nearby to talk to.\n" << termcolor::reset;
                    }
                    break;

                case 5: // Quit
                    std::cout << "You chose to end your journey. Goodbye!\n";
                    gameRunning = false;
                    break;

                default:
                    std::cout << "Invalid choice. Please select between 1-5.\n";
            }

            // === Enemy actiuni===
            if (player.isAlive()) {
                goblin.update(0.1f);
                orc.update(0.1f);
                player.update(0.1f);

                // Uneori apare o poțiune
                if (randomChance() == 1 && !healthPotion) {
                    healthPotion = std::make_unique<Item>(
                        "Health Potion",
                        std::vector<float>{
                        player.getPosition()[0] + static_cast<float>(randomChance(-20, 20)),
                        player.getPosition()[1] + static_cast<float>(randomChance(-20, 20))
},
                        0.0f, 25.0f);
                    std::cout << termcolor::green << "A shimmering potion materializes nearby!\n" << termcolor::reset;
                }
            }

            if (!player.isAlive()) {
                std::cout << termcolor::red << "\nGAME OVER! You have perished on your journey.\n" << termcolor::reset;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        std::cout << termcolor::cyan << "Thanks for playing this tiny adventure!\n" << termcolor::reset;
    }
    catch (const InventoryFullException& e) {
        std::cerr << termcolor::red << "Inventory Error: " << e.what() << termcolor::reset << "\n";
    }
    catch (const DeadEntityException& e) {
        std::cerr << termcolor::red << "Fatal Error: " << e.what() << termcolor::reset << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << termcolor::red << "Unexpected Error: " << e.what() << termcolor::reset << "\n";
    }

    return 0;
}