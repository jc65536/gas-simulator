#include "main.h"
#include <GL/glut.h>
#include <algorithm>
#include <iostream>
#include <cmath>

bool operator<(const Particle &p, const Particle &q) {
    return p.i < q.i;
}

Particle all[NUM_PARTICLES];
HashMap m;

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowPosition(INIT_WIN_X, INIT_WIN_Y);
    glutInitWindowSize(INIT_WIN_W, INIT_WIN_H);

    glutCreateWindow("Gas Simulator");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    for (int i = 0; i < NUM_PARTICLES; i++) {
        for (IntPair k : all[i].keys) {
            addParticle(k, all[i]);
        }
    }

    std::cout << std::endl;
    printMap();

    glutMainLoop();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //draw
    for (Particle &p : all) {
        // deregister all of p's keys so we can update them below
        for (IntPair k : p.keys) {
            std::vector<ParticleRef> &bucket = m.find(k)->second;
            bucket.erase(std::find_if(bucket.begin(), bucket.end(), [&p](Particle &q) {
                return p == q;
            }));
        }
        
        p.updateKeys();

        // need to use a set since each particle has 4 keys, so sometimes they collide more times than desired
        std::set<ParticleRef> collisions;
        for (IntPair k : p.keys) {
            // check collision with all particles in k's cell
            std::vector<ParticleRef> &bucket = addParticle(k, p);
            for (Particle &q : bucket) {
                if (collides(p, q))
                    collisions.insert(q);
            }
        }

        for (Particle &q : collisions) {
            evalCollision(p, q);
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

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);
}

std::vector<ParticleRef> &addParticle(IntPair k, Particle &p) {
    if (m.count(k) > 0) {
        std::vector<ParticleRef> &bucket = m.find(k)->second;
        bucket.push_back(p);
        return bucket;
    } else {
        std::vector<ParticleRef> v = {p};
        return m.insert(std::pair<IntPair, std::vector<ParticleRef>>(k, v)).first->second;
    }
}

void evalCollision(Particle &p, Particle &q) {
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

    /*
    draw();
    p.draw();

    // transverse axis
    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(x, y);
    glVertex2f(p.x, p.y);
    glEnd();

    // transverse angle
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 1.0, 0.0);
    if (atp < 0) {
        for (double t = 0; t > atp; t -= 0.1) {
            glVertex2f(x + r / 2 * cos(t), y + r / 2 * sin(t));
        }
    } else {
        for (double t = 0; t < atp; t += 0.1) {
            glVertex2f(x + r / 2 * cos(t), y + r / 2 * sin(t));
        }
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 1.0, 0.0);
    if (atq < 0) {
        for (double t = 0; t > atq; t -= 0.1) {
            glVertex2f(p.x + p.r / 2 * cos(t), p.y + p.r / 2 * sin(t));
        }
    } else {
        for (double t = 0; t < atq; t += 0.1) {
            glVertex2f(p.x + p.r / 2 * cos(t), p.y + p.r / 2 * sin(t));
        }
    }
    glEnd();


    // transverse velocity component
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(x, y);
    glVertex2f(x + vtp * cos(atp) * 10, y + vtp * sin(atp) * 10);
    glVertex2f(p.x, p.y);
    glVertex2f(p.x + vtq * cos(atq) * 10, p.y + vtq * sin(atq) * 10);
    glEnd();

    // conjugate velocity component
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(x + vtp * cos(atp) * 10, y + vtp * sin(atp) * 10);
    glVertex2f(x + vtp * cos(atp) * 10 + vcpx * 10, y + vtp * sin(atp) * 10 + vcpy * 10);
    glVertex2f(p.x + vtq * cos(atq) * 10, p.y + vtq * sin(atq) * 10);
    glVertex2f(p.x + vtq * cos(atq) * 10 + vcqx * 10, p.y + vtq * sin(atq) * 10 + vcqy * 10);
    glEnd();

    glutSwapBuffers();
    */
}

// Util
void printMap() {
    for (auto e : m) {
        std::cout << "(" << e.first.first << ", " << e.first.second << ") : ";
        for (Particle &f : e.second) {
            std::cout << f.i << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}