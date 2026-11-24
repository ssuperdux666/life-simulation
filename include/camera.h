#pragma once
#include "raylib.h"


class Game;

class Camera_H {
    public:
        int x;
        int y;

        int lastX;
        int lastY;

        float zoom;

        int holding = false;

        bool inspecting = false;

        Camera_H();

        void Move(int x, int y);
        void Frame_Update(Game& game);
};