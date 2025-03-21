#include "bom.h"
#include <SDL2/SDL_image.h>
#include <iostream>

using namespace std;

SDL_Texture* Bomb::bombTexture = nullptr;
SDL_Texture* Bomb::explosionTextures[4] = {nullptr, nullptr, nullptr, nullptr};

Bomb::Bomb(int startX, int startY) {
    x = startX;
    y = startY;
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
