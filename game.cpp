#include "game.h"
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL_image.h>


using namespace std;

void Game::setGameMode(GameMode mode) {
    gameMode = mode;
    if (gameMode == TWO_PLAYER) {
        player2 = Player(((MAP_WIDTH + 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, player2Textures);
    }
}

void Game::playMusic() {
    if (backgroundMusic) {
        Mix_PlayMusic(backgroundMusic, -1);
    }
}

void Game::stopMusic() {
    Mix_HaltMusic();
}

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

    std::vector<std::pair<int, int>> validPositions;

    for (int row = 0; row < MAP_HEIGHT; ++row) {
        for (int col = 0; col < MAP_WIDTH; ++col) {
            if (mapData[row][col] == 0) {
                validPositions.push_back({col * TILE_SIZE, row * TILE_SIZE});
            }
        }
    }

    if (validPositions.empty()) return;

    for (int i = 0; i < enemyNumber; ++i) {
        int index = rand() % validPositions.size();
        int ex = validPositions[index].first;
        int ey = validPositions[index].second;

        if (rand() % 3 != 0) {
            enemies.push_back(new WalkingEnemy(ex, ey, walkingEnemyTextures));
        } else {
            enemies.push_back(new ShootingEnemy(ex, ey, shootingEnemyTextures, bulletTexture));
        }
    }
}



void Game::update() {
    player.updateBombs(walls, enemies, gameOver, playerWon, bombExplosionSound);

    if (gameMode == TWO_PLAYER) {
        player2.update();
        player2.updateBombs(walls, enemies, gameOver, playerWon, bombExplosionSound);
    }

    for (auto* enemy : enemies) {
        enemy->move(walls,player.getBombs());

        if (gameMode == TWO_PLAYER) {
            enemy->move(walls,player2.getBombs());
        }

        if (dynamic_cast<WalkingEnemy*>(enemy)) {
            if (SDL_HasIntersection(&player.rect, &enemy->rect)) {
                gameOver = true;
                playerWon = false;
                return;
            }
        }

        ShootingEnemy* shooter = dynamic_cast<ShootingEnemy*>(enemy);
        if (shooter) {
            shooter->updateBullets(this->walls);
            if (rand() % 100 < 2) {
                shooter->shoot();
            }

            for (auto& bullet : shooter->bullets) {
                if (bullet.active && SDL_HasIntersection(&player.rect, &bullet.rect)) {
                    gameOver = true;
                    playerWon = false;
                    return;
                }
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
        } else if (gameMode == SINGLE_PLAYER) {
                if (event.type == SDL_KEYDOWN) {
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
        } else if (gameMode == TWO_PLAYER) {
            if (event.type == SDL_KEYDOWN) {
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
                case SDLK_RETURN:
                    player.placeBomb();
                    break;

                case SDLK_w:
                    player2.move(0, -5, walls);
                    player2.direction = UP;
                    player2.state = MOVING;
                    break;
                case SDLK_s:
                    player2.move(0, 5, walls);
                    player2.direction = DOWN;
                    player2.state = MOVING;
                    break;
                case SDLK_a:
                    player2.move(-5, 0, walls);
                    player2.direction = LEFT;
                    player2.state = MOVING;
                    break;
                case SDLK_d:
                    player2.move(5, 0, walls);
                    player2.direction = RIGHT;
                    player2.state = MOVING;
                    break;
                case SDLK_SPACE:
                    player2.placeBomb();
                    break;
            }
        } else if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN ||
                event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) {
                if (!SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP] &&
                    !SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN] &&
                    !SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT] &&
                    !SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT]) {
                    player.state = STANDING;
                }
            }

            if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_s ||
                event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_d) {
                if (!SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W] &&
                    !SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S] &&
                    !SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A] &&
                    !SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]) {
                    player2.state = STANDING;
                }
            }
        }
    }
     }

}

Game::Game(){
    running = true;
    inMenu = true;
    menu = new Menu(renderer);
    gameOver = false;
    playerWon = false;
    gameMode = SINGLE_PLAYER;

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }
    window = SDL_CreateWindow("BOM IT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }

    Player::loadBombTextures(renderer);
    bulletTexture = loadTexture("assets/bullet.png");

    winScreen = loadTexture("assets/win.png");
    loseScreen = loadTexture("assets/lose.png");
    groundTexture = loadTexture("assets/ground.png");

    if (!winScreen || !loseScreen || !groundTexture) {
        cerr << "Failed to load some UI textures! SDL_Error: " << SDL_GetError() << endl;
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

    for (int i = 0; i < 12; i++) {
        string path = "assets/character2" + to_string(i + 1) + ".png";
        SDL_Texture* texture = loadTexture(path.c_str());
        if (!texture) {
            cerr << "Failed to load " << path << " SDL_Error: " << SDL_GetError() << endl;
            running = false;
        } else {
            player2Textures.push_back(texture);
        }
    }



    for (int i = 0; i < 12; i++) {
        string path = "assets/walking_enemy" + to_string(i + 1) + ".png";
        SDL_Texture* texture = loadTexture(path.c_str());
        if (!texture) {
            cerr << "Failed to load " << path << " SDL_Error: " << SDL_GetError() << endl;
            running = false;
        } else {
            walkingEnemyTextures.push_back(texture);
        }
    }

    for (int i = 0; i < 12; i++) {
        string path = "assets/shooting_enemy" + to_string(i + 1) + ".png";
        SDL_Texture* texture = loadTexture(path.c_str());
        if (!texture) {
            cerr << "Failed to load " << path << " SDL_Error: " << SDL_GetError() << endl;
            running = false;
        } else {
            shootingEnemyTextures.push_back(texture);
        }
    }
    spawnEnemies();


    if (walkingEnemyTextures.empty() || shootingEnemyTextures.empty() || wallTextures.empty() || playerTextures.empty()) {
        cerr << "Failed to load some textures! Exiting..." << endl;
        running = false;
    }

    generateWalls();
    player = Player(((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, playerTextures);
    player2 = Player(((MAP_WIDTH + 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE, player2Textures);




    menu = new Menu(renderer);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << endl;
    }

    bombExplosionSound = Mix_LoadWAV("assets/bomb_explosion.wav");
    if (!bombExplosionSound) {
        cerr << "Failed to load explosion sound: " << Mix_GetError() << endl;
    }

    backgroundMusic = Mix_LoadMUS("assets/background.mp3");
    if (!backgroundMusic) {
        cout << "Failed to load background music! Mix Error: " << Mix_GetError() << std::endl;
    }
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
        if (gameMode == TWO_PLAYER) {
            player2.render(renderer);
        }

        for (auto &enemy : enemies) {
            enemy->render(renderer);
        }
    }

    SDL_RenderPresent(renderer);
}

void Game::run () {

    SDL_Event e;

    playMusic();

    while (inMenu) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
                return;
            }

            if (menu->handleEvent(e)) {
                inMenu = false;
                setGameMode(menu->selectedGameMode);
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

    stopMusic();
}
Game::~Game() {
    Player::freeTextures();
    for (SDL_Texture* texture : wallTextures) {
        SDL_DestroyTexture(texture);
    }
    if (bulletTexture) SDL_DestroyTexture(bulletTexture);
    wallTextures.clear();
    for (SDL_Texture* texture : playerTextures) {
        SDL_DestroyTexture(texture);
    }
    playerTextures.clear();
    for (SDL_Texture* texture : walkingEnemyTextures) {
        SDL_DestroyTexture(texture);
    }
    walkingEnemyTextures.clear();
    for (SDL_Texture* texture : shootingEnemyTextures) {
        SDL_DestroyTexture(texture);
    }
    shootingEnemyTextures.clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    delete menu;
    Mix_FreeChunk(bombExplosionSound);
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();
    SDL_Quit();
}
