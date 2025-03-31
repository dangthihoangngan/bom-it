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
    // Biến thành viên chính
    GameMode gameMode;
    Player player;
    Player player2;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = true;
    bool isRunning() const { return running; }

    // Âm thanh và âm nhạc
    Mix_Chunk* bombExplosionSound = nullptr;
    Mix_Music* backgroundMusic = nullptr;

    // Văn bản và Font
    TTF_Font* font;

    // Texture
    SDL_Texture* groundTexture = nullptr;
    std::vector<SDL_Texture*> wallTextures;
    std::vector<SDL_Texture*> playerTextures;
    std::vector<SDL_Texture*> player2Textures;
    std::vector<SDL_Texture*> walkingEnemyTextures;
    std::vector<SDL_Texture*> shootingEnemyTextures;
    SDL_Texture* bulletTexture = nullptr;

    // Màn hình và Menu
    SDL_Texture* screen = nullptr;
    SDL_Texture* screen1 = nullptr;
    SDL_Texture* screen2 = nullptr;
    SDL_Texture* screen3 = nullptr;
    SDL_Texture* winScreen = nullptr;
    SDL_Texture* loseScreen = nullptr;
    Menu* menu;
    bool inMenu;

    // Trạng thái game
    bool gameOver = false;
    bool playerWon = false;
    bool isPaused = false;

    // Tường và kẻ địch
    std::vector<Wall> walls;
    int enemyNumber = 5;
    std::vector<Enemy*> enemies;

    // Các nút
    SDL_Rect menuButtonRect;
    SDL_Rect replayButtonRect;

    // Điều khiển
    SDL_Scancode playerKey = SDL_SCANCODE_UNKNOWN;
    SDL_Scancode player2Key = SDL_SCANCODE_UNKNOWN;

    // Constructor & Destructor
    Game();
    ~Game();

    // Hàm chính
    void run();
    void update();
    void render();
    void handleEvents();
    void resetGame();

    // Các chức năng bổ trợ
    void setGameMode(GameMode mode);
    void playMusic();
    void stopMusic();
    bool isWall(int i, int j);
    void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color);
    SDL_Texture* loadTexture(const char* path);

    // Quản lý kẻ địch
    void spawnEnemies();

    // Quản lý bản đồ
    void generateWalls();
};

#endif

