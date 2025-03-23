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


void Game::spawnEnemies(const vector<SDL_Texture*>& enemyTextures) {
    enemies.clear();
    for (int i = 0; i < enemyNumber; ++i) {
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
        enemies.push_back(Enemy(ex, ey, enemyTextures));
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
            gameOver = true;
            playerWon = false;
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
                gameOver = true;
                playerWon = false;
                return;
            }
        }
    }

    if (enemies.empty()) {
        gameOver = true;
        playerWon = true;
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
                    selectedTexture = wallTextures[0];
                } else if (tileType == 2) {
                    selectedTexture = wallTextures[1];
                } else {
                    selectedTexture = wallTextures[2];
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
        } else if (inMenu) {
            if (menu->handleEvent(event)) {
                inMenu = false;
            }
            continue;
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
    inMenu = true;
    menu = new Menu(renderer);

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
    winScreen = loadTexture("assets/win.png");
    loseScreen = loadTexture("assets/lose.png");
    groundTexture = loadTexture("assets/ground.png");

    if (!winScreen) {
            cerr << "Failed to load " << winScreen << " SDL_Error: " << SDL_GetError() << endl;
            running = false;
    }

    if (!loseScreen) {
            cerr << "Failed to load " << loseScreen << " SDL_Error: " << SDL_GetError() << endl;
            running = false;
    }

    if (!groundTexture) {
            cerr << "Failed to load " << groundTexture << " SDL_Error: " << SDL_GetError() << endl;
            running = false;
    }

    for (int i = 0; i < 3; i++) {
        string path = "assets/wall" + to_string(i + 1) + ".png";
        SDL_Texture* texture = loadTexture(path.c_str());
        if (!texture) {
            cerr << "Failed to load " << path << " SDL_Error: " << SDL_GetError() << endl;
            running = false;
        } else {
            wallTextures.push_back(texture);
        }
    }

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

    vector<SDL_Texture*> enemyTextures;
    for (int i = 0; i < 12; i++) {
        string path = "assets/enemy" + to_string(i + 1) + ".png";
        SDL_Texture* texture = loadTexture(path.c_str());
        if (!texture) {
            cerr << "Failed to load " << path << " SDL_Error: " << SDL_GetError() << endl;
            running = false;
        } else {
            enemyTextures.push_back(texture);
        }
    }

    if (enemyTextures.empty() || wallTextures.empty() || playerTextures.empty()) {
        cerr << "No enemy textures loaded! Exiting..." << endl;
        running = false;
    }

    generateWalls();
    player = Player(((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE,playerTextures);
    spawnEnemies(enemyTextures);
    menu = new Menu(renderer);
}

void Game::render () {
    if (inMenu) {
        menu->render();
    } else if (gameOver) {
        if (playerWon) {
            SDL_RenderCopy(renderer, winScreen, NULL, NULL);
        } else {
            SDL_RenderCopy(renderer, loseScreen, NULL, NULL);
        }
    } else {
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 225);
        SDL_RenderClear(renderer);

        renderGround(renderer, walls, groundTexture);
        renderMap(renderer, walls);

        player.update();
        player.render(renderer);

        for (auto &enemy : enemies) {
            enemy.render(renderer);
        }
    }

    SDL_RenderPresent(renderer);
}

void Game::run () {

    SDL_Event e;

    while (inMenu) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
                return;
            }
            if (menu->handleEvent(e)) {
                inMenu = false;
            }
        }
        menu->render();
    }

    while (running) {
        handleEvents();
        update();
        render();
        if (gameOver) {
            SDL_Delay(3000);
            running = false;
        }
        SDL_Delay(16);
    }
}
Game::~Game() {
    Player::freeTextures();
    for (SDL_Texture* texture : wallTextures) {
        SDL_DestroyTexture(texture);
    }
    wallTextures.clear();
    for (SDL_Texture* texture : playerTextures) {
        SDL_DestroyTexture(texture);
    }
    playerTextures.clear();
    for (SDL_Texture* texture : enemyTextures) {
        SDL_DestroyTexture(texture);
    }
    enemyTextures.clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    delete menu;
    SDL_Quit();
}
