#include <raylib.h>
#include "level.h"
#include "util.h"

Level level;

Coord coord(int i, int j) {
    return (Coord) {i, j};
}

void level_clear(Level level) {
    for (int i = 0; i < LEVEL_HEIGHT; i++) {
        for (int j = 0; j < LEVEL_WIDTH; j++) {
            level[i][j] = AIR;
        }
    }
}

void level_set_rect(Level level, Rectangle rec, Tile tile) {
    for (int i = rec.y; i < rec.y + rec.height; i++) {
        for (int j = rec.x; j < rec.x + rec.width; j++) {
            level[i][j] = tile;
        }
    }
}


Coord point_to_coord(Vector2 p) {
    return (Coord) {
        .i = LEVEL_HEIGHT - (SCREEN_HEIGHT-p.y)/TILE_SIZE,
        .j = p.x/TILE_SIZE,
    };
}

// RETURNS TOP LEFT CORNER OF TILE
Vector2 coord_to_point(Coord c) {
    return Vec(
        c.j * TILE_SIZE,
        SCREEN_HEIGHT-(LEVEL_HEIGHT-c.i)*TILE_SIZE
    );
}

Tile level_tile_coord(Level level, Coord c) {
    if (c.i < 0 || c.j < 0 || c.i >= LEVEL_HEIGHT || c.j >= LEVEL_WIDTH) return AIR;
    return level[c.i][c.j];
}

Tile level_tile_point(Level level, Vector2 p) {
    return level_tile_coord(level, point_to_coord(p));
}

bool tile_should_collide_x(Coord c, Player* p) {
    Tile tile = level_tile_coord(level, c);
    return tile != AIR && tile != PLATFORM;
}

bool tile_should_collide_y(Coord c, Player* p) {
    Tile tile = level_tile_coord(level, c);
    if (tile == PLATFORM) return p->vel.y >= 0 && p->pos.y <= coord_to_point(c).y+TILE_SIZE/4.0;
    return tile != AIR;
}

bool tile_check_collision(Coord c) {
    return level_tile_coord(level, c) != AIR;
}

bool tile_is_solid(Coord c) {
    return level_tile_coord(level, c) != AIR;
}
