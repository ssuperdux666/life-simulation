#pragma once
#include "camera.h"
#include "creature.h"
#include "food.h"
#include "raylib.h"
#include <vector>
#include <cmath>
#include <string>

class Game {
    public:
        Camera_H gameCamera;

        std::vector<Creature> creatures;
        std::vector<Food> foods;

        int inspecting;

        Game();

        void Draw();

        void Frame_Update(float dt);

        void Spawn_Food();

        Food Get_Nearest_Food(int x, int y);
};