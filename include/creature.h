#pragma once
#include "raylib.h"
#include "Network.h"

class Creature {
    public:
        int x;
        int y;

        float angle; //in degrees

        float health;
        float hunger;

        float maxHealth;
        float maxHunger;

        float hungriness;

        NeuralNetwork localNN;

        Creature();

        void Draw(int camX, int camY);

        void tickUpdate(float dt);

        void kill();
};