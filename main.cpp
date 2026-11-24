#include <iostream>
#include <string>
#include "include/game.h"
#include <raylib.h>
#include "include/config.h"

int main() {

    InitWindow(SETTINGS::SCREEN_WDITH, SETTINGS::SCREEN_HEIGHT, "Life Simulation");

    SetTargetFPS(240);

    SetRandomSeed(time(nullptr));

    Game mainGame = Game();

    float startTime = GetTime();
    float fpsTimer = startTime;
    int fps = 0;
    int frames = 0;

    while (!WindowShouldClose()) {
        frames++;

        float dt = GetFrameTime();
        float currentTime = GetTime();
        
        mainGame.gameCamera.Frame_Update(mainGame);

        mainGame.Physics_Update(dt);

        if (currentTime - startTime > 0.2f) {
            mainGame.Frame_Update(0.2f);
            startTime = currentTime;
        }

        if (currentTime - fpsTimer > 1.0f) {
            fps = frames;
            frames = 0;
            fpsTimer = currentTime;
        }

        BeginDrawing();

            ClearBackground({23, 25, 35});

            mainGame.Draw();

            DrawText(("FPS: " + std::to_string(fps)).c_str(), 0, 0, 20, WHITE);

        EndDrawing();

        if (mainGame.foodRegions.size() < 23) {
            Food_Region region;
            mainGame.foodRegions.push_back(region);
        }
    }

    CloseWindow();
    
    return 0;
}