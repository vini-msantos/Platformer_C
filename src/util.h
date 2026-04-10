#ifndef UTIL_H
#define UTIL_H

#include <raylib.h>

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
      _a < _b ? _a : _b; })

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
      _a > _b ? _a : _b; })

#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 1000

int signf(float x);
Vector2 Vec(float x, float y);
Vector2 vec_add(Vector2 a, Vector2 b);
Vector2 vec_sub(Vector2 a, Vector2 b);
Vector2 vec_mul(Vector2 a, Vector2 b);
Vector2 vec_scale(Vector2 a, float scalar);

#endif
