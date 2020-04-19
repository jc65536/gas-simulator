#include "Particle.h"
#include <cmath>
#include <iostream>
#include <windows.h>

int Particle::n;

Particle::Particle(double x, double y, int r, double maxv) {
    i = n++;
    this->x = x;
    this->y = y;
    vx = (rand() % 20 * maxv - 10 * maxv) / 10.0;
    vy = (rand() % 20 * maxv - 10 * maxv) / 10.0;
    this->r = r;
    this->maxv = maxv;
}

void Particle::draw() {
    // actual particle
    setColor();
    if (r <= 1) {
        glPointSize(2 * r);
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();
    } else {
        glBegin(GL_POLYGON);
        for (double t = 0; t < 2 * PI; t += 0.05) {
            glVertex2f(x + r * cos(t), y + r * sin(t));
        }
        glEnd();
    }

    /*
    // velocity vector
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x + vx * 10, y + vy * 10);
    glEnd();

    // velocity angle
    glBegin(GL_LINE_STRIP);
    if (vy < 0) {
        for (double t = 0; t > atan2(vy, vx); t -= 0.1) {
            glVertex2f(x + 3 * r / 4 * cos(t), y + 3 * r / 4 * sin(t));
        }
    } else {
        for (double t = 0; t < atan2(vy, vx); t += 0.1) {
            glVertex2f(x + 3 * r / 4 * cos(t), y + 3 * r / 4 * sin(t));
        }
    }
    glEnd();
    */
}

void Particle::setColor() {
    if (pow(vx, 2) + pow(vy, 2) > 2 * pow(maxv, 2)) {
        glColor3f(1.0, 0, 0);
        return;
    }
    double h = (1 - sqrt((pow(vx, 2) + pow(vy, 2)) / (2 * pow(maxv, 2)))) * 270;
    double x = 1 - abs(fmod(h / 60, 2) - 1);
    switch ((int) h / 60) {
    case 0:
        glColor3f(1.0, x, 0);
        break;
    case 1:
        glColor3f(x, 1.0, 0);
        break;
    case 2:
        glColor3f(0, 1.0, x);
        break;
    case 3:
        glColor3f(0, x, 1.0);
        break;
    case 4:
        glColor3f(x, 0, 1.0);
        break;
    case 5:
        glColor3f(1.0, 0, x);
        break;
    }
}