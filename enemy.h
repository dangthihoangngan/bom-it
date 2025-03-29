#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include "bom.h"
#include "wall.h"
#include "bullet.h"
#include "constants.h"

class Player;
class Wall;
class Bomb;

using namespace std;

class Enemy {
public:
    int x, y;
    int dirX, dirY;
    int moveDelay, bombDelay;
    SDL_Rect rect;

    vector<SDL_Texture*> enemyTextures;
    int frame = 0;
    PlayerState state = STANDING;
    Direction direction = DOWN;

    Enemy(int startX, int startY);
    Enemy(int startX, int startY, vector<SDL_Texture*> textures);
    virtual void move(const vector<Wall>& walls,const std::vector<Bomb>& bombs);
    virtual void render(SDL_Renderer* renderer);
    virtual ~Enemy() {}
};

class WalkingEnemy : public Enemy {
public:
    WalkingEnemy(int startX, int startY, vector<SDL_Texture*> textures);
    void render(SDL_Renderer* renderer) override;
};

class ShootingEnemy : public Enemy {
public:
    vector<Bullet> bullets;
    SDL_Texture* bulletTexture;

    ShootingEnemy(int startX, int startY, vector<SDL_Texture*> textures, SDL_Texture* bulletTex);
    void shoot();
    void updateBullets(const std::vector<Wall>& walls);
    void render(SDL_Renderer* renderer) override;
};

#endif
