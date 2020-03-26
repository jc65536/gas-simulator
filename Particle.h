#ifndef PARTICLE_H
#define PARTICLE_H

#include <set>

#define PARTICLE_RADIUS 5
#define CELL_SIZE 100
#define MAX_VX 6.0
#define GET_WIN_W glutGet(GLUT_WINDOW_WIDTH)
#define GET_WIN_H glutGet(GLUT_WINDOW_HEIGHT)

class Particle {
public:
    static int n;
    int i; // Particle ID
    double x, y, vx, vy;
    int r;
    std::set<std::pair<int, int>> keys;

    Particle(int r);

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