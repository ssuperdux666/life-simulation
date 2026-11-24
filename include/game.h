#pragma once
#include "camera.h"
#include "creature.h"
#include "food.h"
#include "foodRegion.h"
#include "raylib.h"
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <iostream>

class Game {
    public:
        Camera_H gameCamera;

        std::vector<Creature> creatures;
        std::vector<Food> foods;

        std::vector<Food_Region> foodRegions;

        std::vector<int> emptyMarking;
        std::vector<int> deathMarking;
        std::vector<int> eatenMarking;

        std::vector<Creature> pendingBirths;

        int inspecting;

        Game();

        void Draw();

        void Frame_Update(float dt);

        void Physics_Update(float dt);

        void Spawn_Food(int minX, int xRange, int minY, int yRange);

        Food* Get_Nearest_Food(int x, int y);
};