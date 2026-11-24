#pragma once
#include "food.h"
#include "raylib.h"

class Game;

class Food_Region {
    public:
        Food_Region();

        void Frame_Update(Game& game, float dt);

        int minX;
        int xRange;

        int minY;
        int yRange;

        float foodDensity;

        int startFood;
        int maxFood;

        int foodReserves;

        float amountedFood;
};