#ifndef VEC4_H
#define VEC4_H

#include "vec2.h"
#include "vec3.h"

typedef union {
	struct { float x, y, z, w; };
	struct { Vec2 xy; Vec2 zw; };
	struct { float __pad1; Vec2 yz; float __pad2; };
	struct { Vec3 xyz; float __pad3; };
	struct { float __pad4; Vec3 yzw; };
	struct { float data[4]; };
} Vec4;

#define vec4(xv, yv, zv, wv) (Vec4){.x = xv, .y = yv, .z = zv, .w = wv}

#endif
