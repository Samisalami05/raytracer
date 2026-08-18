#include "graphics.h"
#include "color.h"
#include "fonts.h"
#include "shader.h"
#include "vec2.h"
#include "vec4.h"
#include "window.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>


static bool initialized = false;
static unsigned int vao, vbo;
static Shader s = {0};

static void init() {
	if (initialized) return;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	ShaderModule mods[] = {
		LoadShader("assets/shaders/text_vert.glsl", GL_VERTEX_SHADER),
		LoadShader("assets/shaders/text_frag.glsl", GL_FRAGMENT_SHADER)
	};
	s = shader_link(mods, 2);

	initialized = true;
}

bool draw_text(Font font, Color col, Vec2 pos, float scale, const char* fmt, ...) {
	init();

	char buf[1024];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, 1024, fmt, args);
	va_end(args);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    glUseProgram(s.handle);

	Vec4 color = color_as_vec(col);
    glUniform3f(glGetUniformLocation(s.handle, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(s.handle, "text"), 0);

    glUniform2f(glGetUniformLocation(s.handle, "pos"), 0.0f, 0.0f);
    glUniform2f(glGetUniformLocation(s.handle, "window"), window_size().x, window_size().y);
    glBindVertexArray(vao);

	Vec2 win = window_size();

	pos.x -= win.x / 2;
	pos.y -= scale - win.y / 2;

	scale /= font.size;

	const char* str = buf;

    for (;*str; str++)
    {
		if (*str > 127) continue;
        Glyph glyph = font.glyphs[(int)*str];

        float xpos = pos.x + glyph.bearing.x * scale;
        float ypos = pos.y - (glyph.size.y - glyph.bearing.y) * scale;

		xpos /= win.x / 2;
		ypos /= win.y / 2;

        float w = glyph.size.x / win.x * 2 * scale;
        float h = glyph.size.y / win.y * 2 * scale;
        
		float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        glBindTexture(GL_TEXTURE_2D, glyph.tex_id);
        
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
		glDrawArrays(GL_TRIANGLES, 0, 6);
        
		pos.x += (glyph.advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}
