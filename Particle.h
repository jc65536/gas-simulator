#ifndef PARTICLE_H
#define PARTICLE_H

#include <set>

#define PARTICLE_RADIUS 5.0
#define MAX_VX 4.0
#define ParticleRef std::reference_wrapper<Particle>
#define HashMap std::map<IntPair, std::vector<ParticleRef>>

class Particle {
public:
    static int nExists;
    int i; // Particle ID
    double x, y, vx, vy;
    const double r = PARTICLE_RADIUS;
    std::set<std::pair<int, int>> keys;

    Particle();
    Particle(int i, double x, double y, double vx, double vy);

    void draw();

    void updatePos();

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