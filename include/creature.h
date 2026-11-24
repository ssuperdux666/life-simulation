#pragma once
#include "raylib.h"
#include "food.h"
#include <algorithm>
#include <string>
#include "Network.h"
#include "game.h"

class Creature {
    public:
        float x;
        float y;

        float xVel;
        float yVel;

        float xAcc;
        float yAcc;

        float angle; //in degrees
        float angularVel;

        float health;
        float hunger;

        int maxHealth;
        int maxHunger;

        float hungriness;

        float birthingCD = 0;

        Color color;

        Food* nearestFood;
        float nearestDistance;

        NeuralNetwork localNN;

        bool death = false;

        std::string name;
        std::string LastName;

        int generation;

        std::string consonants = "bcdfghjklmnpqrstvwxyz";
        std::string vowels = "aeiou";

        std::string combos[17] = {
            "ly", "ai", "eo", "io", "ua", "ei",
            "iu", "ae", "oi", "au", "eu", "ie",
            "uo", "oa", "ui", "ea", "ou"
        };

        Creature();

        std::string generate_Name(int minLen, int maxLen, bool crazy);

        void Draw(int camX, int camY, float zoom);

        void Draw(int camX, int camY, float zoom, Color color);

        void tickUpdate(float dt, Game& game);

        void kill();

        void Birth(Game& game);
};