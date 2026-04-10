#ifndef LEVEL_H
#define LEVEL_H

#include <raylib.h>

#define LEVEL_WIDTH 100
#define LEVEL_HEIGHT 30

#define TILE_SIZE 64
typedef enum Tile {
    AIR = ' ',
    GROUND = '#',
    COIN = 'o',
    SPIKE = 'x',
    TRAMPOLINE = 'z',
    PLATFORM = '-'
} Tile;

typedef struct Coord {
    int i;
    int j;
} Coord;

typedef Tile Level[LEVEL_HEIGHT][LEVEL_WIDTH];

extern Level level;

void level_clear(Level level);
void level_set_rect(Level level, Rectangle rect, Tile tile);

Coord coord(int i, int j);

Coord point_to_coord(Vector2 point);
Vector2 coord_to_point(Coord coord); // RETURNS TOP LEFT CORNER OF TILE

Tile level_tile_coord(Level level, Coord coord);
Tile level_tile_point(Level level, Vector2 point);



#endif
