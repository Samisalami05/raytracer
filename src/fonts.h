#ifndef FONTS_H
#define FONTS_H

#include "vec2.h"
#include <ft2build.h>
#include <stdint.h>
#include FT_FREETYPE_H
#include <stdbool.h>

typedef struct {
    uint32_t tex_id;  // ID handle of the glyph texture
    Vec2 size;       // Size of glyph
    Vec2 bearing;    // Offset from baseline to left/top of glyph
    uint32_t advance;    // Offset to advance to next glyph
} Glyph;

typedef struct {
	Glyph glyphs[128];
	int size;
} Font;

bool init_fonts();
void deinit_fonts();
bool load_font(Font* f, const char* path, int font_size);

#endif
