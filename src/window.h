#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef GLFWwindow Window;

Window* InitWindow();
bool WindowShouldClose(Window* window);
void CloseWindow(Window* window);

// Callbacks
void resize_callback(Window* window, int width, int height);
void key_callback(Window* window, int key, int scancode, int action, int mods);
void mousepos_callback(Window* window, double xpos, double ypos);

#endif
