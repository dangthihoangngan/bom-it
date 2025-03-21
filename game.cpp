#include "game.h"

void Game::update() {
    player.updateBombs();

    std::vector<std::vector<Wall>::iterator> wallsToRemove;

    for (auto it = player.bombs.begin(); it != player.bombs.end();) {
        if (it->exploded) {
            SDL_Rect explosionRects[5];
            explosionRects[0] = { it->x, it->y, TILE_SIZE, TILE_SIZE };
            explosionRects[1] = { it->x - TILE_SIZE, it->y, TILE_SIZE, TILE_SIZE };
            explosionRects[2] = { it->x + TILE_SIZE, it->y, TILE_SIZE, TILE_SIZE };
            explosionRects[3] = { it->x, it->y - TILE_SIZE, TILE_SIZE, TILE_SIZE };
            explosionRects[4] = { it->x, it->y + TILE_SIZE, TILE_SIZE, TILE_SIZE };

            for (int i = 0; i < 5; ++i) {
                for (auto wallIt = walls.begin(); wallIt != walls.end(); ++wallIt) {
                    if (wallIt->active && SDL_HasIntersection(&explosionRects[i], &wallIt->rect)) {
                        wallIt->active = false;
                        wallsToRemove.push_back(wallIt);
                    }
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
    for (auto& wallIt : wallsToRemove) {
        walls.erase(wallIt);
    }
}

void Game::generateWalls() {
    for (int i = 3; i < MAP_HEIGHT - 3; i+=2){
        for (int j = 3; j < MAP_WIDTH - 3; j+=2){
            Wall w = Wall(j * TILE_SIZE, i * TILE_SIZE);
            walls.push_back(w);
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
                case SDLK_UP: player.move(0, -5, walls); break;
                case SDLK_DOWN: player.move(0, 5, walls); break;
                case SDLK_LEFT: player.move(-5, 0, walls); break;
                case SDLK_RIGHT: player.move(5, 0, walls); break;
                case SDLK_SPACE: player.placeBomb(); break;
            }
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
    generateWalls();
    player = Player(((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE);
}

void Game::render () {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 225);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 1; i < MAP_HEIGHT - 1;++i) {
        for (int j = 1; j < MAP_WIDTH - 1;++j) {
            SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &tile);
        }
    }

    for (int i = 0; i < walls.size(); i++){
        walls[i].render(renderer);
    }

    player.render(renderer);
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
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
