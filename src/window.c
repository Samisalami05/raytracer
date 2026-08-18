#include "window.h"
#include "log.h"
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description) {
	LOG_ERROR("GLFW", description);
}

Window* InitWindow() {
	if (!glfwInit()) {
		LOG_ERROR("GLFW", "Failed to initialize GLFW\n");
		return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "window", NULL, NULL);
	if (window == NULL) {
		LOG_ERROR("GLFW", "Failed to create window\n");
		glfwTerminate();
		return NULL;
	}

	glfwSetErrorCallback(error_callback);
	glfwSetFramebufferSizeCallback(window, resize_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mousepos_callback);

	glfwMakeContextCurrent(window);
	//glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		LOG_ERROR("GLAD", "Failed to load OpenGL\n");
		return NULL;
	}

	return window;
}

bool WindowShouldClose(Window* window) {
	return glfwWindowShouldClose(window);
}

void CloseWindow(Window* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}
