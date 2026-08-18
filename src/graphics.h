#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "color.h"
#include "fonts.h"
#include "vec2.h"

bool draw_text(Font font, Color col, Vec2 pos, float scale, const char* fmt, ...);

#endif
