#include "../include/foodRegion.h"
#include "../include/game.h"

Food_Region::Food_Region() {
    minX = GetRandomValue(-10000,10000);
    xRange = GetRandomValue(200,1200);

    minY = GetRandomValue(-10000,10000);
    yRange = GetRandomValue(200,1200);

    foodDensity = GetRandomValue(5,200) / 100.0f;

    foodReserves = xRange * yRange * foodDensity / 1000.0f;

    maxFood = foodReserves;

    startFood = foodReserves * GetRandomValue(1,50) / 100.0f;

    foodReserves -= startFood;

    amountedFood = 0;
}

void Food_Region::Frame_Update(Game& game, float dt) {
    float currentFoodAddition = 0.05f * (sqrt(foodReserves)) * dt;

    if (foodReserves > 0) {
        foodReserves -= currentFoodAddition;
        amountedFood += currentFoodAddition;
    }

    if (amountedFood > 1) {
        amountedFood -= 1;
        game.Spawn_Food(minX, xRange, minY, yRange);
    }

}