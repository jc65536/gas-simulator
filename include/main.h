#ifndef MAIN_H
#define MAIN_H

#define GLFW_DLL

#include "Particle.h"
#include "defines.h"
#include "nanogui/nanogui.h"
#include <map>
#include <vector>
#include <functional>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

void display(GLFWwindow *window);
void timer(GLFWwindow *window, int fps);
void runNano();

#endif // MAIN _H