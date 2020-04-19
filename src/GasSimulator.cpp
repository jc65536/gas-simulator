#include "GasSimulator.h"
#include <GL/glu.h>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <windows.h>

GasSimulator::GasSimulator(int n, int r, double maxv, int fps, int x, int y, int w, int h, int cellw) {
    // Get the GLFW working
    glfwInit();
    glewInit();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    window = glfwCreateWindow(w, h, "Gas Simulator", nullptr, nullptr);

    glfwSetWindowUserPointer(window, this); // to use in the following callback:
    auto resize = [](GLFWwindow *window, int w, int h) {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, w, 0, h);
        glMatrixMode(GL_MODELVIEW);
        GasSimulator *self = (GasSimulator *) glfwGetWindowUserPointer(window);
        self->w = w;
        self->h = h;
    };
    glfwSetFramebufferSizeCallback(window, resize);

    glfwSetWindowPos(window, x, y);
    glfwShowWindow(window);
    glfwMakeContextCurrent(window);
    resize(window, w, h);

    this->cellw = cellw;
    this->fps = fps;
    top = left = right = bottom = INSULATE;
    gasInit(n, r, maxv);
}

void GasSimulator::gasInit(int n, int r, double maxv) {
    this->n = n;
    this->r = r;
    this->maxv = maxv;
    particles.clear();
    particles.reserve(n);
    Particle::n = 0;
    map.clear();
    for (int i = 0; i < n; i++) {
        int x = rand() % (w - 2 * r) + r;
        int y = rand() % (h - 2 * r) + r;
        particles.push_back(Particle(x, y, r, maxv));
        updateKeys(particles[i]);
        for (IntPair k : particles[i].keys) {
            mapKey(k, particles[i].i);
        }
    }
}

void GasSimulator::update() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //draw
    for (Particle &p : particles) {
        // delete all of p's keys so we can update them below
        for (IntPair k : p.keys) {
            auto u = map.find(k);
            std::vector<int> &bucket = u->second;
            bucket.erase(std::find(bucket.begin(), bucket.end(), p.i));
        }

        updateKeys(p);

        // need to use a set since each particle has 4 keys, so keys may overlap, especially on the edge of cells
        std::set<int> collisions;
        for (IntPair k : p.keys) {
            // check collision with all particles in k's cell
            std::vector<int> &bucket = mapKey(k, p.i);
            for (int q : bucket) {
                if (collides(p, particles[q]) && collisions.insert(q).second) {
                    evalCollision(p.i, q);
                }
            }
        }

        updatePos(p);
        p.draw();
    }

    glfwSwapBuffers(window);
}

std::vector<int> &GasSimulator::mapKey(IntPair k, int i) {
    if (map.count(k) > 0) {
        std::vector<int> &bucket = map.find(k)->second;
        bucket.push_back(i);
        return bucket;
    } else {
        std::vector<int> v = {i};
        return map.insert(std::pair<IntPair, std::vector<int>>(k, v)).first->second;
    }
}

void GasSimulator::evalCollision(int i, int j) {
    Particle &p = particles[i], &q = particles[j];
    double vp = sqrt(pow(p.vx, 2) + pow(p.vy, 2)); // speed (velocity magnitude) of p
    double avp = atan2(p.vy, p.vx);                // velocity angle of p
    double atp = atan2(q.y - p.y, q.x - p.x);      // transverse axis angle w/ respect to p
    double arp = avp - atp;                        // reference angle of p (angle btwn velocity angle & transverse axis)
    double vtp = vp * cos(arp);                    // transverse velocity component of p
    double vtpx = vtp * cos(atp);                  // x component of transverse velocity component
    double vtpy = vtp * sin(atp);                  // y component of transverse velocity component
    double vcp = vp * sin(arp);                    // conjugate velocity component of p
    double vcpx = vcp * cos(atp + PI / 2);         // x component of conjugate velocity component
    double vcpy = vcp * sin(atp + PI / 2);         // y component of conjugate velocity component

    double vq = sqrt(pow(q.vx, 2) + pow(q.vy, 2)); // ditto for q
    double avq = atan2(q.vy, q.vx);
    double atq = atan2(p.y - q.y, p.x - q.x);
    double arq = avq - atq;
    double vtq = vq * cos(arq);
    double vtqx = vtq * cos(atq);
    double vtqy = vtq * sin(atq);
    double vcq = vq * sin(arq);
    double vcqx = vcq * cos(atq + PI / 2);
    double vcqy = vcq * sin(atq + PI / 2);

    p.vx = vcpx + vtqx;
    p.vy = vcpy + vtqy;

    q.vx = vcqx + vtpx;
    q.vy = vcqy + vtpy;

    // Make sure particles aren't overlapping
    q.x = p.x + (p.r + q.r) * cos(atp);
    q.y = p.y + (p.r + q.r) * sin(atp);
}

void GasSimulator::updatePos(Particle &p) {
    double &x = p.x, &y = p.y;
    double &vx = p.vx, &vy = p.vy;

    if (x - r + vx < 0) {
        switch (left) {
        case COOL:
            vx = -vx / 2;
            break;
        case INSULATE:
            vx = -vx;
            break;
        case HEAT:
            vx = -(vx - maxv) / 2;
        }
    }

    if (x + r + vx > w) {
        switch (right) {
        case COOL:
            vx = -vx / 2;
            break;
        case INSULATE:
            vx = -vx;
            break;
        case HEAT:
            vx = -(vx + maxv) / 2;
        }
    }

    if (y - r + vy < 0) {
        switch (bottom) {
        case COOL:
            vy = -vy / 2;
            break;
        case INSULATE:
            vy = -vy;
            break;
        case HEAT:
            vy = -(vy - maxv) / 2;
        }
    }
    
    if (y + r + vy > h) {
        switch (top) {
        case COOL:
            vy = -vy / 2;
            break;
        case INSULATE:
            vy = -vy;
            break;
        case HEAT:
            vy = -(vy + maxv) / 2;
        }
    }

    x = std::min(std::max(x + vx, (double) r), (double) w - r);
    y = std::min(std::max(y + vy, (double) r), (double) h - r);
}

void GasSimulator::updateKeys(Particle &p) {
    int x = p.x, y = p.y;
    p.keys.clear();
    p.keys.insert(IntPair((x - r) / cellw, (y - r) / cellw));
    p.keys.insert(IntPair((x + r) / cellw, (y - r) / cellw));
    p.keys.insert(IntPair((x + r) / cellw, (y + r) / cellw));
    p.keys.insert(IntPair((x - r) / cellw, (y + r) / cellw));
}

void GasSimulator::mainloop() {
    while (!glfwWindowShouldClose(window)) {
        double period = 1.0 / fps;
        if (glfwGetTime() > period) {
            mu.lock();
            update();
            mu.unlock();
            glfwSetTime(0.0);
        }
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(0);
}

void GasSimulator::printMap() {
    for (auto &e : map) {
        std::cout << "(" << e.first.first << ", " << e.first.second << ") : ";
        for (int f : e.second) {
            std::cout << f << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool collides(Particle &p, Particle &q) {
    return p != q && pow(p.x - q.x, 2) + pow(p.y - q.y, 2) < pow(p.r + q.r, 2);
}