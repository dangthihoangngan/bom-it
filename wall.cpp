#include "wall.h"


Wall::Wall (int startX, int startY, int wallType, SDL_Texture* tex) {
    x = startX;
    y = startY;
    type = wallType;
    texture = tex;
    active = true;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
}

void Wall::render(SDL_Renderer* renderer) const {
    if (active && texture) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
}

void renderMap(SDL_Renderer* renderer, const vector<Wall>& walls) {
    for (const auto& wall : walls) {
        if (wall.active) {
            wall.render(renderer);
        }
    }
}

void renderGround(SDL_Renderer* renderer, const vector<Wall>& walls, SDL_Texture* groundTexture) {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            SDL_RenderCopy(renderer, groundTexture, NULL, &tile);
        }
    }
}
