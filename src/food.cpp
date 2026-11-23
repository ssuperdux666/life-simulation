#include "../include/food.h"

Food::Food() {
    x = GetRandomValue(-800,800);
    y = GetRandomValue(-800,800);

    value = GetRandomValue(18,92);
}

void Food::Draw(int camX, int camY) {
    int posXRELTOcam = x - camX;
    int posYRELTOcam = y- camY;

    float radius = 0.135f * value + 1.57f;
    
    if (posXRELTOcam - radius < 800 && posXRELTOcam + radius > 0 && posYRELTOcam - radius < 800 && posYRELTOcam + radius > 0) {
        DrawCircle(posXRELTOcam, posYRELTOcam, radius, GREEN);
    }
}