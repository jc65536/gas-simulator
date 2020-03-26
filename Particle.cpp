#include "Particle.h"
#include "main.h"
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <windows.h>

int Particle::n;

Particle::Particle(int rad) {
    i = n++;
    x = rand() % (int) (INIT_WIN_W - 2 * PARTICLE_RADIUS) + PARTICLE_RADIUS;
    y = rand() % (int) (INIT_WIN_H - 2 * PARTICLE_RADIUS) + PARTICLE_RADIUS;
    vx = (rand() % 20 * MAX_VX - 10 * MAX_VX) / 10.0;
    vy = (rand() % 20 * MAX_VX - 10 * MAX_VX) / 10.0;
    r = rad;
    updateKeys();
}

void Particle::draw() {
    // actual particle
    glBegin(GL_POLYGON);
    setColor();
    for (double t = 0; t < 2 * PI; t += 0.05) {
        glVertex2f(x + r * cos(t), y + r * sin(t));
    }
    glEnd();

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
            vx = (vx - MAX_VX / 10) / 2;
        else if (!keyDown(VK_SHIFT) && keyDown('H'))
            vx = (vx - MAX_VX * 2) / 2;
        vx = -vx;
    }
    
    if (x + r + vx > GET_WIN_W) {
        if (!keyDown(VK_SHIFT) && keyDown('C'))
            vx = (vx + MAX_VX / 10) / 2;
        else if (keyDown('H'))
            vx = (vx + MAX_VX * 1.5) / 2;
        vx = -vx;
    }

    if (y - r + vy < 0) {
        if (!keyDown(VK_SHIFT)) {
            if (keyDown('C'))
                vy = (vy - MAX_VX / 10) / 2;
            else if (keyDown('H'))
                vy = (vy - MAX_VX * 1.5) / 2;
        }
        vy = -vy;
    }
    
    if (y + r + vy > GET_WIN_H) {
        if (!keyDown(VK_SHIFT)) {
            if (keyDown('C'))
                vy = (vy + MAX_VX / 10) / 2;
            else if (keyDown('H'))
                vy = (vy + MAX_VX * 1.5) / 2;
        }
        vy = -vy;
    }

    x = std::min(std::max(x + vx, (double) r), (double) GET_WIN_W - r);
    y = std::min(std::max(y + vy, (double) r), (double) GET_WIN_H - r);
}

void Particle::updateKeys() {
    keys.clear();
    keys.insert(IntPair((x - r) / CELL_SIZE, (y - r) / CELL_SIZE));
    keys.insert(IntPair((x + r) / CELL_SIZE, (y - r) / CELL_SIZE));
    keys.insert(IntPair((x + r) / CELL_SIZE, (y + r) / CELL_SIZE));
    keys.insert(IntPair((x - r) / CELL_SIZE, (y + r) / CELL_SIZE));
}

void Particle::setColor() {
    if (vx * vx + vy * vy > 2 * MAX_VX * MAX_VX) {
        glColor3f(1.0, 0, 0);
        return;
    }
    double h = (1 - sqrt((vx * vx + vy * vy) / (2 * MAX_VX * MAX_VX))) * 270;
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