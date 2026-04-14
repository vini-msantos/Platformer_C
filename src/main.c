#include <raylib.h>
#include <stdint.h>
#include "player.h"
#include "level.h"
#include "state.h"
#include "rendering.h"

void handle_drawing(void) {
    BeginDrawing();
    ClearBackground(SKYBLUE);

    draw_level();
    draw_player(&player);
    if (state == EDITOR) draw_editor_cursor();
    
    EndDrawing();
}

void setup(void) {
    player = new_player(200, 750);
    level_clear(level);
    level_set_rect(level, (Rectangle){0, 27, 5, 3}, GROUND);
}

int main() {
    InitWindow(1500, 1000, "Platformer");
    SetTargetFPS(60);

    setup();
    
    while (!WindowShouldClose()) {
        handle_drawing();

        state_update();
    }

    CloseWindow();
}
    
