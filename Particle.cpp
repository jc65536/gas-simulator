#include "Particle.h"
#include "main.h"
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <windows.h>

int Particle::n, Particle::cellw;

Particle::Particle(double r, double maxv) {
    i = n++;
    x = rand() % (int) (INIT_WIN_W - 2 * r) + r;
    y = rand() % (int) (INIT_WIN_H - 2 * r) + r;
    vx = (rand() % 20 * maxv - 10 * maxv) / 10.0;
    vy = (rand() % 20 * maxv - 10 * maxv) / 10.0;
    this->r = r;
    this->maxv = maxv;
    updateKeys();
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

void Particle::updatePos() {
    if (x - r + vx < 0) {
        if (keyDown('C'))
            vx = (vx - maxv / 10) / 2;
        else if (!keyDown(VK_SHIFT) && keyDown('H'))
            vx = (vx - maxv) / 2;
        vx = -vx;
    }

    if (x + r + vx > GET_WIN_W) {
        if (!keyDown(VK_SHIFT) && keyDown('C'))
            vx = (vx + maxv / 10) / 2;
        else if (keyDown('H'))
            vx = (vx + maxv) / 2;
        vx = -vx;
    }

    if (y - r + vy < 0) {
        if (!keyDown(VK_SHIFT)) {
            if (keyDown('C'))
                vy = (vy - maxv / 10) / 2;
            else if (keyDown('H'))
                vy = (vy - maxv) / 2;
        }
        vy = -vy;
    }

    if (y + r + vy > GET_WIN_H) {
        if (!keyDown(VK_SHIFT)) {
            if (keyDown('C'))
                vy = (vy + maxv / 10) / 2;
            else if (keyDown('H'))
                vy = (vy + maxv * 1.5) / 2;
        }
        vy = -vy;
    }

    x = std::min(std::max(x + vx, (double) r), (double) GET_WIN_W - r);
    y = std::min(std::max(y + vy, (double) r), (double) GET_WIN_H - r);
}

void Particle::updateKeys() {
    keys.clear();
    keys.insert(IntPair((x - r) / cellw, (y - r) / cellw));
    keys.insert(IntPair((x + r) / cellw, (y - r) / cellw));
    keys.insert(IntPair((x + r) / cellw, (y + r) / cellw));
    keys.insert(IntPair((x - r) / cellw, (y + r) / cellw));
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

bool keyDown(int k) {
    return GetKeyState(k) & 0x8000;
}