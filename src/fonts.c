#include "fonts.h"
#include "freetype/freetype.h"
#include "log.h"
#include <glad/glad.h>

static FT_Library ft;

bool init_fonts() {
	if (FT_Init_FreeType(&ft)) {
		LOG_ERROR("FREETYPE", "Failed to initialize freetype");
		return false;
	}
	return true;
}

void deinit_fonts() {
	FT_Done_FreeType(ft);
}

bool load_font(Font* f, const char* path, int font_size) {
	f->size = font_size;

	FT_Face face;
	if (FT_New_Face(ft, path, 0, &face))
	{
		LOG_ERROR("FREETYPE", "Failed to load font");
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0, font_size);  

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
		LOG_ERROR("FREETYPE", "Failed to load Glyph");  
		return false;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (int i = 0; i < 128; i++) {
		// load character glyph 
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			LOG_ERROR("FREETYPE", "Failed to load Glyph");
			continue;
		}
		
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);
		
		// now store character for later use
		Glyph glyph = {
			texture, 
			vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		f->glyphs[i] = glyph;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	FT_Done_Face(face);

	return true;
}
