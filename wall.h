#ifndef WALL_H
#define WALL_H

#include <vector>
#include <SDL2/SDL.h>
#include "constants.h"

class Wall {
public:
    int x, y;
    SDL_Rect rect;
    bool active;

    Wall (int startX, int startY);
    void render(SDL_Renderer* renderer);
};

#endif
