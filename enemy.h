#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include "bom.h"
#include "wall.h"

enum PlayerState { STANDING, PREPARING, MOVING };
enum Direction { DOWN, LEFT, RIGHT, UP };

class Player;

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

    Enemy (int startX, int startY);
    Enemy(int startX, int startY, vector<SDL_Texture*> textures);
    void move(const vector<Wall>& walls);
    void render(SDL_Renderer* renderer);
};

#endif
