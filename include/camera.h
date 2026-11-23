#pragma once
#include "raylib.h"

class Camera_H {
    public:
        int x;
        int y;

        int lastX;
        int lastY;

        int holding = false;

        Camera_H();

        void Move(int x, int y);
        void Frame_Update();
};