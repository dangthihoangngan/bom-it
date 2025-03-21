#include "enemy.h"

Enemy::Enemy (int startX, int startY) {
    moveDelay = 15;
    bombDelay = 5;
    x = startX;
    y = startY;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    dirX = 0;
    dirY = 1;
}

void Enemy::move (const vector<Wall>& walls) {
    if (--moveDelay > 0) return;
    moveDelay = 15;
    int r = rand() % 50;
    if (r <= 5) {
        this->dirX = 0;
        this->dirY = -5;
    }
    else if (r >= 6 && r <= 35 ) {
        this->dirX = 0;
        this->dirY = 5;
    }
    else if (r >= 36 && r <= 44) {
        this->dirY = 0;
        this->dirX = -5;
    }
    else if (r >= 45) {
        this->dirY = 0;
        this->dirX = 5;
    }
    int newX = x + this->dirX;
    int newY = y + this->dirY;
    SDL_Rect newRect = {newX, newY, TILE_SIZE, TILE_SIZE};
    for (const auto& wall : walls) {
        if (wall.active && SDL_HasIntersection (&newRect, &wall.rect)) {
            return;
        }
    }

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }
}

void Enemy::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}
