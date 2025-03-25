#include "enemy.h"

Enemy::Enemy(int startX, int startY) {
    moveDelay = 15;
    bombDelay = 5;
    x = startX;
    y = startY;
    rect = { x, y, TILE_SIZE, TILE_SIZE };
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

void Enemy::move(const vector<Wall>& walls) {
    if (--moveDelay > 0) return;
    moveDelay = 15;

    int r = rand() % 50;
    if (r <= 5) {
        dirX = 0;
        dirY = -10;
        direction = UP;
    } else if (r <= 25) {
        dirX = 0;
        dirY = 10;
        direction = DOWN;
    } else if (r <= 37) {
        dirX = -10;
        dirY = 0;
        direction = LEFT;
    } else {
        dirX = 10;
        dirY = 0;
        direction = RIGHT;
    }
    state = MOVING;

    int newX = x + dirX;
    int newY = y + dirY;
    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };

    for (const auto& wall : walls) {
        if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) {
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
        if (index >= 0 && index < (int) enemyTextures.size()) {
            SDL_Rect destRect = { x, y, TILE_SIZE, TILE_SIZE };
            SDL_RenderCopy(renderer, enemyTextures[index], nullptr, &destRect);
            return;
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void ShootingEnemy::shoot() {
    if (--bombDelay > 0) return;
    bombDelay = 5;
    int bulletSpeed = 5;
    bullets.push_back(Bullet(x, y, dirX * bulletSpeed, dirY * bulletSpeed, bulletTexture));
}

void ShootingEnemy::updateBullets(const std::vector<Wall>& walls) {
    for (auto &bullet : bullets) {
        bullet.move(walls);
    }
    bullets.erase(remove_if(bullets.begin(), bullets.end(),
                            [](Bullet &b) { return !b.active; }),
                  bullets.end());
}

ShootingEnemy::ShootingEnemy(int startX, int startY, vector<SDL_Texture*> textures, SDL_Texture* bulletTex)
    : Enemy(startX, startY, textures), bulletTexture(bulletTex) {
    enemyTextures = textures;
}

void ShootingEnemy::render(SDL_Renderer* renderer) {
    if (!enemyTextures.empty()) {
        int index = direction * 3 + frame;
        SDL_Rect destRect = { x, y, TILE_SIZE, TILE_SIZE };
        SDL_RenderCopy(renderer, enemyTextures[index], nullptr, &destRect);
    }
    cout<<bullets.size();

    for (auto& bullet : bullets) {
        bullet.render(renderer);
    }
}

WalkingEnemy::WalkingEnemy(int startX, int startY, vector<SDL_Texture*> textures)
    : Enemy(startX, startY, textures) {
    enemyTextures = textures;
}

void WalkingEnemy::render(SDL_Renderer* renderer) {
    if (!enemyTextures.empty()) {
        int index = direction * 3 + frame;
        SDL_Rect destRect = { x, y, TILE_SIZE, TILE_SIZE };
        SDL_RenderCopy(renderer, enemyTextures[index], nullptr, &destRect);
    }
}
