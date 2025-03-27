#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include "constants.h"
#include "wall.h"
#include <SDL2/SDL_mixer.h>
#include "enemy.h"
#include <algorithm>

class Enemy;
class Bomb;

using namespace std;

class Player {
public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;
    int frame = 0;
    int killCount = 0;
    PlayerState state = STANDING;
    Direction direction = DOWN;
    bool alive = true;
    vector<SDL_Texture*> playerTextures;
    vector <Bomb> bombs;

    Player ();
    Player(int startX, int startY, vector<SDL_Texture*> textures);

    static void loadBombTextures(SDL_Renderer* renderer);
    static void freeTextures();

    const std::vector<Bomb>& getBombs() const;
    void placeBomb();
    void updateBombs(vector<Wall>& walls, vector<Enemy*>& enemies,
                             bool& gameOver, bool& playerWon, Mix_Chunk* explosionSound,Player* player2 = nullptr);
    void move(int dx, int dy, const vector<Wall>& walls);

    void update();
    void render(SDL_Renderer* renderer);
};

#endif
