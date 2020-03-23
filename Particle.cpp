#include "main.h"

Particle::Particle(int i, double x, double y, double vx, double vy) {
    this->i = i;
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
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

void Particle::update() {
    if (y + r + vy > WINDOW_H || y - r + vy < 0) {
        if (GetKeyState(VK_SHIFT) & 0x8000) {
            if (GetKeyState('C') & 0x8000)
                vy *= 0.9;
            else if (GetKeyState('H') & 0x8000)
                vy *= 1.11;
        }
        vy = -vy;
    }

    if (x + r + vx > WINDOW_W) {
        if (GetKeyState('H') & 0x8000)
            vx *= 1.11;
        if (GetKeyState(VK_SHIFT) & GetKeyState('C') & 0x8000)
            vx *= 0.9;
        vx = -vx;
    }

    if (x - r + vx < 0) {
        if (GetKeyState('C') & 0x8000)
            vx *= 0.9;
        if (GetKeyState(VK_SHIFT) & GetKeyState('H') & 0x8000)
            vx *= 0.9;
        vx = -vx;
    }

    x = std::min(std::max(x + vx, r), (double) WINDOW_W - r);
    y = std::min(std::max(y + vy, r), (double) WINDOW_H - r);
}

bool Particle::collides(Particle &p) {
    return pow(x - p.x, 2) + pow(y - p.y, 2) < pow(r + p.r, 2);
}

void Particle::updateKeys() {
    keys.clear();
    keys.insert(IntPair((x - r) / CELL_SIZE, (y - r) / CELL_SIZE));
    keys.insert(IntPair((x + r) / CELL_SIZE, (y - r) / CELL_SIZE));
    keys.insert(IntPair((x + r) / CELL_SIZE, (y + r) / CELL_SIZE));
    keys.insert(IntPair((x - r) / CELL_SIZE, (y + r) / CELL_SIZE));
}

void Particle::setColor() {
    double rv = (1 - (vx * vx + vy * vy) / sqrt(2) / MAX_VX) * 240;
    double X = 1 - abs(fmod(rv / 60, 2) - 1);
    switch ((int) rv / 60) {
    case 0:
        glColor3f(1.0, X, 0);
        break;
    case 1:
        glColor3f(X, 1.0, 0);
        break;
    case 2:
        glColor3f(0, 1.0, X);
        break;
    case 3:
        glColor3f(0, X, 1.0);
        break;
    case 4:
        glColor3f(X, 0, 1.0);
        break;
    case 5:
        glColor3f(1.0, 0, X);
        break;
    }
}

void Particle::heat(int d) {
    if (d == 0) {
        vx += (MAX_VX - vx) * 0.1;
    } else if (d == 1) {
        vy += (MAX_VX - vy) * 0.1;
    }
}