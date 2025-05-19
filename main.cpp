#include "libs/termcolor/termcolor.hpp"
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <random>
#include "GameEntity.h"
#include "Character.h"
#include "Prop.h"
#include "Item.h"
#include "Enemy.h"
#include "Obstacle.h"
#include "Exceptions.h"

// Helper function
int randomChoice() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> distrib(0, 1);
    return distrib(gen);
}

int main() {
    try {
        // Initialize game entities using smart pointers
        auto superSword = std::make_unique<Item>("Excalibur", std::vector<float>{10.0f, 20.0f}, 50.0f, 0.0f);
        auto superSword2 = std::make_unique<Item>("Wallace", std::vector<float>{10.0f, 15.0f}, 40.0f, 0.0f);
        auto swordProp = std::make_unique<Prop>(std::vector<float>{50.0f, 60.0f}, "Sword");

        // Create character with weapon
        Character knight(800, 600, std::move(swordProp));

        // Create enemies and obstacles
        Enemy goblin({300.0f, 400.0f});
        auto potion = std::make_unique<Item>("XP", std::vector<float>{5.0f, 15.0f}, 0.0f, 25.0f);
        Obstacle obstacle1({8.0f, 8.0f}, "Obstacle1");
        Obstacle obstacle2({12.0f, 12.0f}, "Obstacle2");

        bool running = true;
        int cnt = 0;

        while (running) {
            auto frameStart = std::chrono::steady_clock::now();

            std::cout << "\nChoose an action: (1) Attack (2) Move (3) Equip Item (4) Use Potion (5) Quit\n";
            int choice;
            std::cin >> choice;

            if (cnt == 3) cnt = 0;
            else cnt++;

            // Random damage unless moving or equipping
            if (choice != 3 && choice != 2) {
                if (knight.getHealth() > 0) {
                    knight.takeDamage(1.0f);
                    std::cout << "Character took damage! Health: " << knight.getHealth() << "\n";
                }
                knight.autoHeal();
                goblin.attack(knight);
            }

            // Game over check
            if (knight.getHealth() <= 0) {
                std::cout << "Game Over!\n";
                break;
            }

            switch (choice) {
                case 1:
                    knight.attack();
                    break;
                case 2: {
                    float dx, dy;
                    std::cout << "Enter movement (dx dy): ";
                    std::cin >> dx >> dy;
                    knight.move(dx, dy);
                    std::cout << "New position: " << knight << "\n";

                    // Random encounter chance
                    if (randomChoice() == 1) {
                        knight.takeDamage(1.0f);
                        std::cout << "Random encounter! Health: " << knight.getHealth() << "\n";
                        knight.autoHeal();
                        goblin.attack(knight);
                    }
                    break;
                }
                case 3:
                    if (!superSword->isEquipped()) {
                        knight.addItemToInventory(*superSword);
                        std::cout << "Equipped Excalibur!\n";
                    }
                    break;
                case 4:
                    knight.heal(potion->getHealingAmount());
                    std::cout << "Used potion! Health: " << knight.getHealth() << "\n";
                    break;
                case 5:
                    running = false;
                    break;
                default:
                    std::cout << "Invalid choice!\n";
                    running = false;
                    break;
            }

            if (!running) break;

            // Random obstacle interactions
            if (randomChoice()) obstacle1.checkDangerZone(knight);
            if (randomChoice()) obstacle2.checkDangerZone(knight);

            // Periodic upgrades/degradations
            if (cnt == 3 && choice != 5) superSword->upgradeItem(10);
            if (randomChoice()) superSword->equipItem();
            if (randomChoice()) superSword2->degradeItem(5);

            // Special attacks
            if (randomChoice()) superSword->specialAttack();
            if (randomChoice()) superSword2->specialAttack();

            // Item management
            if (randomChoice()) potion->unequipItem();
            if (randomChoice()) potion->dropItem(10.0f, 20.0f);

            // Frame rate control
            auto frameEnd = std::chrono::steady_clock::now();
            auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
            std::this_thread::sleep_for(std::chrono::milliseconds(16) - frameDuration);
        }
    }
    catch (const InvalidPositionException& e) {
        std::cerr << termcolor::red << "Position error: " << e.what() << termcolor::reset << std::endl;
    }
    catch (const InventoryFullException& e) {
        std::cerr << termcolor::red << "Inventory error: " << e.what() << termcolor::reset << std::endl;
    }
    catch (const DeadEntityException& e) {
        std::cerr << termcolor::red << "Entity error: " << e.what() << termcolor::reset << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << termcolor::red << "Unexpected error: " << e.what() << termcolor::reset << std::endl;
    }

    return 0;
}