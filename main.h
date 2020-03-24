#ifndef MAIN_H
#define MAIN_H

#include "Particle.h"
#include <map>
#include <vector>
#include <functional>

#define PI 3.141593
#define CELL_SIZE 100
#define NUM_PARTICLES 200
#define FPS 60
#define IntPair std::pair<int, int>
#define ParticleRef std::reference_wrapper<Particle>
#define HashMap std::map<IntPair, std::vector<ParticleRef>>

#define INIT_WIN_X 200
#define INIT_WIN_Y 100
#define INIT_WIN_W 500
#define INIT_WIN_H 500
#define GET_WIN_W glutGet(GLUT_WINDOW_WIDTH)
#define GET_WIN_H glutGet(GLUT_WINDOW_HEIGHT)

void display();
void reshape(int, int);
void timer(int);
void printMap();
std::vector<ParticleRef> &addParticle(IntPair k, Particle &p);
void evalCollision(Particle &p, Particle &q);

#endif //MAIN _H