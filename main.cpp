#include <iostream>
#include <vector>
#include <string>

class Character {
private:
    int windowWidth;
    int windowHeight;
    std::vector<float> screenPos;
    float width, height;
    float scale{4.0f};
    float health{100.0f};
    bool alive{true};

public:

    Character(int winWidth, int winHeight) : windowWidth(winWidth), windowHeight(winHeight),
      screenPos{float(winWidth) / 2, float(winHeight) / 2} {}


    Character(const Character& other): windowWidth{other.windowWidth}, windowHeight{other.windowHeight}, scale{other.scale},
        health{other.health}, alive{other.alive}, screenPos{other.screenPos}, width{other.width}, height{other.height} {
        std::cout << "Constr de copiere Character\n";

    }

    Character& operator=(const Character& other) {
        std::cout << "operator = copiere Character\n";
        windowWidth = other.windowWidth, windowHeight = other.windowHeight, scale = other.scale,
        health = other.health, alive = other.alive, screenPos = other.screenPos, width = other.width, height = other.height;
        return *this;
    }

    ~Character() {}

    void takeDamage(float damage) {
        health -= damage;
        if (health <= 0.0f) alive = false;
    }

    bool isAlive() const { return alive; }
};

class Enemy {
private:
    std::vector<float> worldPos{};
    float width{}, height{};
    float speed{3.5f};
    float health{100.0f};
    bool alive{true};
    float damagePerSec{10.0f};
    float radius{50.0f};

public:
    Enemy(std::vector<float> pos) : worldPos(pos) {}

    Enemy(const Enemy& other):
        worldPos(other.worldPos), width(other.width), height(other.height),
        speed(other.speed), health(other.health), alive(other.alive),
        damagePerSec(other.damagePerSec), radius(other.radius) {
        std::cout << "Constr de copiere Enemy\n";
    }

    Enemy& operator=(const Enemy& other) {
        std::cout << "operator = copiere Enemy\n";
        if (this == &other) {
            worldPos = other.worldPos;
            width = other.width;
            height = other.height;
            speed = other.speed;
            health = other.health;
            alive = other.alive;
            damagePerSec = other.damagePerSec;
            radius = other.radius;
        }
            return *this;
    }

    ~Enemy() {}

    void tick(float deltaTime) {
        if (!alive) return;
    }

    void takeDamage(float damage) {
        health -= damage;
        if (health <= 0.0f) alive = false;
    }

    bool isAlive() const { return alive; }
};

class Prop {
private:
    std::vector<float> worldPos{};
    float scale{4.0f};
    std::string texture;
    std::string name;

public:
    /*Prop(std::vector<float> pos, std::string tex, std::string n):
        worldPos(pos),
        //texture(tex),
        name(n){} */
    Prop(std::vector<float> pos, std::string n) : worldPos(pos), name(n) {}

    std::vector<float> getWorldPos() const { return worldPos; }
    std::string getTexture() const { return texture; }
    std::string getName() const { return name; }

    Prop(const Prop& other): worldPos(other.worldPos), scale(other.scale), name(other.name)
    //texture(other.texture)
    {
        std::cout << "Constr de copiere Prop\n";
    }

    Prop& operator=(const Prop& other) {
        std::cout << "operator = copiere Prop\n";
        if (this == &other) {
            worldPos = other.worldPos;
            scale = other.scale;
            name = other.name;
            //texture = other.texture;
        }
        return *this;
    }

    void printPos() const {
        std::cout << "The tree " << name << " is located at " << worldPos[0] << "," << worldPos[1] << "\n";
    }

};

/*class Item {
private:

public:

}; */

int main() {
    int windowWidth = 800, windowHeight = 600;
    Character knight1{windowWidth, windowHeight};
    Character knight2=knight1;

    Character knight3{1024, 768};
    knight3 = knight1;

    Enemy goblin({10.0f, 20.0f});
    Enemy goblin2 = goblin;
    Enemy goblin3({30.0f, 40.0f});
    goblin3 = goblin;

    Prop tree({100.0f, 200.0f}, "Oak");
    tree.printPos();

    Prop tree2({25.0f, 35.0f}, "Birch");
    tree2 = tree;

    tree2.printPos();
}

