/*
TODO:
 - make collision detection based on intersecting path lines
 - optimize!!
 - add buttons to heat/cool
 - add data visualizations
 - add customizable runs
 - make 3d???
*/

#include "main.h"
#include "util.h"
#include <algorithm>
#include <cmath>

bool operator<(const Particle &p, const Particle &q) {
    return p.i < q.i;
}

std::vector<Particle> all;
HashMap map;

bool pause = false;
int n = 800; // Goal: simulate 10k particles at 60 fps!
int r = 8;
int fps = 60;
int cellw = 15;
double maxv = 8.0; // in one dimension, actual max v is maxv * sqrt(2)

int main(int argc, char *argv[]) {
    all.reserve(n);
    Particle::cellw = cellw;
    for (int i = 0; i < n; i++) {
        all.push_back(Particle(r, maxv));
        for (IntPair k : all[i].keys) {
            mapParticle(k, i);
        }
    }

    if (!glfwInit()) exit(1);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow *window = glfwCreateWindow(INIT_WIN_W, INIT_WIN_H, "Gas Simulator", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);

    glfwSetWindowPos(window, INIT_WIN_X, INIT_WIN_Y);
    glfwShowWindow(window);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        reshape(window);
        timer(window, fps);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void display(GLFWwindow *window) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //draw
    for (Particle &p : all) {
        // deregister all of p's keys so we can update them below
        for (IntPair k : p.keys) {
            std::vector<int> &bucket = map.find(k)->second;
            bucket.erase(std::find(bucket.begin(), bucket.end(), p.i));
        }

        p.updateKeys();

        // need to use a set since each particle has 4 keys, so sometimes they collide more times than desired
        std::set<int> collisions;
        for (IntPair k : p.keys) {
            // check collision with all particles in k's cell
            std::vector<int> &bucket = mapParticle(k, p.i);
            for (int q : bucket) {
                if (collides(p.i, q) && collisions.insert(q).second) evalCollision(p.i, q);
            }
        }

        p.updatePos(window);
        p.draw();
    }
    glfwSwapBuffers(window);
}

void reshape(GLFWwindow *window) {
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

void timer(GLFWwindow *window, int fps) {
    double period = 1.0 / fps;
    print(glfwGetTime());

    if (glfwGetTime() > period) {
        if (!pause) {
            if (keyDown('P')) {
                print("");
                printSpeeds();
                pause = true;
            }
            display(window);
        } else {
            if (keyDown('R')) {
                pause = false;
            }
        }
        glfwSetTime(0.0);
    }
}

std::vector<int> &mapParticle(IntPair k, int p) {
    if (map.count(k) > 0) {
        std::vector<int> &bucket = map.find(k)->second;
        bucket.push_back(p);
        return bucket;
    } else {
        std::vector<int> v = {p};
        return map.insert(std::pair<IntPair, std::vector<int>>(k, v)).first->second;
    }
}

void evalCollision(int p, int q) {
    double vp = sqrt(pow(all[p].vx, 2) + pow(all[p].vy, 2));      // speed (velocity magnitude) of p
    double avp = atan2(all[p].vy, all[p].vx);                     // velocity angle of p
    double atp = atan2(all[q].y - all[p].y, all[q].x - all[p].x); // transverse axis angle w/ respect to p
    double arp = avp - atp;                                       // reference angle of p (angle btwn velocity angle & transverse axis)
    double vtp = vp * cos(arp);                                   // transverse velocity component of p
    double vtpx = vtp * cos(atp);                                 // x component of transverse velocity component
    double vtpy = vtp * sin(atp);                                 // y component of transverse velocity component
    double vcp = vp * sin(arp);                                   // conjugate velocity component of p
    double vcpx = vcp * cos(atp + PI / 2);                        // x component of conjugate velocity component
    double vcpy = vcp * sin(atp + PI / 2);                        // y component of conjugate velocity component

    double vq = sqrt(pow(all[q].vx, 2) + pow(all[q].vy, 2)); // ditto for q
    double avq = atan2(all[q].vy, all[q].vx);
    double atq = atan2(all[p].y - all[q].y, all[p].x - all[q].x);
    double arq = avq - atq;
    double vtq = vq * cos(arq);
    double vtqx = vtq * cos(atq);
    double vtqy = vtq * sin(atq);
    double vcq = vq * sin(arq);
    double vcqx = vcq * cos(atq + PI / 2);
    double vcqy = vcq * sin(atq + PI / 2);

    all[p].vx = vcpx + vtqx;
    all[p].vy = vcpy + vtqy;

    all[q].vx = vcqx + vtpx;
    all[q].vy = vcqy + vtpy;

    // Make sure particles aren't overlapping
    all[q].x = all[p].x + (all[p].r + all[q].r) * cos(atp);
    all[q].y = all[p].y + (all[p].r + all[q].r) * sin(atp);
}

bool collides(int p, int q) {
    return p != q && pow(all[p].x - all[q].x, 2) + pow(all[p].y - all[q].y, 2) < pow(all[p].r + all[q].r, 2);
}