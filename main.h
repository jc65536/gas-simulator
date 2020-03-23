#ifndef MAIN_H
#define MAIN_H

#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <functional>
#include <algorithm>
#include <windows.h>

#define PI 3.141593
#define CELL_SIZE 100
#define MAX_VX 40.0
#define NUM_PARTICLES 200
#define RADIUS 5
#define FPS 60
#define IntPair std::pair<int, int>
#define ParticleRef std::reference_wrapper<Particle>
#define HashMap std::map<IntPair, std::vector<ParticleRef>>
#define WINDOW_W glutGet(GLUT_WINDOW_WIDTH)
#define WINDOW_H glutGet(GLUT_WINDOW_HEIGHT)

#include "Particle.h"

void init();
void display();
void reshape(int, int);
void timer(int);
void printMap();
std::vector<ParticleRef> &addParticle(IntPair k, Particle &p);
void evalCollision(Particle &p, Particle &q);

#endif //MAIN _H