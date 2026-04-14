#include "state.h"
#include <raylib.h>
#include <stdio.h>
#include "editor.h"
#include "util.h"
#include "rendering.h"

void exit_state(void);

State state = EDITOR;

void exit_editor(void) {
    level_save("editor_temp.txt");
    
    player.collision = false;
    player.visible = false;
    player.controllable = false;
}

void exit_playing(void) {
    level_load("editor_temp.txt");

    player.collision = false;
    player.visible = false;
    player.controllable = false;

}

void state_playing_enter(void) {
    exit_state();   
    
    player.vel = Vec(0, 0);
    state = PLAYING;
    player.collision = true;
    player.visible = true;
    player.controllable = true;
}

void state_editor_enter(void) {
    exit_state();   

    state = EDITOR;
    player.collision = false;
    player.visible = true;
    player.controllable = true;
}


void exit_state(void) {
    printf("exiting %d", state);
    switch (state) {
        case EDITOR: return exit_editor();
        case PLAYING: return exit_playing();
    }
}

void state_editor_update(void) {
    player_update_editor(&player);
    update_editor();
    update_camera_editor();
}

void state_playing_update(void) {
    player_update_playing(&player);
    update_camera_playing();
}

void state_update(void) {
    switch (state) {
        case EDITOR: return state_editor_update();
        case PLAYING: return state_playing_update();
    }
}
