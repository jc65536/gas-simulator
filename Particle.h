#ifndef PARTICLE_H
#define PARTICLE_H

#include "main.h"

class Particle {
public:
    int i;
    double x, y, r = RADIUS, vx, vy;
    std::set<IntPair> keys;

    Particle() {}

    Particle(int i, double x, double y, double vx, double vy);

    void draw();

    void update();

    bool collides(Particle &p);

    void updateKeys();

    void setColor();

    void heat(int d);
    void cool(int d);

    bool operator==(Particle &p) {
        return i == p.i;
    }

    bool operator!=(Particle &p) {
        return i != p.i;
    }
};

#endif //PARTICLE_H