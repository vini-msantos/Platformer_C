#include "util.h"

int signf(float x) {
    if (x < 0) return -1;
    return x > 0;
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

