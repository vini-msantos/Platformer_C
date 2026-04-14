#include "util.h"

#include <math.h>

int signf(float x) {
    if (x < 0) return -1;
    return x > 0;
}

Vector2 vec_addx(Vector2 a, float x) {
    return Vec(a.x + x, a.y);
}

Vector2 vec_addy(Vector2 a, float y) {
    return Vec(a.x, a.y + y);
}

Vector2 vec_addxy(Vector2 a, float x, float y) {
    return Vec(a.x + x, a.y + y);
}

float vec_dist(Vector2 a, Vector2 b) {
    return sqrt((b.x-a.x)*(b.x-a.x) + (b.y-a.y)*(b.y-a.y));
}

Vector2 Vec(float x, float y) {
    return (Vector2) {x, y};
}

Vector2 vec_add(Vector2 a, Vector2 b) {
    return Vec(a.x + b.x, a.y + b.y);
}

Vector2 vec_sub(Vector2 a, Vector2 b) {
    return Vec(a.x - b.x, a.y - b.y);
}

Vector2 vec_mul(Vector2 a, Vector2 b) {
    return Vec(a.x * b.x, a.y * b.y);
}

Vector2 vec_scale(Vector2 a, float scalar) {
    return Vec(a.x * scalar, a.y * scalar);
}

