#include <raylib.h>
#include <stdio.h>
#include "editor.h"
#include "level.h"
#include "rendering.h"

Tile selected = AIR;

void update_editor(void) {
    if (IsKeyDown(KEY_ONE)) selected = AIR;
    else if (IsKeyDown(KEY_TWO)) selected = GROUND;
    else if (IsKeyDown(KEY_THREE)) selected = PLATFORM;
    else if (IsKeyDown(KEY_FOUR)) selected = COIN;
    else if (IsKeyDown(KEY_FIVE)) selected = TRAMPOLINE;


    Coord c = point_to_coord(mouse_position_absolute());

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        printf("mouse i: %d mouse j: %d\n", c.i, c.j);
        printf("mouse x: %f, mouse y: %f\n", mouse_position_absolute().x, mouse_position_absolute().y);
        if (c.i < 0 || c.i >= LEVEL_HEIGHT || c.j < 0 || c.j >= LEVEL_WIDTH) return;
        level[c.i][c.j] = selected;
    }
}
