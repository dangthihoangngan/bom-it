#include "bom.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include "enemy.h"

using namespace std;

SDL_Texture* Bomb::bombTexture = nullptr;
SDL_Texture* Bomb::explosionTextures[4] = {nullptr, nullptr, nullptr, nullptr};

Bomb::Bomb(int startX, int startY) {
    x = (startX / TILE_SIZE) * TILE_SIZE;

    // Kiểm tra nếu startY không nằm chính xác trên một ô lưới
    if (startY % TILE_SIZE != 0) {
        y = (startY / TILE_SIZE) * TILE_SIZE + TILE_SIZE; // Đặt xuống ô dưới
    } else {
        y = (startY / TILE_SIZE) * TILE_SIZE; // Giữ nguyên nếu đã khớp ô
    }
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    placedTime = SDL_GetTicks();
    explosionEndTime = placedTime + EXPLOSION_TIME + EXPLOSION_DURATION;
    exploded = false;
}

void Bomb::loadBombTextures(SDL_Renderer* renderer) {
    bombTexture = IMG_LoadTexture(renderer, "assets/explosion1.png");
    explosionTextures[0] = IMG_LoadTexture(renderer, "assets/explosion1.png");
    explosionTextures[1] = IMG_LoadTexture(renderer, "assets/explosion2.png");
    explosionTextures[2] = IMG_LoadTexture(renderer, "assets/explosion3.png");
    explosionTextures[3] = IMG_LoadTexture(renderer, "assets/explosion4.png");

    if (!bombTexture) cerr << "Failed to load bomb texture! SDL_Error: " << IMG_GetError() << endl;
    for (int i = 0; i < 4; i++) {
        if (!explosionTextures[i]) cerr << "Failed to load explosion texture " << i << "! SDL_Error: " << IMG_GetError() << endl;
    }
}

void Bomb::freeBombTextures() {
    if (bombTexture) {
        SDL_DestroyTexture(bombTexture);
        bombTexture = nullptr;
    }
    for (int i = 0; i < 4; i++) {
        if (explosionTextures[i]) {
            SDL_DestroyTexture(explosionTextures[i]);
            explosionTextures[i] = nullptr;
        }
    }
}

void Bomb::update() {
    Uint32 currentTime = SDL_GetTicks();
    Uint32 timeSincePlaced = currentTime - placedTime;
    if (!exploded && (timeSincePlaced >= EXPLOSION_TIME)) {
        exploded = true;
        explosionEndTime = currentTime + EXPLOSION_DURATION;
    }
}

void Bomb::explode(std::vector<Wall>& walls, std::vector<Enemy*>& enemies,
                   Player& player, bool& gameOver, bool& playerWon, Mix_Chunk* explosionSound, Player* player2) {
    if (!exploded){
        return;
    }

    if (explosionSound) {
        Mix_PlayChannel(-1, explosionSound, 0);
    }

    SDL_Rect explosionRects[5] = {
        {x, y, TILE_SIZE, TILE_SIZE},
        {x - TILE_SIZE, y, TILE_SIZE, TILE_SIZE},
        {x + TILE_SIZE, y, TILE_SIZE, TILE_SIZE},
        {x, y - TILE_SIZE, TILE_SIZE, TILE_SIZE},
        {x, y + TILE_SIZE, TILE_SIZE, TILE_SIZE}
    };

    for (auto it = walls.begin(); it != walls.end();) {
        bool destroyed = false;
        for (int i = 0; i < 5; i++) {
            if (it->active && SDL_HasIntersection(&explosionRects[i], &it->rect)) {
                destroyed = true;
                break;
            }
        }
        if (destroyed) {
            it = walls.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = enemies.begin(); it != enemies.end();) {
        bool killed = false;
        for (int i = 0; i < 5; i++) {
            if (SDL_HasIntersection(&explosionRects[i], &(*it)->rect)) {
                killed = true;
                player.killCount++;
                break;
            }
        }
        if (killed) {
            delete *it;
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }

    bool player1Hit = false;
    bool player2Hit = false;

    for (int i = 0; i < 5; i++) {
        if (SDL_HasIntersection(&player.rect, &explosionRects[i])) {
            player.alive = false;
            player1Hit = true;
        }
        if (player2 && SDL_HasIntersection(&player2->rect, &explosionRects[i])) {
            player2->alive = false;
            player2Hit = true;
        }
    }

    if (player1Hit && (!player2 || player2Hit)) {
        gameOver = true;
        playerWon = false;
    }
}


void Bomb::render(SDL_Renderer* renderer) {
    Uint32 currentTime = SDL_GetTicks();
    Uint32 timeSincePlaced = currentTime - placedTime;

    if (timeSincePlaced >= EXPLOSION_TIME) {
        exploded = true;
    }

    if (exploded) {
        SDL_Rect explosionRects[5] = {
            {x, y, TILE_SIZE, TILE_SIZE},
            {x - TILE_SIZE, y, TILE_SIZE, TILE_SIZE},
            {x + TILE_SIZE, y, TILE_SIZE, TILE_SIZE},
            {x, y - TILE_SIZE, TILE_SIZE, TILE_SIZE},
            {x, y + TILE_SIZE, TILE_SIZE, TILE_SIZE}
        };

        for (int i = 0; i < 5; i++) {
            SDL_RenderCopy(renderer, explosionTextures[3], nullptr, &explosionRects[i]);
        }
        return;
    }
    SDL_Texture* textureToRender = bombTexture;
    if (timeSincePlaced >= 2900) {
        textureToRender = explosionTextures[2];
    } else if (timeSincePlaced >= 2000) {
        textureToRender = explosionTextures[1];
    }

    SDL_RenderCopy(renderer, textureToRender, nullptr, &rect);
}
