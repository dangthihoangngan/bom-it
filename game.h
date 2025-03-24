#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "player.h"
#include "enemy.h"
#include "wall.h"
#include "menu.h"

enum GameMode { SINGLE_PLAYER, TWO_PLAYER };

class Game{
public:

    GameMode gameMode;
    Player player2;

    Mix_Chunk* bombExplosionSound = nullptr;
    Mix_Music* backgroundMusic;

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    Menu* menu;
    bool inMenu;
    bool isRunning() const { return running; }

    void setGameMode(GameMode mode);

    vector <Wall> walls;
    Player player;
    int enemyNumber = 5;
    vector <Enemy> enemies;

    SDL_Texture* winScreen;
    SDL_Texture* loseScreen;
    bool gameOver = false;
    bool playerWon = false;

    SDL_Texture* groundTexture;
    vector<SDL_Texture*> wallTextures;
    vector<SDL_Texture*> playerTextures;
    vector<SDL_Texture*> enemyTextures;

    void playMusic();
    void stopMusic();

    bool isWall(int i, int j);
    bool init();
    SDL_Texture* loadTexture(const char* path);
    void spawnEnemies(const vector<SDL_Texture*>& enemyTextures);
    void update();
    void generateWalls();
    void handleEvents();
    Game();
    void render ();
    void run ();
    ~Game();
};


#endif
