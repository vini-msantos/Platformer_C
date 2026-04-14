#include <stdio.h>
#include <raylib.h>
#include "editor.h"
#include "level.h"
#include "rendering.h"

Tile selected = AIR;

void update_editor(void) {
    if (IsKeyDown(KEY_ONE)) selected = GROUND;
    else if (IsKeyDown(KEY_TWO)) selected = PLATFORM;
    else if (IsKeyDown(KEY_THREE)) selected = AIR;

    Coord c = point_to_coord(mouse_position_absolute());
    printf("i: %d, j: %d\n", c.i, c.j);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (c.i < 0 || c.i >= LEVEL_HEIGHT || c.j < 0 || c.j >= LEVEL_WIDTH) return;
        level[c.i][c.j] = selected;
    }
}
