#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "bom.h"
#include "wall.h"

using namespace std;

class Player {
public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;
    vector <Bomb> bombs;

    Player ();
    Player (int startX, int startY);
    void placeBomb();
    void updateBombs();
    void move(int dx, int dy, const vector<Wall>& walls);
    void render(SDL_Renderer* renderer);
};

#endif
