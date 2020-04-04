#ifndef PARTICLE_H
#define PARTICLE_H

#include "defines.h"
#include <set>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define MAX_VX 6.0

class Particle {
public:
    static int n, cellw;
    int i, r; // Particle ID
    double x, y, vx, vy, maxv;
    std::set<IntPair> keys;

    Particle(int r, double maxv);

    void draw();

    void updatePos(GLFWwindow *window);

    void updateKeys();

    void setColor();

    bool operator==(Particle &p) {
        return i == p.i;
    }

    bool operator!=(Particle &p) {
        return i != p.i;
    }
};

// Util functions
bool keyDown(int k);
bool collides(Particle &p, Particle &q);

#endif //PARTICLE_H