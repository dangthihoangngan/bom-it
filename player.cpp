#include "player.h"
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <iostream>
#include "bom.h"
using namespace std;

Player::Player () {
    x = 0;
    y = 0;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    dirX = 0;
    dirY = -1;
}

Player::Player(int startX, int startY, vector<SDL_Texture*> textures) {
    x = startX;
    y = startY;
    playerTextures = textures;
    rect = {x, y, TILE_SIZE - 5, TILE_SIZE - 5};
    dirX = 0;
    dirY = -1;
}

void Player::loadBombTextures(SDL_Renderer* renderer) {
    Bomb::loadBombTextures(renderer);
}

void Player::freeTextures() {
    Bomb::freeBombTextures();
}

void Player::placeBomb() {
    bombs.push_back(Bomb(x,y));
}

const std::vector<Bomb>& Player::getBombs() const {
    return bombs;
}

void Player::updateBombs(vector<Wall>& walls, vector<Enemy*>& enemies, bool& gameOver, bool& playerWon, Mix_Chunk* explosionSound,Player* player2) {
    if (walls.empty() && enemies.empty()) return;
    for (auto &bomb : bombs) {
        bomb.update();
    }

    for (auto it = bombs.begin(); it != bombs.end();) {
        if (SDL_GetTicks() >= it->explosionEndTime) {
            it->exploded = true;
        }

        it->explode(walls, enemies, *this, gameOver, playerWon, explosionSound, player2);

        if (it->exploded && SDL_GetTicks() >= it->explosionEndTime) {
            it = bombs.erase(it);
        } else {
            ++it;
        }
    }
}

void Player::move(int dx, int dy, const vector<Wall>& walls) {
    int newX = x + dx;
    int newY = y + dy;
    this->dirX = dx;
    this->dirY = dy;

    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE};
    for (int i = 0; i < (int) walls.size(); i++) {
        if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)) {
            return;
        }
    }

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }
}

void Player::update() {
    if (state == STANDING) {
        frame = 0;
    } else if (state == MOVING) {
        frame = (SDL_GetTicks() / 150) % 3;
    }
}

void Player::render(SDL_Renderer* renderer) {
    int index = direction * 3 + frame;
    if (index >= 0 && index < (int) playerTextures.size()) {
        SDL_Rect destRect = {x, y, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy(renderer, playerTextures[index], nullptr, &destRect);
    }
    for (auto& bomb : bombs) {
        bomb.render(renderer);
    }
}
