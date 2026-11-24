#include "../include/food.h"

Food::Food() {
    x = 0;
    y = 0;

    value = GetRandomValue(18,92);
}

void Food::Draw(int camX, int camY, float zoom) {
    int posXRELTOcam = (x - camX) / zoom;
    int posYRELTOcam = (y- camY) / zoom;

    float radius = (0.135f * value + 1.57f) / zoom;
    
    if (posXRELTOcam - radius < 1200 && posXRELTOcam + radius > 0 && posYRELTOcam - radius < 1200 && posYRELTOcam + radius > 0) {
        DrawCircle(posXRELTOcam, posYRELTOcam, radius, GREEN);
    }
}