#include "../include/game.h"
#include "../include/config.h"

Game::Game() {
    gameCamera = Camera_H();

    for (int i = 0; i < 2500; i++) {
        Creature cutie = Creature();

        cutie.localNN.Mutate();
        cutie.localNN.Mutate();
        cutie.localNN.Mutate();

        creatures.push_back(std::move(cutie));
    }

    inspecting = 0;
}

void Game::Draw() {
    for (Food food : foods) {
        food.Draw(gameCamera.x, gameCamera.y, gameCamera.zoom);
    }

    int cretIndex = 0;
    for (Creature& creature : creatures) {

        if (cretIndex != inspecting) {
            creature.Draw(gameCamera.x, gameCamera.y, gameCamera.zoom);
        }
        cretIndex++;
    }

    creatures[inspecting].Draw(gameCamera.x, gameCamera.y, gameCamera.zoom, {255,255,255,255});

    std::string fullName = "name: " + creatures[inspecting].name;
    std::string fullLastName = "family name: " + creatures[inspecting].LastName;
    std::string generation = "generation: " + std::to_string(creatures[inspecting].generation);

    DrawText(fullName.c_str(), SETTINGS::SCREEN_WDITH-MeasureText(fullName.c_str(), 20)-10, 10, 20, WHITE);
    DrawText(fullLastName.c_str(), SETTINGS::SCREEN_WDITH-MeasureText(fullLastName.c_str(), 20)-10, 30, 20, WHITE);
    DrawText(generation.c_str(), SETTINGS::SCREEN_WDITH-MeasureText(generation.c_str(), 20)-10, 50, 20, WHITE);

    DrawText(("Alive: " + std::to_string(creatures.size())).c_str(), 10, SETTINGS::SCREEN_HEIGHT-110, 20, WHITE);

    DrawText(("X: " + std::to_string(gameCamera.x+SETTINGS::SCREEN_WDITH)).c_str(), 10, SETTINGS::SCREEN_HEIGHT-90, 20, WHITE);
    DrawText(("Y: " + std::to_string(gameCamera.y+SETTINGS::SCREEN_HEIGHT)).c_str(), 10, SETTINGS::SCREEN_HEIGHT-70, 20, WHITE);

    DrawText(("health: " + std::to_string(creatures[inspecting].health)).c_str(), 10, SETTINGS::SCREEN_HEIGHT-50, 20, WHITE);
    DrawText(("hunger: " + std::to_string(creatures[inspecting].hunger)).c_str(), 10, SETTINGS::SCREEN_HEIGHT-30, 20, WHITE);
}

void Game::Frame_Update(float dt) {
    for (int i = 0; i < creatures.size(); i++) {
        if (creatures[i].death) {
            deathMarking.push_back(i);
            continue;
        }
        creatures[i].localNN.Forward_Pass(*this, creatures[i]);
        creatures[i].tickUpdate(dt, *this);
    }
    for (int i = 0; i < foods.size(); i++) {
        if (foods[i].value == 0) {
            eatenMarking.push_back(i);
        }
    }

    for (int i = 0; i < foodRegions.size(); i++) {
        if (foodRegions[i].foodReserves <= 0) {
            emptyMarking.push_back(i);
            continue;
        }
        foodRegions[i].Frame_Update(*this, dt);
    }

    std::sort(emptyMarking.begin(), emptyMarking.end(), std::greater<int>());

    for (int index : emptyMarking) {
        if (index >= 0 && index < foodRegions.size()) {
            foodRegions.erase(foodRegions.begin() + index);
        }
    }

    emptyMarking.clear();

    std::sort(deathMarking.begin(), deathMarking.end(), std::greater<int>());

    for (int index : deathMarking) {
        if (index >= 0 && index < creatures.size()) {
            if (inspecting == index) {
                inspecting = 0;
            }else if (index < inspecting) {
                inspecting--;
            }
            creatures.erase(creatures.begin() + index);
        }
    }

    deathMarking.clear();

    std::sort(eatenMarking.begin(), eatenMarking.end(), std::greater<int>());

    for (int index : eatenMarking) {
        if (index >= 0 && index < foods.size()) {
            foods.erase(foods.begin() + index);
        }
    }

    eatenMarking.clear();

    for (Creature& newborn : pendingBirths) {
        creatures.push_back(std::move(newborn));
    }

    pendingBirths.clear();
}

void Game::Physics_Update(float dt) {
    for (Creature& creature : creatures) {
        float newXVel = std::clamp(creature.xVel + creature.xAcc * dt, -200.0f, 200.0f);
        float newYVel = std::clamp(creature.yVel + creature.yAcc * dt, -200.0f, 200.0f);
        creature.xVel = newXVel;
        creature.yVel = newYVel;

        creature.x += newXVel * dt;
        creature.y += newYVel * dt;

        creature.angle += creature.angularVel * dt;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse = GetMousePosition();

        int nearestCreature = -1;
        float nearestDistance = 20.0f;

        for (int i = 0; i < creatures.size(); i++)
        {
            float distance = std::hypot(
                mouse.x - (creatures[i].x - gameCamera.x) / gameCamera.zoom,
                mouse.y - (creatures[i].y - gameCamera.y) / gameCamera.zoom
            );

            if (distance < nearestDistance)
            {
                nearestDistance = distance;
                nearestCreature = i;
            }
        }

        if (nearestCreature != -1)
            inspecting = nearestCreature;
    }
}

void Game::Spawn_Food(int minX, int xRange, int minY, int yRange) {
    Food food = Food();

    food.x = GetRandomValue(minX, minX+xRange);
    food.y = GetRandomValue(minY, minY+yRange);

    foods.push_back(food);
}

Food* Game::Get_Nearest_Food(int x, int y) {
    Food* nearestFood = nullptr;
    float nearestDistance = HUGE;

    for (Food& food : foods) {
        float distance = std::hypot(x - food.x, y - food.y);
        if (distance < nearestDistance) {
            nearestFood = &food;
            nearestDistance = distance;
        }
    }
    
    return nearestFood;
}