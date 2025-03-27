#ifndef CONSTANTS_H
#define CONSTANTS_H

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;
const int EXPLOSION_TIME = 3000;
const int EXPLOSION_DURATION = 500;
extern int mapData[MAP_HEIGHT][MAP_WIDTH];
const int SPRITE_WIDTH = 22;
const int SPRITE_HEIGHT = 29;
const int SPRITE_COLUMNS = 3;
const int SPRITE_ROWS = 4;

enum PlayerState { STANDING, PREPARING, MOVING };
enum Direction { DOWN, LEFT, RIGHT, UP };

#endif

