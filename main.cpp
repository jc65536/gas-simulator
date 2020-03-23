#include "main.h"

bool operator<(const Particle &p, const Particle &q) {
    return p.i < q.i;
}

Particle allParticles[NUM_PARTICLES];
HashMap m;
/*
 ___________________
| 0 | 1 | 2 | 3 | 4 |
|___|___|___|___|___|
| 5 | 6 | 7 | 8 | 9 |
|___|___|___|___|___|
|10 |11 |12 |13 |14 |
|___|___|___|___|___|
|15 |16 |17 |18 |19 |
|___|___|___|___|___|
*/

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowPosition(200, 100);
    glutInitWindowSize(800, 500);

    glutCreateWindow("Window 1");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    
    for (int i = 0; i < NUM_PARTICLES; i++) {
        allParticles[i] = Particle(i,
                                   rand() % (WINDOW_W - 10) + 5,
                                   rand() % (WINDOW_H - 10) + 5,
                                   (rand() % 20 * MAX_VX - 10 * MAX_VX) / 10.0,
                                   (rand() % 20 * MAX_VX - 10 * MAX_VX) / 10.0);
        //std::cout << allParticles[i].x << "," << allParticles[i].y << "," << allParticles[i].vx << "," << allParticles[i].vy << std::endl;
        for (IntPair k : allParticles[i].keys) {
            addParticle(k, allParticles[i]);
        }
    }
    /*
    allParticles[0] = Particle(0, 30, 30, 2, 2);
    allParticles[1] = Particle(1, 170, 170, -2, -2);
    for (IntPair k : allParticles[0].keys) {
        addParticle(k, allParticles[0]);
    }

    for (IntPair k : allParticles[1].keys) {
        addParticle(k, allParticles[1]);
    }
    */
    std::cout << std::endl;

    printMap();

    glutMainLoop();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //draw
    for (Particle &p : allParticles) {
        for (IntPair k : p.keys) {
            std::vector<ParticleRef> &bucket = m.find(k)->second;
            bucket.erase(std::find_if(bucket.begin(), bucket.end(), [&p](Particle &q) {
                return p == q;
            }));
        }

        p.updateKeys();

        std::set<ParticleRef> collisions;

        for (IntPair k : p.keys) {
            // check collision with all particles in k's cell
            std::vector<ParticleRef> &bucket = addParticle(k, p);
            for (Particle &q : bucket) {
                if (p != q && p.collides(q))
                    collisions.insert(q);
            }
        }

        for (Particle &q : collisions) {
            evalCollision(p, q);
        }

        p.update();
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
    if (GetKeyState('A') & 0x8000) {
        printMap();
    }
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, 0);
}

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
    double vp = sqrt(pow(p.vx, 2) + pow(p.vy, 2));  // speed (velocity magnitude) of p
    double avp = atan2(p.vy, p.vx);                 // velocity angle of p
    double atp = atan2(q.y - p.y, q.x - p.x);       // transverse axis angle w/ respect to p
    double arp = avp - atp;                         // reference angle of p (angle btwn velocity angle & transverse axis)
    double vtp = vp * cos(arp);                     // transverse velocity component of p
    double vtpx = vtp * cos(atp);                   // x component of transverse velocity component
    double vtpy = vtp * sin(atp);                   // y component of transverse velocity component
    double vcp = vp * sin(arp);                     // conjugate velocity component of p
    double vcpx = vcp * cos(atp + PI / 2);          // x component of conjugate velocity component
    double vcpy = vcp * sin(atp + PI / 2);          // y component of conjugate velocity component

    double vq = sqrt(pow(q.vx, 2) + pow(q.vy, 2));  // ditto for q
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

    q.x = p.x + (p.r + q.r) * cos(atp);
    q.y = p.y + (p.r + q.r) * sin(atp);

    /*
    if (std::max(abs(vtp), abs(vtq)) < 0) return;

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

    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(x, y);
    glVertex2f(x + vx * 10, y + vy * 10);
    glVertex2f(p.x, p.y);
    glVertex2f(p.x + p.vx * 10, p.y + p.vy * 10);
    glEnd();
*/
    //glutSwapBuffers();
    //system("pause");
    //exit(0);
}