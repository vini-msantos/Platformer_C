#include <math.h>
#include <raylib.h>
#include "rendering.h"
#include "level.h"
#include "util.h"

Vector2 camera = (Vector2) {750, 500};

Vector2 mouse_position_absolute(void) {
    return Vec(
        camera.x + GetMouseX() - SCREEN_WIDTH/2.0,
        camera.y + GetMouseY() - SCREEN_HEIGHT/2.0
    );
}

Vector2 point_on_camera(Vector2 p) {
    return vec_add(vec_sub(p, camera), Vec(SCREEN_WIDTH/2.0, SCREEN_HEIGHT/2.0));
}

void lerp_camera(Vector2 pos) {
    float dt = GetFrameTime();
    pos.x = max(pos.x, SCREEN_WIDTH/2.0);
    pos.y = min(pos.y, SCREEN_HEIGHT/2.0);
    Vector2 force = vec_scale(vec_sub(pos, camera), CAMERA_LERP_FACTOR * dt);
    camera = vec_add(camera, force);
    if (fabsf(force.x) < 0.1 && fabsf(force.y) < 0.1) camera = pos;
}

void draw_tile(int i, int j) {
    Tile tile = level[i][j];
    if (tile == AIR) return;
    Vector2 pos = point_on_camera(coord_to_point(coord(i, j)));

    if (tile == PLATFORM) return DrawRectangle(pos.x, pos.y, TILE_SIZE, TILE_SIZE/4.0, BROWN);
    
    Color color = ((i+j) % 2) ? GREEN : DARKGREEN;
    DrawRectangle(pos.x, pos.y, TILE_SIZE, TILE_SIZE, color);
}

void draw_level(void) {
    for (int i = 0; i < LEVEL_HEIGHT; i++) {
        for (int j = 0; j < LEVEL_WIDTH; j++) {
            draw_tile(i, j);
        }
    }
}

void draw_player(Player* p) {
    Vector2 pos = point_on_camera(p->pos);
    DrawRectangle(
        pos.x - PLAYER_WIDTH/2.0,
        pos.y - PLAYER_HEIGHT,
        PLAYER_WIDTH,
        PLAYER_HEIGHT,
        RED
    );
}

void draw_editor_cursor(void) {
    Coord c = point_to_coord(mouse_position_absolute());
    Vector2 p = coord_to_point(c);
    DrawRectangleLinesEx(
        (Rectangle) {
            .x = point_on_camera(p).x,
            .y = point_on_camera(p).y,
            .width = TILE_SIZE,
            .height = TILE_SIZE
        },
        5, RED);
}

void update_camera(void) {
    lerp_camera(vec_add(player.pos, Vec(0, SCREEN_HEIGHT/6.0)));
}
