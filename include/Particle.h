#ifndef PARTICLE_H
#define PARTICLE_H

#include "defines.h"
#include "GLFW/glfw3.h"
#include <set>

#define MAX_VX 6.0

class Particle {
public:
    static int n;
    int i, r;
    double x, y, vx, vy, maxv;
    std::set<IntPair> keys;

    Particle(double x, double y, int r, double maxv);

    void draw();
    void setColor();

    bool operator==(Particle &p) {
        return i == p.i;
    }

    bool operator!=(Particle &p) {
        return i != p.i;
    }
};

#endif //PARTICLE_H