#ifndef GASSIMULATOR_H
#define GASSIMULATOR_H

#include "Particle.h"
#include "defines.h"
#include "GLFW/glfw3.h"
#include <map>
#include <vector>
#include <mutex>

class GasSimulator {
public:
    std::vector<Particle> particles;
    HashMap map;
    int n, r, w, h, cellw, fps;
    double maxv;
    bool pause = false;
    GLFWwindow *window;
    std::mutex mu;

    GasSimulator(int n = 2, int r = 10, double maxv = 10, int fps = 1, int x = 200, int y = 100, int w = 800, int h = 500, int cellw = 50);

    void gasInit(int n, int r, double maxv);
    void update();
    bool collides(int p, int q);
    std::vector<int> &mapParticle(IntPair k, int p);
    void evalCollision(int p, int q);
    void printMap();
    void printSpeeds();
    void mainloop();
};

#endif // GASSIMULATOR_H