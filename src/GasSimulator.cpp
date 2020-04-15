#include "GasSimulator.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

GasSimulator::GasSimulator(int n, int r, double maxv, int fps, int x, int y, int w, int h, int cellw) {
    glfwInit();

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    window = glfwCreateWindow(w, h, "Gas Simulator", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int w, int h) {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, w, 0, h);
        glMatrixMode(GL_MODELVIEW);
        GasSimulator *self = (GasSimulator *) glfwGetWindowUserPointer(window);
        self->w = w;
        self->h = h;
    });
    glfwSetWindowPos(window, x, y);
    glfwShowWindow(window);
    glfwMakeContextCurrent(window);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    this->w = w;
    this->h = h;

    gasInit(n, r, maxv);
    this->cellw = cellw;
    this->fps = fps;
}

void GasSimulator::gasInit(int n, int r, double maxv) {
    particles.reserve(n);
    particles.clear();
    map.clear();
    this->n = n;
    this->r = r;
    this->maxv = maxv;
    for (int i = 0; i < n; i++) {
        particles.push_back(Particle(r, maxv, w, h));
        for (IntPair k : particles[i].keys) {
            mapParticle(k, particles[i].i);
        }
    }
}

std::vector<int> &GasSimulator::mapParticle(IntPair k, int p) {
    if (map.count(k) > 0) {
        std::vector<int> &bucket = map.find(k)->second;
        bucket.push_back(p);
        return bucket;
    } else {
        std::vector<int> v = {p};
        return map.insert(std::pair<IntPair, std::vector<int>>(k, v)).first->second;
    }
}

void GasSimulator::evalCollision(int p, int q) {
    double vp = sqrt(pow(particles[p].vx, 2) + pow(particles[p].vy, 2));                  // speed (velocity magnitude) of p
    double avp = atan2(particles[p].vy, particles[p].vx);                                 // velocity angle of p
    double atp = atan2(particles[q].y - particles[p].y, particles[q].x - particles[p].x); // transverse axis angle w/ respect to p
    double arp = avp - atp;                                                               // reference angle of p (angle btwn velocity angle & transverse axis)
    double vtp = vp * cos(arp);                                                           // transverse velocity component of p
    double vtpx = vtp * cos(atp);                                                         // x component of transverse velocity component
    double vtpy = vtp * sin(atp);                                                         // y component of transverse velocity component
    double vcp = vp * sin(arp);                                                           // conjugate velocity component of p
    double vcpx = vcp * cos(atp + PI / 2);                                                // x component of conjugate velocity component
    double vcpy = vcp * sin(atp + PI / 2);                                                // y component of conjugate velocity component

    double vq = sqrt(pow(particles[q].vx, 2) + pow(particles[q].vy, 2)); // ditto for q
    double avq = atan2(particles[q].vy, particles[q].vx);
    double atq = atan2(particles[p].y - particles[q].y, particles[p].x - particles[q].x);
    double arq = avq - atq;
    double vtq = vq * cos(arq);
    double vtqx = vtq * cos(atq);
    double vtqy = vtq * sin(atq);
    double vcq = vq * sin(arq);
    double vcqx = vcq * cos(atq + PI / 2);
    double vcqy = vcq * sin(atq + PI / 2);

    particles[p].vx = vcpx + vtqx;
    particles[p].vy = vcpy + vtqy;

    particles[q].vx = vcqx + vtpx;
    particles[q].vy = vcqy + vtpy;

    // Make sure particles aren't overlapping
    particles[q].x = particles[p].x + (particles[p].r + particles[q].r) * cos(atp);
    particles[q].y = particles[p].y + (particles[p].r + particles[q].r) * sin(atp);
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

void GasSimulator::printSpeeds() {
    int a[40] = {0};
    for (Particle &p : particles) {
        double s = sqrt(p.vx * p.vx + p.vy * p.vy);
        a[(int) s]++;
    }
    std::cout << "speed | count |" << std::endl;
    for (int i = 0; i < 20; i++) {
        std::cout << std::setw(i < 9 ? 3 : 2) << i << "-" << i + 1 << " | " << std::setw(5) << a[i] << " |";
        for (int j = 0; j < a[i]; j++) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }
}

void GasSimulator::update() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    //draw
    //std::cout << 3;
    for (Particle &p : particles) {
        //std::cout << 4;
        // deregister all of p's keys so we can update them below
        for (IntPair k : p.keys) {
            std::vector<int> &bucket = map.find(k)->second;
            std::cout << 1;
            bucket.erase(std::find(bucket.begin(), bucket.end(), p.i));
        }
        std::cout << 5;

        p.updateKeys();
        std::cout << 6;

        // need to use a set since each particle has 4 keys, so sometimes they collide more times than desired
        std::set<int> collisions;
        for (IntPair k : p.keys) {
            // check collision with all particles in k's cell
            std::vector<int> &bucket = mapParticle(k, p.i);
            std::cout << 2;
            for (int q : bucket) {
                //std::cout << q;
                if (collides(p.i, q)) {
                    std::cout << 9;
                    if (collisions.insert(q).second) {
                        evalCollision(p.i, q);
                        
                    }
                    std::cout << 0;
                }  
                //std::cout << ".";
            }
            std::cout << std::endl;
        }
        std::cout << 7;

        p.updatePos(w, h);
        p.draw();
        std::cout << 8 << std::endl;
    }
    glfwSwapBuffers(window);
}

bool GasSimulator::collides(int p, int q) {
    return p != q && pow(particles[p].x - particles[q].x, 2) + pow(particles[p].y - particles[q].y, 2) < pow(particles[p].r + particles[q].r, 2);
}

void GasSimulator::mainloop() {
    while (!glfwWindowShouldClose(window)) {

        double period = 1.0 / fps;

        if (glfwGetTime() > period) {
            if (!pause) {
                if (keyDown('P')) {
                    pause = true;
                }
                mu.lock();
                std::cout << "locked by gs" << std::endl;
                update();
                mu.unlock();
                std::cout << "unlocked by gs" << std::endl;
            } else {
                if (keyDown('R')) {
                    pause = false;
                }
            }
            glfwSetTime(0.0);
        }
        glfwPollEvents();
    }
}