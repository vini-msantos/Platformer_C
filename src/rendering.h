#ifndef RENDERING_H
#define RENDERING_H

#include <raylib.h>
#include "player.h"

#define CAMERA_LERP_FACTOR 5

extern Vector2 camera;

Vector2 mouse_position_absolute(void);
Vector2 point_on_camera(Vector2 p);
void update_camera_playing(void);
void update_camera_editor(void);
void draw_level(void);
void draw_player(Player* p);
void draw_editor_cursor(void);

#endif
