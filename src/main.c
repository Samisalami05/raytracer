#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "log.h"
#include "shader.h"
#include "vec3.h"
#include "window.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

static float vertices[] = {
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  // top right
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom left
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f   // top left 
};

static unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

unsigned int texture;
int screen_width = 512;
int screen_height = 512;

Camera cam = {
	.pos = vec3(0, 0, 0),
	.target = vec3(0.0f, 0.0f, 1.0f),
	.fov = 90.0f,
};

void resize_callback(Window* window, int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, width, height);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	//printf("%d, %d\n", width, height);
}

void key_callback(Window* window, int key, int scancode, int action, int mods) {
	
}

Vec2 last = vec2(0, 0);

void mousepos_callback(Window* window, double xpos, double ypos) {
	Vec2 delta = vec2(xpos - last.x, ypos - last.y);
	
	camera_pitch(&cam, -delta.y / 500);
	camera_yaw(&cam, -delta.x / 500);

	last.x = xpos;
	last.y = ypos;
}

static GLenum texture_format(int channel_count) {
	switch (channel_count) {
		case 1: return GL_R;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		default:
			fprintf(stderr, "No texture format available for channel count %d\n", channel_count);
	}

	return GL_RGB; // Default
}

unsigned int cubemap_init(const char** faces) {
	unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	
	int channel_count, width, height;
	for (int i = 0; i < 6; i++) {
		uint8_t* data = stbi_load(faces[i], &width, &height, &channel_count, 0);
		if (!data) {
			fprintf(stderr, "cubemap: Failed to load image %s\n", faces[i]);
			stbi_image_free(data);
			continue;
		}
		
		GLenum format = texture_format(channel_count);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
				0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data
		);

		stbi_image_free(data);
	}

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	return id;
}

int main(void) {
	Window *window = InitWindow();
	if (!window) return 1;

	const char** faces = (const char*[]){
		"assets/skybox/right.jpg",
		"assets/skybox/left.jpg", 
		"assets/skybox/top.jpg",
		"assets/skybox/bottom.jpg",
		"assets/skybox/front.jpg",

		"assets/skybox/back.jpg",
		NULL
	};
	unsigned int skybox = cubemap_init(faces);

	ShaderModule mods[2];
	mods[0] = LoadShader("vert.glsl", GL_VERTEX_SHADER);
	mods[1] = LoadShader("frag.glsl", GL_FRAGMENT_SHADER);

	Shader quad = shader_link(mods, 2);

	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLuint vao;
	glGenVertexArrays(1, &vao);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glUseProgram(quad.handle);
	glBindVertexArray(vao);

	ShaderModule compute = LoadShader("raytrace.glsl", GL_COMPUTE_SHADER);
	Shader raytrace = shader_link(&compute, 1);
	
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, screen_width, screen_height, 0, GL_RGBA, 
				 GL_FLOAT, NULL);

	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	float last = 0;
	while (!WindowShouldClose(window)) {
		float time = glfwGetTime();
		float deltatime = time - last;
		last = time;

		printf("\rfps: %.1f           ", 1 / deltatime);

		float speed = 4 * deltatime;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera_move_forward(&cam, speed);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera_move_forward(&cam, -speed);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera_move_right(&cam, speed);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera_move_right(&cam, -speed);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera_move_up(&cam, speed);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera_move_up(&cam, -speed);



		glUseProgram(raytrace.handle);
		shader_set1f(raytrace, "time", glfwGetTime());
		shader_set3f(raytrace, "cam.pos", cam.pos);
		shader_set3f(raytrace, "cam.target", cam.target);
		shader_set1f(raytrace, "cam.fov", cam.fov);

		glDispatchCompute(screen_width / 8, screen_height / 8, 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(quad.handle);
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		
		unsigned int loc = glGetUniformLocation(quad.handle, "tex");
		glUniform1i(loc, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	CloseWindow(window);
}
