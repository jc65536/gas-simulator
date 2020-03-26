#include "main.h"
#include <GL/glut.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

bool operator<(const Particle &p, const Particle &q) {
    return p.i < q.i;
}

std::ofstream fout("output.txt");
std::vector<Particle> all;
HashMap m;
bool flag = true;

int main(int argc, char *argv[]) {
    int n = 2000;
    int r = 5;
    int fps = 60;

    all.reserve(n);
    for (int i = 0; i < n; i++) {
        all.push_back(Particle(r));
        for (IntPair k : all[i].keys) {
            mapParticle(k, i);
        }
    }

    printMap();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowPosition(INIT_WIN_X, INIT_WIN_Y);
    glutInitWindowSize(INIT_WIN_W, INIT_WIN_H);

    glutCreateWindow("Gas Simulator");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, fps);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glutMainLoop();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //draw
    for (Particle &p : all) {
        // deregister all of p's keys so we can update them below
        for (IntPair k : p.keys) {
            std::vector<int> &bucket = m.find(k)->second;
            bucket.erase(std::find(bucket.begin(), bucket.end(), p.i));
        }

        p.updateKeys();

        // need to use a set since each particle has 4 keys, so sometimes they collide more times than desired
        std::set<int> collisions;
        for (IntPair k : p.keys) {
            // check collision with all particles in k's cell
            std::vector<int> &bucket = mapParticle(k, p.i);
            for (int q : bucket) {
                if (collides(p.i, q))
                    collisions.insert(q);
            }
        }

        for (int q : collisions) {
            evalCollision(p.i, q);
        }

        p.updatePos();
        p.draw();
    }

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int fps) {
    if (flag) {
        if (keyDown('P')) {
            fout << std::endl;
            printSpeeds();
            flag = false;
        }
        glutPostRedisplay();
    } else {
        if (keyDown('R')) {
            flag = true;
        }
    }
    glutTimerFunc(1000 / fps, timer, fps);
}

std::vector<int> &mapParticle(IntPair k, int p) {
    if (m.count(k) > 0) {
        std::vector<int> &bucket = m.find(k)->second;
        bucket.push_back(p);
        return bucket;
    } else {
        std::vector<int> v = {p};
        return m.insert(std::pair<IntPair, std::vector<int>>(k, v)).first->second;
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

// Util
void printMap() {
    for (auto &e : m) {
        fout << "(" << e.first.first << ", " << e.first.second << ") : ";
        for (int f : e.second) {
            fout << f << ", ";
        }
        fout << std::endl;
    }
    fout << std::endl;
}

void printSpeeds() {
    int a[40] = {0};
    for (Particle &p : all) {
        double s = sqrt(p.vx * p.vx + p.vy * p.vy);
        a[(int) s]++;
    }
    fout << "speed | count |" << std::endl;
    for (int i = 0; i < 20; i++) {
        fout << std::setw(i < 9 ? 3 : 2) << i << "-" << i + 1 << " | " << std::setw(5) << a[i] << " |";
        for (int j = 0; j < a[i]; j++) {
            fout << "*";
        }
        fout << std::endl;
    }
}

bool collides(int p, int q) {
    return p != q && pow(all[p].x - all[q].x, 2) + pow(all[p].y - all[q].y, 2) < pow(all[p].r + all[q].r, 2);
}