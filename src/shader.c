#include "shader.h"
#include <errno.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

static char* read_file(const char* path) {
	FILE* f = fopen(path, "rb");
	if (!f) {
		fprintf(stderr, "Could not open file '%s': %s\n", path, strerror(errno));
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* content = malloc(size + 1);
	fread(content, 1, size, f);
	content[size] = '\0';

	return content;
}

ShaderModule LoadShader(char* path, GLenum type) {
	const char* content = read_file(path);
	if (!path) return INVALID_SHADER;
	
	ShaderModule shader = {0};
	int  success;

	shader.handle = glCreateShader(type);
    glShaderSource(shader.handle, 1, &content, NULL);
    glCompileShader(shader.handle);

	glGetShaderiv(shader.handle, GL_COMPILE_STATUS, &success);
	if(!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader.handle, 512, NULL, infoLog);
		LOG_ERROR("Shader", infoLog);
		return INVALID_SHADER;
	}

	return shader;
}

Shader shader_link(ShaderModule* modules, int count) {
	Shader s = {
		.handle = glCreateProgram(),
	};
	
	for (int i = 0; i < count; i++) {
		if (modules[i].handle == 0) continue;
		glAttachShader(s.handle, modules[i].handle);
	}

	glLinkProgram(s.handle);

	for (int i = 0; i < count; i++) {
		if (modules[i].handle == 0) continue;
		glDeleteShader(modules[i].handle);
	}

	return s;
}

void shader_set1f(Shader s, const char* name, float v) {
	glUseProgram(s.handle);
	int loc = glGetUniformLocation(s.handle, name);
	glUniform1f(loc, v);
}

void shader_set3f(Shader s, const char* name, Vec3 v) {
	glUseProgram(s.handle);
	int loc = glGetUniformLocation(s.handle, name);
	glUniform3f(loc, v.x, v.y, v.z);
}




