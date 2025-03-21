#include "player.h"

Player::Player () {
    x = 0;
    y = 0;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    dirX = 0;
    dirY = -1;
}

Player::Player (int startX, int startY) {
    x = startX;
    y = startY;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    dirX = 0;
    dirY = -1;
}

void Player::placeBomb() {
    bombs.push_back(Bomb(x,y));
}

void Player::updateBombs() {
    for (auto &bomb : bombs) {
        bomb.update();
    }
    bombs.erase(std::remove_if(bombs.begin(), bombs.end(), [](Bomb &b) {
    return b.exploded && SDL_GetTicks() >= b.explosionEndTime;}), bombs.end());
}

void Player::move(int dx, int dy, const vector<Wall>& walls) {
    int newX = x + dx;
    int newY = y + dy;
    this->dirX = dx;
    this->dirY = dy;

    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE};
    for (int i = 0; i < walls.size(); i++) {
        if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)) {
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

void Player::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    for (auto &bomb : bombs) {
        bomb.render(renderer);
    }
}


