#ifndef GASSIMULATOR_H
#define GASSIMULATOR_H

#include "GL/glew.h"
#include "Particle.h"
#include "defines.h"
#include "GLFW/glfw3.h"
#include <map>
#include <vector>
#include <mutex>

enum HeatState { COOL = 1, INSULATE = 2, HEAT = 3 };

class GasSimulator {
public:
    std::vector<Particle> particles;
    HashMap map;
    int n, r, w, h, cellw, fps;
    double maxv;
    bool pause = false;
    GLFWwindow *window;
    std::mutex mu;
    HeatState top, left, right, bottom;

    GasSimulator(int n = 200, int r = 8, double maxv = 6, int fps = 60, int x = 200, int y = 100, int w = 1000, int h = 600, int cellw = 50);

    void gasInit(int n, int r, double maxv);
    void update();
    std::vector<int> &mapKey(IntPair k, int i);
    void evalCollision(int i, int j);
    void updatePos(Particle &p);
    void updateKeys(Particle &p);
    void mainloop();
    void printMap();
};

bool collides(Particle &p, Particle &q);

#endif // GASSIMULATOR_H