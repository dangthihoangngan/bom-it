#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "bom.h"
#include "wall.h"
enum PlayerState { STANDING, PREPARING, MOVING };
enum Direction { DOWN, LEFT, RIGHT, UP };

using namespace std;

class Player {
public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;
    int frame = 0;
    PlayerState state = STANDING;
    Direction direction = DOWN;
    vector<SDL_Texture*> playerTextures;
    vector <Bomb> bombs;

    Player ();
    Player(int startX, int startY, vector<SDL_Texture*> textures);

    static void loadBombTextures(SDL_Renderer* renderer);
    static void freeTextures();

    void placeBomb();
    void updateBombs();
    void move(int dx, int dy, const vector<Wall>& walls);

    void update();
    void render(SDL_Renderer* renderer);
};

#endif
