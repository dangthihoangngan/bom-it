#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>
#include <vector>
#include "constants.h"
#include "wall.h"

class Bullet {
public:
    int x, y;
    int dx, dy;
    bool active;
    SDL_Rect rect;
    SDL_Texture* texture;

    Bullet(int startX, int startY, int dirX, int dirY, SDL_Texture* bulletTexture);
    void move(const std::vector<Wall>& walls);
    void render(SDL_Renderer* renderer);
};

#endif
