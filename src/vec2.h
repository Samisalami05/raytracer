#ifndef VEC2_H
#define VEC2_H

typedef union {
	struct { float x, y; };
	struct { float data[2]; };
} Vec2;

#define vec2(xv, yv) (Vec2){.x = xv, .y = yv}

float vec2_magnitude(Vec2 v);
Vec2 vec2_normalized(Vec2 v);

Vec2 vec2_add(Vec2 a, Vec2 b);

Vec2 vec2_scale(Vec2 v, float s);

void vec2_print(Vec2 v);

#endif
