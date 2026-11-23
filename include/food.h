#pragma once
#include "raylib.h"

class Food {
    public:
        int x;
        int y;

        int value;

        Food();

        void Draw(int camX, int camY);
};