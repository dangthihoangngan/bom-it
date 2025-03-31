#ifndef BOMB_H
#define BOMB_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include "constants.h"
#include "wall.h"
#include "player.h"

class Enemy;
class Player;

class Bomb {
public:
    // Vị trí và kích thước
    int x, y;
    SDL_Rect rect;

    // Thời gian
    Uint32 placedTime;
    Uint32 explosionEndTime;
    volatile bool exploded;

    // Texture
    static SDL_Texture* bombTexture;
    static SDL_Texture* explosionTextures[4];

    // Constructor & Destructor
    Bomb(int startX, int startY);

    // Quản lý tài nguyên
    static void loadBombTextures(SDL_Renderer* renderer);
    static void freeBombTextures();

    // Cập nhật trạng thái
    void update();
    void explode(std::vector<Wall>& walls, std::vector<Enemy*>& enemies,
                 Player& player, bool& gameOver, bool& playerWon,
                 Mix_Chunk* explosionSound, Player* player2 = nullptr);

    // Render
    void render(SDL_Renderer* renderer);
};

#endif

