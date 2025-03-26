#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "player.h"
#include "enemy.h"
#include "wall.h"
#include "menu.h"
#include "bullet.h"

class Game {
public:
    GameMode gameMode;
    Player player2;

    Mix_Chunk* bombExplosionSound = nullptr;
    Mix_Music* backgroundMusic = nullptr;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = true;

    Menu* menu;
    bool inMenu;
    bool isRunning() const { return running; }

    void setGameMode(GameMode mode);

    vector<Wall> walls;
    Player player;
    int enemyNumber = 5;
    vector<Enemy*> enemies;

    SDL_Texture* winScreen = nullptr;
    SDL_Texture* loseScreen = nullptr;
    bool gameOver = false;
    bool playerWon = false;

    SDL_Texture* groundTexture = nullptr;
    vector<SDL_Texture*> wallTextures;
    vector<SDL_Texture*> playerTextures;
    vector<SDL_Texture*> player2Textures;

    vector<SDL_Texture*> walkingEnemyTextures;
    vector<SDL_Texture*> shootingEnemyTextures;

    SDL_Texture* bulletTexture = nullptr;

    void playMusic();
    void stopMusic();

    bool isWall(int i, int j);
    bool init();
    SDL_Texture* loadTexture(const char* path);

    void spawnEnemies();
    void clearEnemies();

    void update();
    void generateWalls();
    void handleEvents();
    Game();
    void render();
    void run();
    ~Game();
};

#endif
