#ifndef WINDOW_H
#define WINDOW_H

#include "vec2.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef GLFWwindow Window;

bool InitWindow();
bool WindowShouldClose();
void CloseWindow();

void BeginFrame();
void EndFrame();

float Time();
float DeltaTime();

Vec2 window_size();
Window* window();

// Callbacks
void resize_callback(Window* window, int width, int height);
void key_callback(Window* window, int key, int scancode, int action, int mods);
void mousepos_callback(Window* window, double xpos, double ypos);

#endif
