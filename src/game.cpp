#include "../include/game.h"

Game::Game() {
    gameCamera = Camera_H();

    for (int i = 0; i < 3000; i++) {
        Creature cutie = Creature();

        creatures.push_back(cutie);
    }

    inspecting = 0;
}

void Game::Draw() {
    for (Food food : foods) {
        food.Draw(gameCamera.x, gameCamera.y);
    }

    for (Creature creature : creatures) {
        creature.Draw(gameCamera.x, gameCamera.y);
    }

    DrawText(("health: " + std::to_string(creatures[inspecting].health)).c_str(), 0, 760, 20, WHITE);
    DrawText(("hunger: " + std::to_string(creatures[inspecting].hunger)).c_str(), 0, 780, 20, WHITE);
}

void Game::Frame_Update(float dt) {
    for (Creature& creature : creatures) {
        creature.tickUpdate(dt);
        creature.localNN.Forward_Pass(*this, creature);
    }
}

void Game::Spawn_Food() {
    Food food = Food();
    foods.push_back(food);
}

Food Game::Get_Nearest_Food(int x, int y) {
    Food nearestFood;
    float nearestDistance = HUGE;

    for (Food food : foods) {
        double distance = std::hypot(x - food.x, y - food.y);
        if (distance < nearestDistance) {
            nearestFood = food;
            nearestDistance = distance;
        }
    }
    
    return nearestFood;
}