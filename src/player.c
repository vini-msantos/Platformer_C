#include <math.h>
#include <raylib.h>
#include "player.h"
#include "util.h"
#include "level.h"
#include "controls.h"

Player player;

int last_dir = 1;
float jump_buffer = 0;
bool buffered_jump(void) {
    jump_buffer = max(jump_buffer - GetFrameTime(), 0);
    if (IsKeyDown(JUMP_KEY)) jump_buffer = JUMP_BUFFER;
    return jump_buffer > 0;
}

Player new_player(int x, int y) {
    return (Player) {
        Vec(x, y),
        Vec(0, 0),
        false,
        0.0
    };
}

bool player_is_grounded(Player* p) {
    Vector2 bot_right_point = vec_add(p->pos, Vec(PLAYER_WIDTH/2.0, 0.5));
    Vector2 bot_left_point = vec_add(p->pos, Vec(-PLAYER_WIDTH/2.0, 0.5));
    return level_tile_point(level, bot_left_point) != AIR || level_tile_point(level, bot_right_point) != AIR;
}

void player_hitbox_offsets(Player* p, Vector2* buf) {
    buf[0] = Vec(PLAYER_WIDTH/2.0, 0);
    buf[1] = Vec(-PLAYER_WIDTH/2.0, 0);
    buf[2] = Vec(PLAYER_WIDTH/2.0, -PLAYER_HEIGHT/2.0);
    buf[3] = Vec(-PLAYER_WIDTH/2.0, -PLAYER_HEIGHT/2.0);
    buf[4] = Vec(PLAYER_WIDTH/2.0, -PLAYER_HEIGHT);
    buf[5] = Vec(-PLAYER_WIDTH/2.0, -PLAYER_HEIGHT);
}

bool tile_should_collide(Coord c, Player* p) {
    Tile tile = level_tile_coord(level, c);
    if (tile == PLATFORM && point_to_coord(p->pos).i <= c.i) return true;
    return tile != AIR;
}

void player_adjust_point(Player* p, Vector2 point, Coord c) {
    Vector2 point_offset = vec_sub(p->pos, point);
    Vector2 tile_center = vec_add(coord_to_point(c), Vec(TILE_SIZE/2.0, TILE_SIZE/2.0));
    Vector2 diff = vec_sub(point, tile_center);

    Coord neighbor_x = (Coord) {c.i, c.j + signf(diff.x)};
    Coord neighbor_y = (Coord) {c.i + signf(diff.y), c.j};

    if (fabsf(diff.x) >= fabsf(diff.y) && !tile_should_collide(neighbor_x, p)) {
        p->vel.x = 0;
        p->pos.x = point_offset.x + tile_center.x + signf(diff.x) * TILE_SIZE/1.99;
    } else if (!tile_should_collide(neighbor_y, p)) {
        p->vel.y = 0;
        p->pos.y = point_offset.y + tile_center.y + signf(diff.y) * TILE_SIZE/1.99;
    }
}

void player_tile_collision(Player* p) {
    Vector2 points[6];
    player_hitbox_offsets(p, points);
    for (int i = 0; i < 6; i++) {
        Vector2 point = vec_add(p->pos, points[i]);
        Coord coord = point_to_coord(point);
        if (!tile_should_collide(coord, p)) continue;
        player_adjust_point(p, point, coord);     
    }
}

bool player_can_jump(Player* p) {
    return player_is_grounded(p) || p->coyote_timer > 0;
}

void player_allow_jump(Player* p) {
    p->coyote_timer = COYOTE_TIME;
}

void player_in_air(Player* p) {
    float dt = GetFrameTime();
    p->coyote_timer = max(p->coyote_timer - dt, 0);
    if (p->vel.y >= 0 && p->jumping) p->jumping = false;
    if (p->vel.y < 0 && IsKeyUp(JUMP_KEY) && p->jumping) {
        p->vel.y *= 0.6;    
        p->jumping = false;
    }; 

    p->vel.y += GRAVITY * dt; 
}

void player_apply_vel(Player* p) {
    float dt = GetFrameTime();
    p->pos.x += p->vel.x * dt;
    p->pos.y += p->vel.y * dt;
}

void player_jump(Player* p) {
    p->vel.y = -JUMP_FORCE;
    p->jumping = true;
}

void player_handle_jumping(Player* p) {
    if (buffered_jump() && player_can_jump(p)) player_jump(p);
}

void player_handle_moving(Player* p) {
    float dt = GetFrameTime();

    int x_dir = IsKeyDown(RIGHT_KEY) - IsKeyDown(LEFT_KEY);
    if (x_dir != 0) last_dir = x_dir;

    bool grounded = player_is_grounded(p);
    float friction = (grounded) ? FRICTION : AIR_DRAG;
    float control = (grounded) ? 1.0 : AIR_CONTROL;
    int mult = (x_dir != signf(p->vel.x)) ? 5 : 1;

    if (x_dir == 0 || fabsf(p->vel.x) > MAX_VEL_X) {
        p->vel.x -= (p->vel.x * friction/150 + friction * signf(p->vel.x)) * dt;
    }
    if (fabsf(p->vel.x) < 1) p->vel.x = 0;

    float sprint_mult = (p->running) ? 1 : 0.5;  
    if (signf(p->vel.x) != x_dir || fabsf(p->vel.x) < MAX_VEL_X * sprint_mult) {
        p->vel.x += x_dir * ACCEL * control * mult * dt;
    }
}

void player_dash(Player* p) {
    if (IsKeyDown(UP_KEY)) {
        p->vel.x += last_dir * DASH_FORCE * 0.5;
        p->vel.y = -DASH_FORCE * 1.4;
    } else {
        p->vel.x += last_dir * DASH_FORCE;
        p->vel.y = -DASH_FORCE * 0.6;
    }
    p->can_dash = false;
    p->dash_timer = DASH_TIME;
}

void player_handle_dashing(Player* p) {   
    p->dash_timer = max(0, p->dash_timer - GetFrameTime());
    if (p->dash_timer == 0 && player_is_grounded(p)) p->can_dash = true;
    
    if (IsKeyPressed(DASH_KEY)) {
        p->running = true;  
        if (p->can_dash) player_dash(p);
    } 
    if (IsKeyUp(DASH_KEY) && fabsf(p->vel.x) < MAX_VEL_X) p->running = false;
}

void player_handle_input(Player* p) {
    player_handle_moving(p);
    player_handle_dashing(p);
    player_handle_jumping(p);
}

void player_normalize(Player* p) {
    p->pos.x = max(p->pos.x, PLAYER_WIDTH/2.0);
    if (p->pos.x == PLAYER_WIDTH/2.0) p->vel.x = max(p->vel.x, 0);

    p->pos.y = min(p->pos.y, 2000);

    p->vel.y = min(p->vel.y, MAX_VEL_Y);
}

void update_player(Player* p) {
    float dt = GetFrameTime();

    if (!player_is_grounded(p)) player_in_air(p);
    else player_allow_jump(p); 

    player_handle_input(p);
    player_apply_vel(p);    

    player_tile_collision(p);
    player_normalize(p);
}

