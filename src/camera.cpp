#include "../include/camera.h"
#include "../include/game.h"
#include "../include/config.h"

Camera_H::Camera_H() {
    x = -SETTINGS::SCREEN_WDITH/2;
    y = -SETTINGS::SCREEN_HEIGHT/2;

    zoom = 1;
}

void Camera_H::Move(int x, int y) {
    this->x += x * zoom;
    this->y += y * zoom;
}

void Camera_H::Frame_Update(Game& game) {
    if (IsKeyReleased(KEY_F)) {
        inspecting = !inspecting;
    }

    float mouseMovement = -(GetMouseWheelMove()) / 4.0f;

    if (mouseMovement != 0) {
        zoom += mouseMovement;
        if (zoom < 1) {
            zoom = 1;
        }else {
            x += SETTINGS::SCREEN_WDITH/2 * -mouseMovement;
            y += SETTINGS::SCREEN_HEIGHT/2 * -mouseMovement;
        }
    }

    if (IsKeyReleased(KEY_X)) {
        zoom += 1;
        x -= SETTINGS::SCREEN_WDITH/2;
        y -= SETTINGS::SCREEN_HEIGHT/2;
    }

    if (IsKeyReleased(KEY_Z)) {
        if (zoom > 1) {
            zoom -= 1;
            x += SETTINGS::SCREEN_WDITH/2;
            y += SETTINGS::SCREEN_HEIGHT/2;
        }
    }

    if (inspecting) {
        x = game.creatures[game.inspecting].x - SETTINGS::SCREEN_WDITH/2*zoom;
        y = game.creatures[game.inspecting].y - SETTINGS::SCREEN_HEIGHT/2*zoom;
    }else {
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
}
