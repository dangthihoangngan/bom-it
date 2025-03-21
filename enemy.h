#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include "bom.h"
#include "wall.h"

using namespace std;

class Enemy {
public:
    int x, y;
    int dirX, dirY;
    int moveDelay, bombDelay;
    SDL_Rect rect;

    Enemy (int startX, int startY);
    void move(const vector<Wall>& walls);
    void render(SDL_Renderer* renderer);
};

#endif
