#include "vec3.h"
#include <math.h>
#include <stdio.h>

float vec3_magnitude(Vec3 v) {
	return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

void vec3_normalize(Vec3* v) {
	float mag = vec3_magnitude(*v);
	if (mag <= 0) return;

	v->x /= mag;
	v->y /= mag;
	v->z /= mag;
}

Vec3 vec3_normalized(Vec3 v) {
	float mag = vec3_magnitude(v);
	if (mag <= 0) return v;

	return (Vec3){
		.x = v.x / mag,
		.y = v.y / mag,
		.z = v.z / mag,
	};
}

float vec3_dot(Vec3 v1, Vec3 v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

Vec3 vec3_inverse(Vec3 v) {
	return (Vec3){
		.x = -v.x,
		.y = -v.y,
		.z = -v.z
	};
}

Vec3 vec3_cross(Vec3 v1, Vec3 v2) {
	return (Vec3) {
		.x = v1.y*v2.z - v1.z*v2.y,
		.y = v1.z*v2.x - v1.x*v2.z,
		.z = v1.x*v2.y - v1.y*v2.x
	};
}

float vec3_angle(Vec3 v1, Vec3 v2) {
    float result = 0.0f;

    Vec3 cross = vec3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
    float len = sqrtf(cross.x*cross.x + cross.y*cross.y + cross.z*cross.z);
    float dot = (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
    result = atan2f(len, dot);

    return result;
}

// Rotates a vector around an axis
Vec3 vec3_rotate_euler(Vec3 v, Vec3 axis, float angle)
{
    // Using Euler-Rodrigues Formula
    // Ref.: https://en.wikipedia.org/w/index.php?title=Euler%E2%80%93Rodrigues_formula

    Vec3 result = v;

    // Vector3Normalize(axis);
    float length = sqrtf(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);
    if (length == 0.0f) length = 1.0f;
    float ilength = 1.0f/length;
    axis.x *= ilength;
    axis.y *= ilength;
    axis.z *= ilength;

    angle /= 2.0f;
    float a = sinf(angle);
    float b = axis.x*a;
    float c = axis.y*a;
    float d = axis.z*a;
    a = cosf(angle);
    Vec3 w = vec3(b, c, d);

    // Vector3CrossProduct(w, v)
    Vec3 wv = vec3(w.y*v.z - w.z*v.y, w.z*v.x - w.x*v.z, w.x*v.y - w.y*v.x);

    // Vector3CrossProduct(w, wv)
    Vec3 wwv = vec3(w.y*wv.z - w.z*wv.y, w.z*wv.x - w.x*wv.z, w.x*wv.y - w.y*wv.x);

    // Vector3Scale(wv, 2*a)
    a *= 2;
    wv.x *= a;
    wv.y *= a;
    wv.z *= a;

    // Vector3Scale(wwv, 2)
    wwv.x *= 2;
    wwv.y *= 2;
    wwv.z *= 2;

    result.x += wv.x;
    result.y += wv.y;
    result.z += wv.z;

    result.x += wwv.x;
    result.y += wwv.y;
    result.z += wwv.z;

    return result;
}

Vec3 vec3_add(Vec3 v1, Vec3 v2) {
	return (Vec3) {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y,
		.z = v1.z + v2.z
	};
}

Vec3 vec3_sub(Vec3 v1, Vec3 v2) {
	return (Vec3) {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y,
		.z = v1.z - v2.z
	};
}

Vec3 vec3_mul(Vec3 v1, Vec3 v2) {
	return (Vec3) {
		.x = v1.x * v2.x,
		.y = v1.y * v2.y,
		.z = v1.z * v2.z
	};
}

Vec3 vec3_div(Vec3 v1, Vec3 v2) {
	return (Vec3) {
		.x = v1.x / v2.x,
		.y = v1.y / v2.y,
		.z = v1.z / v2.z
	};
}

Vec3 vec3_add_f(Vec3 v1, float v2) {
	return (Vec3) {
		.x = v1.x + v2,
		.y = v1.y + v2,
		.z = v1.z + v2
	};
}

Vec3 vec3_sub_f(Vec3 v1, float v2) {
	return (Vec3) {
		.x = v1.x - v2,
		.y = v1.y - v2,
		.z = v1.z - v2
	};
}

Vec3 vec3_scale(Vec3 v, float s) {
	return (Vec3) {
		.x = v.x * s,
		.y = v.y * s,
		.z = v.z * s
	};
}

Vec3 vec3_div_f(Vec3 v1, float v2) {
	return (Vec3) {
		.x = v1.x / v2,
		.y = v1.y / v2,
		.z = v1.z / v2
	};
}

void vec3_print(Vec3 v) {
	printf("%f, %f, %f\n", v.x, v.y, v.z);
}
