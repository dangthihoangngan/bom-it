#ifndef BOMB_H
#define BOMB_H

#include <algorithm>
#include <SDL2/SDL.h>
#include <iostream>
#include "constants.h"

class Bomb {
public:
    int x, y;
    SDL_Rect rect;
    Uint32 placedTime;
    Uint32 explosionEndTime;
    volatile bool exploded;

    Bomb(int startX, int startY);
    void update();
    void render(SDL_Renderer* renderer);
};

#endif
