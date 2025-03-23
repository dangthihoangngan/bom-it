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

Enemy::Enemy(int startX, int startY, vector<SDL_Texture*> textures) {
    moveDelay = 15;
    bombDelay = 5;
    x = startX;
    y = startY;
    rect = { x, y, TILE_SIZE, TILE_SIZE };
    dirX = 0;
    dirY = 1;
    enemyTextures = textures;
    frame = 0;
    direction = DOWN;
}

void Enemy::move (const vector<Wall>& walls) {
    if (--moveDelay > 0) return;
    moveDelay = 15;
    int r = rand() % 50;
    if (r <= 5) {
        this->dirX = 0;
        this->dirY = -5;
        direction = UP;
        state = MOVING;
    }
    else if (r >= 6 && r <= 25 ) {
        this->dirX = 0;
        this->dirY = 5;
        direction = DOWN;
        state = MOVING;
    }
    else if (r >= 26 && r <= 37) {
        this->dirY = 0;
        this->dirX = -5;
        direction = LEFT;
        state = MOVING;
    }
    else if (r >= 38) {
        this->dirY = 0;
        this->dirX = 5;
        direction = RIGHT;
        state = MOVING;
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

    frame = (SDL_GetTicks() / 150) % 3;
}

void Enemy::render(SDL_Renderer* renderer) {
    if (!enemyTextures.empty()) {
        int index = direction * 3 + frame;
        if (index >= 0 && index < enemyTextures.size()) {
            SDL_Rect destRect = { x, y, TILE_SIZE, TILE_SIZE };
            SDL_RenderCopy(renderer, enemyTextures[index], nullptr, &destRect);
            return;
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}
