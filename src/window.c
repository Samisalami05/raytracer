#include "window.h"
#include "fonts.h"
#include "log.h"
#include <GLFW/glfw3.h>

static Window* win;

void error_callback(int error, const char* description) {
	LOG_ERROR("GLFW", description);
}

bool InitWindow() {
	if (!glfwInit()) {
		LOG_ERROR("GLFW", "Failed to initialize GLFW\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	win = glfwCreateWindow(1280, 720, "window", NULL, NULL);
	if (win == NULL) {
		LOG_ERROR("GLFW", "Failed to create window\n");
		glfwTerminate();
		return false;
	}

	glfwSetErrorCallback(error_callback);
	glfwSetFramebufferSizeCallback(win, resize_callback);
	glfwSetKeyCallback(win, key_callback);
	glfwSetCursorPosCallback(win, mousepos_callback);

	glfwMakeContextCurrent(win);
	//glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_ERROR("GLAD", "Failed to load OpenGL\n");
		return false;
	}

	if (!init_fonts()) return false;

	return true;
}

bool WindowShouldClose() {
	return glfwWindowShouldClose(win);
}

void CloseWindow() {
	deinit_fonts();
	glfwDestroyWindow(win);
	glfwTerminate();
}

static float last;
static float dt = 0;

void BeginFrame() {
	float time = Time();
	dt = time - last;
	last = time;
}

void EndFrame() {
	glfwSwapBuffers(window());
	glfwPollEvents();
}

float Time() {
	return glfwGetTime();
}


float DeltaTime() {
	return dt;
}

Vec2 window_size() {
	int width, height;
	glfwGetWindowSize(win, &width, &height);

	return vec2(width, height);
}

Window* window() {
	return (Window*)win;
}
