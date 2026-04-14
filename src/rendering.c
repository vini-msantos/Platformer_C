#include <math.h>
#include <raylib.h>
#include "rendering.h"
#include "level.h"
#include "util.h"
#include "controls.h"

Vector2 camera = (Vector2) {750, 500};

Vector2 mouse_position_absolute(void) {
    return Vec(
        camera.x + GetMouseX() - SCREEN_WIDTH/2.0,
        camera.y + GetMouseY() - SCREEN_HEIGHT/2.0
    );
}

Vector2 point_on_camera(Vector2 p) {
    return vec_addxy(vec_sub(p, camera), SCREEN_WIDTH/2.0, SCREEN_HEIGHT/2.0);
}

void lerp_camera(Vector2 pos) {
    float dt = GetFrameTime();
    pos.x = max(pos.x, SCREEN_WIDTH/2.0);
    pos.y = min(pos.y, SCREEN_HEIGHT/2.0);
    Vector2 force = vec_scale(vec_sub(pos, camera), CAMERA_LERP_FACTOR * dt);
    camera = vec_add(camera, force);
    if (fabsf(force.x) < 0.1 && fabsf(force.y) < 0.1) camera = pos;
}

void draw_ground_tile(Coord c, Vector2 pos) {
    Color color = ((c.i+c.j) % 2) ? GREEN : DARKGREEN;
    DrawRectangle(pos.x, pos.y, TILE_SIZE, TILE_SIZE, color);
}

void draw_platform(Vector2 pos) {
    DrawRectangle(pos.x, pos.y, TILE_SIZE, TILE_SIZE/4.0, BROWN);
}

void draw_coin(Vector2 pos) {
    DrawCircle(pos.x + TILE_SIZE/2.0, pos.y + TILE_SIZE/2.0, COIN_RADIUS, YELLOW);
}

void draw_spike(Vector2 pos) {
    float thick = TILE_SIZE/6.0;
    Vector2 p1 = vec_addxy(pos, thick, thick); 
    Vector2 p2 = vec_addx(p1, TILE_SIZE-2*thick);
    Vector2 p3 = vec_addy(p1, TILE_SIZE-2*thick);
    Vector2 p4 = vec_addx(p3, TILE_SIZE-2*thick);
    DrawLineEx(p1, p4, thick, GRAY);
    DrawLineEx(p2, p3, thick, GRAY);
}

void draw_trampoline(Vector2 pos) {
    float thick = TILE_SIZE/8.0;
    float sz = TILE_SIZE;
    float mrg = thick/2;
    Vector2 p1 = vec_addxy(pos, thick, mrg);
    Vector2 p2 = vec_addx(p1, sz-2*thick);
    Vector2 p3 = vec_addy(p1, sz-2*mrg);
    Vector2 p4 = vec_addx(p3, sz-2*thick);
    // Draw Middle Segment:
    DrawLineEx(vec_add(p2, Vec(-thick, 0)), vec_add(p3, Vec(thick, 0)), thick, GRAY);
    // Draw Top and Bottom:
    DrawLineEx(p1, p2, thick, RED);
    DrawLineEx(p3, p4, thick, RED);
}

void draw_tile(Coord c) {
    Tile tile = level[c.i][c.j];
    if (tile == AIR) return;
    Vector2 pos = point_on_camera(coord_to_point(coord(c.i, c.j)));

    switch (tile) {
        case GROUND:     return draw_ground_tile(c, pos);
        case PLATFORM:   return draw_platform(pos);
        case COIN:       return draw_coin(pos);
        case TRAMPOLINE: return draw_trampoline(pos);
        case SPIKE:      return draw_spike(pos);
        default:         return;
    }
    
}

void draw_level(void) {
    for (int i = 0; i < LEVEL_HEIGHT; i++) {
        for (int j = 0; j < LEVEL_WIDTH; j++) {
            draw_tile(coord(i, j));
        }
    }
}

void draw_player(Player* p) {
    if (!p->visible) return;
    
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

void update_camera_playing(void) {
    Vector2 offset = (IsKeyDown(LOOK_DOWN_KEY)) ? Vec(0, SCREEN_HEIGHT/5.0) : Vec(0, -SCREEN_HEIGHT/8.0); 
    lerp_camera(vec_add(player.pos, offset));
}

void update_camera_editor(void) {
    lerp_camera(vec_add(player.pos, Vec(0, -SCREEN_HEIGHT/8.0)));
}
