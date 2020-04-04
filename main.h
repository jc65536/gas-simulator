#ifndef MAIN_H
#define MAIN_H

#define GLFW_DLL

#include "Particle.h"
#include "defines.h"
#include <map>
#include <vector>
#include <functional>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

void display(GLFWwindow *window);
void reshape(GLFWwindow *window);
void timer(GLFWwindow *window, int fps);
std::vector<int> &mapParticle(IntPair k, int p);
bool collides(int p, int q);
void evalCollision(int p, int q);

extern std::vector<Particle> all;
extern HashMap map;

#endif //MAIN _H