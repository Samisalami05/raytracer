#ifndef COLOR_H
#define COLOR_H

#include "vec4.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

typedef union {
	struct {
		uint32_t r : 8;
		uint32_t g : 8;
		uint32_t b : 8;
		uint32_t a : 8;
	};
	uint32_t raw;
} Color;

#define color(rv, gv, bv, av) (Color){.r = rv, .g = gv, .b = bv, .a = av}
#define hex(rawv) (Color){.raw = rawv}

#define WHITE     color(255, 255, 255, 255)
#define GRAY      color(200, 200, 200, 255)
#define DARK_GRAY color(50, 50, 50, 255)
#define BLACK     color(0, 0, 0, 255)
#define RED       color(255, 0, 0, 255)
#define GREEN     color(0, 255, 0, 255)
#define BLUE      color(0, 0, 255, 255)

static inline Vec4 color_as_vec(Color col) {
	return vec4(
		(float)col.r / 255.0f,
		(float)col.g / 255.0f,
		(float)col.b / 255.0f,
		(float)col.a / 255.0f
	);
}

#endif
