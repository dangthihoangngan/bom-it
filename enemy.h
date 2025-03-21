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
    int lastBombTime;
    int lastBombX, lastBombY;
    SDL_Rect rect;
    vector <Bomb> bombs;

    Enemy (int startX, int startY);
    void placeBomb();
    void updateBombs();
    void move(const vector<Wall>& walls);
    void render(SDL_Renderer* renderer);
};

#endif
