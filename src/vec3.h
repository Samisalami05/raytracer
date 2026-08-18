#ifndef VEC3_H
#define VEC3_H

#include "vec2.h"

typedef union {
	struct { float x, y, z; };
	struct { Vec2 xy; float __pad1; };
	struct { float __pad2; Vec2 yz; };
	struct { float data[3]; };
} Vec3;

#define vec3(xv, yv, zv) (Vec3){.x = xv, .y = yv, .z = zv}

float vec3_magnitude(Vec3 v);

void vec3_normalize(Vec3* v);
Vec3 vec3_normalized(Vec3 v);
float vec3_dot(Vec3 v1, Vec3 v2);
Vec3 vec3_inverse(Vec3 v);
Vec3 vec3_cross(Vec3 v1, Vec3 v2);
float vec3_angle(Vec3 v1, Vec3 v2);
Vec3 vec3_rotate_euler(Vec3 v, Vec3 axis, float angle);

Vec3 vec3_add(Vec3 v1, Vec3 v2);
Vec3 vec3_sub(Vec3 v1, Vec3 v2);
Vec3 vec3_mul(Vec3 v1, Vec3 v2);
Vec3 vec3_div(Vec3 v1, Vec3 v2);

Vec3 vec3_add_f(Vec3 v1, float v2);
Vec3 vec3_sub_f(Vec3 v1, float v2);
Vec3 vec3_scale(Vec3 v, float s);
Vec3 vec3_div_f(Vec3 v1, float v2);

void vec3_print(Vec3 v);

#endif
