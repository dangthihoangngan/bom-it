#include "game.h"
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL_image.h>


using namespace std;

bool Game::isWall(int i, int j) {
    for (const auto& wall : walls) {
        if (wall.x == j * TILE_SIZE && wall.y == i * TILE_SIZE) {
            return true;
        }
    }
    return false;
}

SDL_Texture* Game::loadTexture(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}


void Game::spawnEnemies() {
    enemies.clear();
    for (int i=0; i< enemyNumber; ++i) {
        int ex, ey;
        bool validPosition = false;
        while (!validPosition) {
            ex = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
            ey = (rand() % (MAP_HEIGHT - 2) + 1) * TILE_SIZE;
            validPosition = true;
            for (const auto& wall : walls) {
                if (wall.active && wall.x == ex && wall.y == ey) {
                    validPosition = false;
                    break;
                }
            }
        }
        enemies.push_back (Enemy(ex, ey));
    }
}

void Game::update() {
    player.updateBombs();

    vector<vector<Wall>::iterator> wallsToRemove;
    vector<vector<Enemy>::iterator> enemiesToRemove;

    for (auto it = player.bombs.begin(); it != player.bombs.end();) {
        if (it->exploded) {
            SDL_Rect explosionRects[5] = {
                {it->x, it->y, TILE_SIZE, TILE_SIZE},
                {it->x - TILE_SIZE, it->y, TILE_SIZE, TILE_SIZE},
                {it->x + TILE_SIZE, it->y, TILE_SIZE, TILE_SIZE},
                {it->x, it->y - TILE_SIZE, TILE_SIZE, TILE_SIZE},
                {it->x, it->y + TILE_SIZE, TILE_SIZE, TILE_SIZE}
            };
            auto wallIt = walls.begin();
            while (wallIt != walls.end()) {
                bool destroyed = false;
                for (int i = 0; i < 5; i++) {
                    if (wallIt->active && SDL_HasIntersection(&explosionRects[i], &wallIt->rect)) {
                        destroyed = true;
                        break;
                    }
                }
                if (destroyed) {
                    wallIt = walls.erase(wallIt);
                } else {
                    ++wallIt;
                }
            }
            auto enemyIt = enemies.begin();
            while (enemyIt != enemies.end()) {
                bool killed = false;
                for (int i = 0; i < 5; i++) {
                    if (SDL_HasIntersection(&explosionRects[i], &enemyIt->rect)) {
                        killed = true;
                        break;
                    }
                }
                if (killed) {
                    enemyIt = enemies.erase(enemyIt);
                } else {
                    ++enemyIt;
                }
            }
            if (SDL_GetTicks() >= it->explosionEndTime) {
                it = player.bombs.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }

    for (auto& enemy : enemies) {
        enemy.move(walls);
        if (SDL_HasIntersection(&player.rect, &enemy.rect)) {
            running = false;
            return;
        }
    }

    for (auto& bomb : player.bombs) {
        if (!bomb.exploded) continue;

        SDL_Rect explosionRects[5] = {
            {bomb.x, bomb.y, TILE_SIZE, TILE_SIZE},
            {bomb.x - TILE_SIZE, bomb.y, TILE_SIZE, TILE_SIZE},
            {bomb.x + TILE_SIZE, bomb.y, TILE_SIZE, TILE_SIZE},
            {bomb.x, bomb.y - TILE_SIZE, TILE_SIZE, TILE_SIZE},
            {bomb.x, bomb.y + TILE_SIZE, TILE_SIZE, TILE_SIZE}
        };

        for (int i = 0; i < 5; ++i) {
            if (SDL_HasIntersection(&player.rect, &explosionRects[i])) {
                running = false;
                return;
            }
        }
    }

    if (enemies.empty()) {
        running = false;
    }
}

void Game::generateWalls() {
    walls.clear();
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            int tileType = mapData[i][j];

            if (tileType != 0) {
                SDL_Texture* selectedTexture = nullptr;
                if (tileType == 1) {
                    selectedTexture = wallTexture1;
                } else if (tileType == 2) {
                    selectedTexture = wallTexture2;
                } else {
                    selectedTexture = wallTexture3;
                }

                walls.push_back(Wall(j * TILE_SIZE, i * TILE_SIZE, tileType, selectedTexture));
            }
        }
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                     player.move(0, -5, walls);
                     player.direction = UP;
                     player.state = MOVING;
                     break;
                case SDLK_DOWN:
                     player.move(0, 5, walls);
                     player.direction = DOWN;
                     player.state = MOVING;
                     break;
                case SDLK_LEFT:
                     player.move(-5, 0, walls);
                     player.direction = LEFT;
                     player.state = MOVING;
                     break;
                case SDLK_RIGHT:
                     player.move(5, 0, walls);
                     player.direction = RIGHT;
                     player.state = MOVING;
                     break;
                case SDLK_SPACE: player.placeBomb(); break;
            }
        }else if (event.type == SDL_KEYUP) {
            player.state = STANDING;
        }
    }
}

Game::Game(){
    running=true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }
    window = SDL_CreateWindow("BOM IT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(!window){
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() <<endl;
        running = false;
    }

    Player::loadBombTextures(renderer);

    wallTexture1 = loadTexture("assets/wall1.png");
    wallTexture2 = loadTexture("assets/wall2.png");
    wallTexture3 = loadTexture("assets/wall3.png");

    for (int i = 0; i < 12; i++) {
        string path = "assets/character" + to_string(i + 1) + ".png";
        SDL_Texture* texture = loadTexture(path.c_str());
        if (!texture) {
            cerr << "Failed to load " << path << " SDL_Error: " << SDL_GetError() << endl;
            running = false;
        } else {
            playerTextures.push_back(texture);
        }
    }

    if (!wallTexture1 || !wallTexture2 || !wallTexture3) {
        cerr << "Could not load wall textures! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }

    if (playerTextures.empty()) {
        cerr << "No player textures loaded! Exiting..." << endl;
        running = false;
    }

    generateWalls();
    player = Player(((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE,playerTextures);
    spawnEnemies();
}

void Game::render () {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 225);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_Texture* selectedTexture = nullptr;
            for (const auto& wall : walls) {
                if (wall.x == j * TILE_SIZE && wall.y == i * TILE_SIZE) {
                    if (wall.type == 1) {
                        selectedTexture = wallTexture1;
                    } else if (wall.type == 2) {
                        selectedTexture = wallTexture2;
                    } else if (wall.type == 3) {
                        selectedTexture = wallTexture3;
                    }
                    break;
                }
            }
            if (selectedTexture) {
                SDL_RenderCopy(renderer, selectedTexture, NULL, &tile);
            }
        }
    }
    player.update();

    player.render(renderer);

    for (auto &enemy : enemies) {
        enemy.render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::run () {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
    }
}
Game::~Game() {
    Player::freeTextures();
    SDL_DestroyTexture(wallTexture1);
    SDL_DestroyTexture(wallTexture2);
    SDL_DestroyTexture(wallTexture3);
    for (SDL_Texture* texture : playerTextures) {
        SDL_DestroyTexture(texture);
    }
    playerTextures.clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
