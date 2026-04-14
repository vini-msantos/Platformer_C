#include <raylib.h>
#include <stdint.h>
#include "player.h"
#include "level.h"
#include "rendering.h"
#include "editor.h"

void handle_drawing(void) {
    BeginDrawing();
    ClearBackground(SKYBLUE);

    draw_level();
    draw_player(&player);
    draw_editor_cursor();
    
    EndDrawing();
}

void setup(void) {
    player = new_player(1000, 500);
    level_clear(level);
    level_set_rect(level, (Rectangle){5, 25, 75, 3}, GROUND);
    level_set_rect(level, (Rectangle){20, 21, 1, 4}, GROUND);
    level_set_rect(level, (Rectangle){8, 20, 4, 1}, PLATFORM);
}

int main() {
    InitWindow(1500, 1000, "Platformer");
    SetTargetFPS(60);

    setup();
    
    while (!WindowShouldClose()) {
        handle_drawing();

        update_player(&player);
        update_editor();
        update_camera();
    }

    CloseWindow();
}
    
