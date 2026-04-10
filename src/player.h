#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

#define JUMP_BUFFER 0.12
#define PLAYER_WIDTH 48 
#define PLAYER_HEIGHT 96
#define MAX_VEL_X 900
#define MAX_VEL_Y 1000
#define GRAVITY 2000
#define ACCEL 1500
#define AIR_CONTROL 0.75
#define JUMP_FORCE 700
#define DASH_FORCE 500
#define COYOTE_TIME 0.2
#define DASH_TIME 0.8
#define AIR_DRAG 100
#define FRICTION 700
typedef struct Player {
    Vector2 pos; // Anchor: x: centered, y: alligned to bottom
    Vector2 vel;
    bool jumping;
    bool running;
    bool can_dash;
    float dash_timer;
    float coyote_timer;
} Player;

extern Player player;

Player new_player(int x, int y);
void update_player(Player* p);
#endif
