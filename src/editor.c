#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "editor.h"
#include "level.h"
#include "rendering.h"

Tile selected = AIR;

void level_load(char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("ERROR: could not load level from file: \"%s\"", path);
        return;
    }

    int width, height;
    fscanf(file, "%d %d\n", &width, &height); // TODO: MAKE LEVEL SIZE DYNAMIC
    
    char line[102];
    for (int i = 0; i < height; i++) {
        if (fgets(line, 102, file) == NULL) printf("ERROR: error parsing level data.");

        for (int j = 0; j < width; j++) {
            level[i][j] = line[j];
        }
    }

    fclose(file);
}

void level_save(char* path) {
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        printf("ERROR: could not save level to file: \"%s\"", path);
        return;
    }

    fprintf(file, "%d %d\n", LEVEL_WIDTH, LEVEL_HEIGHT);
    
    char line[LEVEL_WIDTH+1];
    line[LEVEL_WIDTH] = '\0';

    for (int i = 0; i < LEVEL_HEIGHT; i++) {
        for (int j = 0; j < LEVEL_WIDTH; j++) {
            line[j] = level[i][j];
        }
        fprintf(file, "%s\n", line);
    }

    fclose(file);
}

void update_editor(void) {
    if (IsKeyPressed(KEY_ONE))        selected = AIR;
    else if (IsKeyPressed(KEY_TWO))   selected = GROUND;
    else if (IsKeyPressed(KEY_THREE)) selected = PLATFORM;
    else if (IsKeyPressed(KEY_FOUR))  selected = COIN;
    else if (IsKeyPressed(KEY_FIVE))  selected = TRAMPOLINE;
    else if (IsKeyPressed(KEY_SIX))   selected = SPIKE;

    Coord c = point_to_coord(mouse_position_absolute());

    if (IsKeyPressed(KEY_P)) level_save("./level.txt");
    if (IsKeyPressed(KEY_L)) level_load("./level.txt");

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (c.i < 0 || c.i >= LEVEL_HEIGHT || c.j < 0 || c.j >= LEVEL_WIDTH) return;
        level[c.i][c.j] = selected;
    }
}
