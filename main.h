#ifndef MAIN_H
#define MAIN_H

#include "Particle.h"
#include <map>
#include <vector>
#include <functional>

#define PI 3.141593
#define IntPair std::pair<int, int>
#define ParticleRef std::reference_wrapper<Particle>
#define HashMap std::map<IntPair, std::vector<int>>

#define INIT_WIN_X 200
#define INIT_WIN_Y 100
#define INIT_WIN_W 500
#define INIT_WIN_H 500

void display();
void reshape(int, int);
void timer(int);
void printMap();
void printSpeeds();
std::vector<int> &mapParticle(IntPair k, int p);
bool collides(int p, int q);
void evalCollision(int p, int q);

#endif //MAIN _H