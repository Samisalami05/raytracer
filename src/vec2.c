#include "vec2.h"
#include <math.h>
#include <stdio.h>

float vec2_magnitude(Vec2 v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}

Vec2 vec2_normalized(Vec2 v) {
	float mag = vec2_magnitude(v);
	if (mag <= 0) return v;

	return (Vec2){
		.x = v.x / mag,
		.y = v.y / mag
	};
}

Vec2 vec2_add(Vec2 a, Vec2 b) {
	return (Vec2){
		.x = a.x + b.x,
		.y = a.y + b.y
	};
}

Vec2 vec2_scale(Vec2 v, float s) {
	return (Vec2) {
		.x = v.x * s,
		.y = v.y * s
	};
}

void vec2_print(Vec2 v) {
	printf("%f, %f\n", v.x, v.y);
}
