#include "bom.h"

Bomb::Bomb(int startX, int startY) {
    x = startX;
    y = startY;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    placedTime = SDL_GetTicks();
    explosionEndTime = placedTime + EXPLOSION_TIME + EXPLOSION_DURATION;
    exploded = false;
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
    if (!exploded) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    } else {
        if (exploded && SDL_GetTicks() < explosionEndTime) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_Rect explosionRects[5] = {
                {x, y, TILE_SIZE, TILE_SIZE},
                {x - TILE_SIZE, y, TILE_SIZE, TILE_SIZE},
                {x + TILE_SIZE, y, TILE_SIZE, TILE_SIZE},
                {x, y - TILE_SIZE, TILE_SIZE, TILE_SIZE},
                {x, y + TILE_SIZE, TILE_SIZE, TILE_SIZE}
            };
            for (int i = 0; i < 5; i++) {
                SDL_RenderFillRect(renderer, &explosionRects[i]);
            }
        }
    }
}
