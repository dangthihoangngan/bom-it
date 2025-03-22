#ifndef WALL_H
#define WALL_H

#include <vector>
#include <SDL2/SDL.h>
#include "constants.h"

using namespace std;

class Wall {
public:
    int x, y;
    int type;
    SDL_Rect rect;
    bool active;
    SDL_Texture* texture;

    Wall (int startX, int startY, int wallType, SDL_Texture* tex);
    void render(SDL_Renderer* renderer) const;
};

void renderMap(SDL_Renderer* renderer, const vector<Wall>& walls);
void renderGround(SDL_Renderer* renderer, const vector<Wall>& walls, SDL_Texture* groundTexture);

#endif
