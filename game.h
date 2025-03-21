#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL2/SDL.h>
#include "player.h"
#include "enemy.h"
#include "wall.h"

class Game{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    vector <Wall> walls;
    Player player;
    int enemyNumber = 5;
    vector <Enemy> enemies;

    void spawnEnemies();
    void update();
    void generateWalls();
    void handleEvents();
    Game();
    void render ();
    void run ();
    ~Game();
};


#endif
