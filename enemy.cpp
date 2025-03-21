#include "enemy.h"

Enemy::Enemy (int startX, int startY) {
    moveDelay = 15;
    bombDelay = 5;
    x = startX;
    y = startY;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    dirX = 0;
    dirY = 1;
    lastBombTime = SDL_GetTicks();
}

void Enemy::move (const vector<Wall>& walls) {
    if (--moveDelay > 0) return;
    moveDelay = 15;
    int r = rand() % 50;
    if (r <= 5) {
        this->dirX = 0;
        this->dirY = -20;
    }
    else if (r >= 6 && r <= 35 ) {
        this->dirX = 0;
        this->dirY = 20;
    }
    else if (r >= 36 && r <= 45) {
        this->dirY = 0;
        this->dirX = -20;
    }
    else if (r >= 46) {
        this->dirY = 0;
        this->dirX = 20;
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

void Enemy::placeBomb() {
    Uint32 currentTime = SDL_GetTicks();
    if (--bombDelay > 0) return;
    bombDelay = 5;
    if (currentTime - lastBombTime >= 5000) {
        bombs.push_back(Bomb(x,y));
        lastBombTime = currentTime;
        lastBombX = x;
        lastBombY = y;
    }
}

void Enemy::updateBombs() {
    for (auto &bomb : bombs) {
        bomb.update();
    }
    bombs.erase(std::remove_if(bombs.begin(), bombs.end(), [](Bomb &b) {
    return b.exploded && SDL_GetTicks() >= b.explosionEndTime;}), bombs.end());
}

void Enemy::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    for (auto &bomb : bombs) {
        bomb.render(renderer);
    }
}
