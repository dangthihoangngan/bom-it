#ifndef BOMB_H
#define BOMB_H

#include <algorithm>
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "constants.h"
#include "wall.h"
#include <SDL2/SDL_mixer.h>

class Enemy;

class Bomb {
public:
    int x, y;
    SDL_Rect rect;
    Uint32 placedTime;
    Uint32 explosionEndTime;
    volatile bool exploded;

    static SDL_Texture* bombTexture;
    static SDL_Texture* explosionTextures[4];

    Bomb(int startX, int startY);
    static void loadBombTextures(SDL_Renderer* renderer);
    static void freeBombTextures();
    void update();
    void explode(std::vector<Wall>& walls, std::vector<Enemy*>& enemies,
                 SDL_Rect playerRect, bool& gameOver, bool& playerWon, Mix_Chunk* explosionSound);
    void render(SDL_Renderer* renderer);
};

#endif
