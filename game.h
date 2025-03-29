#ifndef GAME_H
#define GAME_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "player.h"
#include "enemy.h"
#include "wall.h"
#include "menu.h"
#include "bullet.h"

class Game {
public:
    GameMode gameMode;
    Player player;
    Player player2;

    Mix_Chunk* bombExplosionSound = nullptr;
    Mix_Music* backgroundMusic = nullptr;

    TTF_Font* font;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = true;

    Menu* menu;
    bool inMenu;
    bool isRunning() const { return running; }

    void setGameMode(GameMode mode);

    vector<Wall> walls;
    int enemyNumber = 5;
    vector<Enemy*> enemies;

    SDL_Texture* screen = nullptr;
    SDL_Texture* screen1 = nullptr;
    SDL_Texture* screen2 = nullptr;
    SDL_Texture* screen3 = nullptr;
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

    SDL_Rect menuButtonRect;
    SDL_Rect replayButtonRect;

    void playMusic();
    void stopMusic();

    bool isWall(int i, int j);
    bool init();
    SDL_Texture* loadTexture(const char* path);
    void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color);

    void spawnEnemies();
    void clearEnemies();

    void update();
    void generateWalls();
    void handleEvents();
    Game();
    void render();
    void run();
    void resetGame();
    ~Game();
};

#endif
