#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include "player.h"
#include "util.h"
#include "level.h"
#include "controls.h"

Player player;
int coins = 0;

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
    Coord c_left = point_to_coord(vec_add(p->pos, Vec(PLAYER_WIDTH/2.0, 0.5)));
    Coord c_right = point_to_coord(vec_add(p->pos, Vec(-PLAYER_WIDTH/2.0, 0.5)));
    return tile_should_collide_y(c_left, p) || tile_should_collide_y(c_right, p);
}

void player_jump(Player* p, float force) {
    p->vel.y = -force;
    p->jumping = true;
}

void player_hitbox_offsets(Player* p, Vector2* buf) {
    buf[0] = Vec(PLAYER_WIDTH/2.0, 0);
    buf[1] = Vec(-PLAYER_WIDTH/2.0, 0);
    buf[2] = Vec(PLAYER_WIDTH/2.0, -PLAYER_HEIGHT/2.0);
    buf[3] = Vec(-PLAYER_WIDTH/2.0, -PLAYER_HEIGHT/2.0);
    buf[4] = Vec(PLAYER_WIDTH/2.0, -PLAYER_HEIGHT);
    buf[5] = Vec(-PLAYER_WIDTH/2.0, -PLAYER_HEIGHT);
}

void player_collide_coin(Player* p, Coord c, Vector2 collision_point) {
    Vector2 tile_center = vec_add(coord_to_point(c), Vec(TILE_SIZE/2.0, TILE_SIZE/2.0));
    if (fabsf(vec_dist(collision_point, tile_center)) > COIN_RADIUS) return;

    level[c.i][c.j] = AIR;
    coins++;
}

void player_collide_trampoline(Player* p) {
    if (!p->jumping && p->vel.y > 0) player_jump(p, JUMP_FORCE*2);
}

void player_on_collide(Player* p, Coord c, Vector2 collision_point) {    
    Tile tile = level_tile_coord(level, c);
    if (tile == COIN) player_collide_coin(p, c, collision_point);
    if (tile == TRAMPOLINE) player_collide_trampoline(p);
}

void player_collide_solid(Player* p, Coord c, Vector2 collision_point) {
    Vector2 offset = vec_sub(p->pos, collision_point);
    Vector2 tile_center = vec_add(coord_to_point(c), Vec(TILE_SIZE/2.0, TILE_SIZE/2.0));
    Vector2 diff = vec_sub(collision_point, tile_center);

    // printf("collided block: %d,  i: %d, j: %d\n", level_tile_coord(level, c), c.i, c.j);
    // printf("off_x: %f, off_y: %f\n", offset.x, offset.y);
    // printf("centerx: %f, centery: %f\n", tile_center.x, tile_center.y);
    printf("cpoint: %f, cpoint: %f\n", collision_point.x, collision_point.y);
    printf("diff_x: %f, diff_y: %f\n", diff.x, diff.y);

    Coord x_neighbor = (Coord) {c.i, c.j + signf(diff.x)};
    Coord y_neighbor = (Coord) {c.i + signf(diff.y), c.j};

    bool collide_x = tile_should_collide_x(c, p) && !tile_should_collide_x(x_neighbor, p);
    bool collide_y = tile_should_collide_y(c, p) && !tile_should_collide_y(y_neighbor, p);

    // printf("Collide x: %d, collide y: %d\n", collide_x, collide_y);

    if (collide_x && fabsf(diff.x) >= fabsf(diff.y)) {
        p->vel.x = 0;
        p->pos.x = offset.x + tile_center.x + signf(diff.x) * TILE_SIZE/1.99;
    } else if (collide_y) {
        p->vel.y = 0;
        p->pos.y = offset.y + tile_center.y + signf(diff.y) * TILE_SIZE/1.99;
    }

    player_on_collide(p, c, collision_point);
}

void player_check_collision(Player* p, Vector2 point, Coord c) {
    if (tile_is_solid(c)) player_collide_solid(p, c, point);
    else if (tile_check_collision(c)) player_on_collide(p, c, point);
}

void player_tile_collision(Player* p) {
    Vector2 points[6];
    player_hitbox_offsets(p, points);
    for (int i = 0; i < 6; i++) {
        Vector2 point = vec_add(p->pos, points[i]);
        Coord coord = point_to_coord(point);

        if (!tile_check_collision(coord)) continue;
        
        printf("point index: %d\n", i);
        
        player_check_collision(p, point, coord);     
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


void player_handle_jumping(Player* p) {
    if (buffered_jump() && player_can_jump(p)) player_jump(p, JUMP_FORCE);
}

void player_handle_moving(Player* p) {
    int x_dir = IsKeyDown(RIGHT_KEY) - IsKeyDown(LEFT_KEY);
    if (x_dir != 0) last_dir = x_dir;

    float dt       = GetFrameTime();
    bool  grounded = player_is_grounded(p);
    float friction = (grounded) ? FRICTION : AIR_DRAG;
    float control  = (grounded) ? 1.0 : AIR_CONTROL;
    int   dir_mult = (x_dir != signf(p->vel.x)) ? 5 : 1;
    float run_mult = (p->running) ? 1 : 0.5;  

    if (x_dir == 0 || fabsf(p->vel.x) > MAX_VEL_X) {
        p->vel.x -= (p->vel.x * friction/150 + friction * signf(p->vel.x)) * dt;
    }
    if (fabsf(p->vel.x) < 1) p->vel.x = 0;

    if (signf(p->vel.x) != x_dir || fabsf(p->vel.x) < MAX_VEL_X * run_mult) {
        p->vel.x += x_dir * ACCEL * control * dir_mult * dt;
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
    // DEBUG:
    if (IsKeyPressed(KEY_I)) {
        printf("\n\n\n\n");
    }
    if (IsKeyPressed(KEY_P)) {
        printf("pos x: %f, pos y: %f\n", p->pos.x, p->pos.y);
    }
    
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

