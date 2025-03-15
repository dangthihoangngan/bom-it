#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>

using namespace std;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;
const int EXPLOSION_TIME = 3000;
const int EXPLOSION_DURATION = 500;

class Bomb {
public:
    int x, y;
    SDL_Rect rect;
    Uint32 placedTime;
    Uint32 explosionEndTime;
    volatile bool exploded;

    Bomb(int startX, int startY) {
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        placedTime = SDL_GetTicks();
        explosionEndTime = placedTime + EXPLOSION_TIME + EXPLOSION_DURATION; 
        exploded = false;
    }

    void update() {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 timeSincePlaced = currentTime - placedTime;
        if (!exploded && (timeSincePlaced >= EXPLOSION_TIME)) {
        exploded = true;
        explosionEndTime = currentTime + EXPLOSION_DURATION;
        cout<<exploded;
        }
    }


    void render(SDL_Renderer* renderer) {
    cout << "Bomb::render() called" << endl;
    cout<<exploded;
    if (!exploded) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    } else {
            if (exploded && SDL_GetTicks() < explosionEndTime) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_Rect explosionRects[5] = {
                    {x, y, TILE_SIZE, TILE_SIZE},
                    {x - TILE_SIZE, y, TILE_SIZE, TILE_SIZE},
                    {x + TILE_SIZE, y, TILE_SIZE, TILE_SIZE},
                    {x, y - TILE_SIZE, TILE_SIZE, TILE_SIZE},
                    {x, y + TILE_SIZE, TILE_SIZE, TILE_SIZE}
                };
                for (int i = 0; i < 5; i++) {
                    SDL_RenderFillRect(renderer, &explosionRects[i]);
                }
            }
        }
    }
};

class Wall {
public:
    int x, y;
    SDL_Rect rect;
    bool active;
    Wall (int startX, int startY) {
        x = startX;
        y = startY;
        active = true;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
    }
    void render(SDL_Renderer* renderer) {
        if(active) {
            SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
};

class Player {
public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;
    vector <Bomb> bombs;

    Player () {
        x = 0;
        y = 0;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        dirX = 0;
        dirY = -1;
    }

    Player (int startX, int startY) {
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE, TILE_SIZE};
        dirX = 0;
        dirY = -1;
    }

    void placeBomb() {
        bombs.push_back(Bomb(x,y));
    }

    void updateBombs() {
        for (auto &bomb : bombs) {
            bomb.update();
        }
        bombs.erase(std::remove_if(bombs.begin(), bombs.end(), [](Bomb &b) {
        return b.exploded && SDL_GetTicks() >= b.explosionEndTime;}), bombs.end());
    }

    void move(int dx, int dy, const vector<Wall>& walls) {
        int newX = x + dx;
        int newY = y + dy;
        this->dirX = dx;
        this->dirY = dy;

        SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE};
        for (int i = 0; i < walls.size(); i++) {
            if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)) {
                return;
            }
        }

        if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
            newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
            x = newX;
            y = newY;
            rect.x = x;
            rect.y = y;
        }
    }

    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
        for (auto &bomb : bombs) {
            bomb.render(renderer);
        }
    }
};

class Game{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    vector <Wall> walls;
    Player player;


    void update() {
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
                            std::cout << "Wall hit at: " << wallIt->rect.x << ", " << wallIt->rect.y << std::endl;
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

    void generateWalls() {
        for (int i = 3; i < MAP_HEIGHT - 3; i+=2){
            for (int j = 3; j < MAP_WIDTH - 3; j+=2){
                Wall w = Wall(j * TILE_SIZE, i * TILE_SIZE);
                walls.push_back(w);
            }
        }
    }
    void handleEvents() {
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
    Game(){
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
    void render () {
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
    void run () {
        while (running) {
            handleEvents();
            update();
            render();
            SDL_Delay(16);
        }
    }
    ~Game() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

int main(int argc, char* argv[]) {
    Game game;
    if(game.running){
        game.run();
    }
    return 0;
}
