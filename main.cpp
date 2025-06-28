#include "GameWorld.h"
#include <iostream>

int main() {
    try {
        GameWorld world;
        world.initialize();
        world.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}