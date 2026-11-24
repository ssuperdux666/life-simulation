#include "../include/creature.h"
#include <random>
#include <iostream>
#include <string>

Creature::Creature() : localNN(5, 2){
    x = 0;
    y = 0;
    
    xVel = 0;
    yVel = 0;

    xAcc = 0;
    yAcc = 0;


    angularVel = 0;

    health = 100;
    hunger = 100;

    maxHealth = 100;
    maxHunger = 200;

    hungriness = 1;

    angle = GetRandomValue(0,360);

    color = {(unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255};

    name = generate_Name(4, 8, false);
    LastName = generate_Name(6, 16, true);
    generation = 1;
}

std::string Creature::generate_Name(int minLen, int maxLen, bool crazy)
{
    int length = GetRandomValue(minLen, maxLen);
    std::string name = "";

    int index = 0;

    while (index < length)
    {
        if (crazy)
        {
            if (GetRandomValue(0, 999) < 100)
            {
                name += combos[GetRandomValue(0, 16)];
                index += 2;
            }
            else if (GetRandomValue(0, 999) < 150)
            {
                name += consonants[GetRandomValue(0, consonants.size() - 1)];
                name += consonants[GetRandomValue(0, consonants.size() - 1)];
                index += 2;
            }
            else
            {
                if (index % 2 == 0)
                    name += consonants[GetRandomValue(0, consonants.size() - 1)];
                else
                    name += vowels[GetRandomValue(0, vowels.size() - 1)];

                index++;
            }
        }
        else
        {
            if (GetRandomValue(0, 999) < 15 && index + 1 < length)
            {
                name += combos[GetRandomValue(0, 16)];
                index += 2;
            }
            else
            {
                if (index % 2 == 0)
                    name += consonants[GetRandomValue(0, consonants.size() - 1)];
                else
                    name += vowels[GetRandomValue(0, vowels.size() - 1)];

                index++;
            }
        }
    }

    name[0] = toupper(name[0]);

    return name;
}

void Creature::Draw(int camX, int camY, float zoom) {
    float posXRELTOcam = (x - camX) / zoom;
    float posYRELTOcam = (y- camY) / zoom;

    float radius = 20/zoom;
    
    if (posXRELTOcam - radius < 1200 && posXRELTOcam + radius > 0 && posYRELTOcam - radius < 1200 && posYRELTOcam + radius > 0) {
        DrawCircle(posXRELTOcam, posYRELTOcam, radius, color);
        DrawLine(posXRELTOcam, posYRELTOcam, posXRELTOcam+cos(angle*(PI/180))*radius*2, posYRELTOcam+sin(angle*(PI/180))*radius*2, BLUE);
    }
};

void Creature::Draw(int camX, int camY, float zoom, Color color) {
    float posXRELTOcam = (x - camX) / zoom;
    float posYRELTOcam = (y- camY) / zoom;

    float radius = 20 / zoom;
    
    if (posXRELTOcam - radius < 1200 && posXRELTOcam + radius > 0 && posYRELTOcam - radius < 1200 && posYRELTOcam + radius > 0) {
        DrawCircle(posXRELTOcam, posYRELTOcam, radius, color);
        DrawLine(posXRELTOcam, posYRELTOcam, posXRELTOcam+cos(angle*(PI/180))*radius*2, posYRELTOcam+sin(angle*(PI/180))*radius*2, BLUE);
    }
};

void Creature::kill() {
    std::cout << "death" << std::endl;
    death = true;
}

void Creature::Birth(Game& game) {
    Creature cutie = Creature();

    cutie.localNN = this->localNN;
    cutie.x = this->x;
    cutie.y = this->y;

    this->hunger -= 50;

    cutie.color = this->color;
    cutie.LastName = this->LastName;
    cutie.generation = this->generation+1;

    for (int i = 0; i < GetRandomValue(0,2); i++) {
        cutie.localNN.Mutate();
        int r = GetRandomValue(1,3);
        if (r == 1) {
            cutie.color.r += GetRandomValue(-5,5);
        }else if (r == 2) {
            cutie.color.g += GetRandomValue(-5,5);
        }else {
            cutie.color.b += GetRandomValue(-5,5);
        }
    }

    game.pendingBirths.push_back(std::move(cutie));

    birthingCD = 60;

    std::cout << "birthed" << std::endl;
}

void Creature::tickUpdate(float dt, Game& game) {
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

    if (nearestFood != nullptr) {
        if (nearestDistance < 25.0f && nearestFood->value > 0) {// nearestDistance en nearestFood worden gezet in Network.cpp in the set inputs function
            float newPotHunger = hunger + nearestFood->value;
            float over = std::max(newPotHunger - maxHunger, 0.0f);
            hunger = newPotHunger - over;
            nearestFood->value = over;
        }
    }

    birthingCD -= dt;

    if (birthingCD <= 0 && hunger > 100) {
        Birth(game);
    }

    xAcc = cos(angle * (PI / 180)) * localNN.realOutputs[0] * 100;
    yAcc = sin(angle * (PI / 180)) * localNN.realOutputs[0] * 100;

    angularVel = localNN.realOutputs[1] * 180;

    hungriness = 2 + 6*((abs(xAcc) + abs(yAcc) + abs(angularVel))/380);
}