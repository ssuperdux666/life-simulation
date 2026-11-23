#include "../include/camera.h"

Camera_H::Camera_H() {
    x = 0;
    y = 0;
}

void Camera_H::Move(int x, int y) {
    this->x += x;
    this->y += y;
}

void Camera_H::Frame_Update() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        holding = true;

        Vector2 current = GetMousePosition();
        lastX = current.x;
        lastY = current.y;
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
        holding = false;
    }

    if (holding) {
        Vector2 current = GetMousePosition();

        float differenceX = lastX - current.x;
        float differenceY = lastY - current.y;

        Move(differenceX, differenceY);

        lastX = current.x;
        lastY = current.y;
    }
}
