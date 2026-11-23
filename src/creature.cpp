#include "../include/creature.h"
#include <random>
#include <iostream>

Creature::Creature() : localNN(5, 2){
    x = 0;
    y = 0;

    health = 100;
    hunger = 100;

    maxHealth = 100;
    maxHunger = 100;

    hungriness = 1;

    angle = 0;

    localNN.Create_Random_Connecttion();
    localNN.Create_Random_Connecttion();
    localNN.Create_Random_Connecttion();
    localNN.Create_Random_Connecttion();
    localNN.Create_Random_Connecttion();
    localNN.Create_Random_Connecttion();
    localNN.Create_Random_Connecttion();
    localNN.Create_Random_Connecttion();
    localNN.Create_Random_Connecttion();
}

void Creature::Draw(int camX, int camY) {
    int posXRELTOcam = x - camX;
    int posYRELTOcam = y- camY;

    float radius = 20;
    
    if (posXRELTOcam - radius < 800 && posXRELTOcam + radius > 0 && posYRELTOcam - radius < 800 && posYRELTOcam + radius > 0) {
        DrawCircle(posXRELTOcam, posYRELTOcam, radius, RED);
        DrawLine(posXRELTOcam, posYRELTOcam, posXRELTOcam+cos(angle*(PI/180))*radius*2, posYRELTOcam+sin(angle*(PI/180))*radius*2, BLUE);
    }
};

void Creature::kill() {

}

void Creature::tickUpdate(float dt) {
    hunger -= hungriness * dt;

    if (hunger < 0) {
        float emptiness = 0 - hunger;
        health -= emptiness;
        hunger += emptiness;
    }

    if (health < 0) {
        kill();
    }

    if (health < maxHealth) {
        float emptiness = (maxHealth - health)*dt;

        if (hunger - emptiness >= 10) {
            health += emptiness;
            hunger -= emptiness;
        }
    }

    for (float output : localNN.realOutputs) {
        //std::cout << output << "\n";
    }
}