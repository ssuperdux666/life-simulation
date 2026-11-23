#include <iostream>
#include <string>
#include "include/game.h"
#include <raylib.h>

int main() {

    InitWindow(800, 800, "Life Simulation");

    SetTargetFPS(240);

    Game mainGame = Game();

    for (int i = 0; i < 100; i++) {
        mainGame.Spawn_Food();
    }

    float startTime = GetTime();

    while (!WindowShouldClose()) {

        float dt = GetFrameTime();
        int fps = 1/dt;

        mainGame.gameCamera.Frame_Update();

        float currentTime = GetTime();

        if (currentTime - startTime > 0.1f) {
            mainGame.Frame_Update(0.1f);
            startTime = currentTime;
        }

        BeginDrawing();

            ClearBackground({23, 25, 35});

            mainGame.Draw();

            DrawText(("FPS: " + std::to_string(fps)).c_str(), 0, 0, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}