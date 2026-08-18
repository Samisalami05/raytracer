#ifndef SHADER_H
#define SHADER_H

#include "vec3.h"
#include <glad/glad.h>

#define INVALID_SHADER (ShaderModule){0}

typedef struct {
	unsigned int handle;
} ShaderModule;

ShaderModule LoadShader(char* path, GLenum type);

// Shader program
typedef struct {
	unsigned int handle;
} Shader;

Shader shader_link(ShaderModule* modules, int count);

void shader_set1f(Shader s, const char* name, float v);
void shader_set3f(Shader s, const char* name, Vec3 v);


#endif
