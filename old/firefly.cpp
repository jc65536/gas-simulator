#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <iostream>

#define PI 3.14159265

struct Particle {
    double x, y, rad = rand() % 33 + 8, vx, vy;
    double vr = 0.6;
    double r = 1.0 - rand() / 5.0 / RAND_MAX;
    double g = 1.0 - rand() / 5.0 / RAND_MAX;
    double b = rand() / 5.0 / RAND_MAX;

    Particle(double x, double y, double vx, double vy) {
        this->x = x;
        this->y = y;
        this->vx = vx;
        this->vy = vy;
    }

    void draw() {
        for (double i = rad; i > 0; i -= 2) {
            glBegin(GL_POLYGON);
            for (double t = 0; t < 2 * PI; t += 0.05) {
                glColor4f(r, g, b, 1.0 / i);
                glVertex2f(x + i * cos(t), y + i * sin(t));
            }
            glEnd();
        }
    }

    void update() {
        if (rad <= 8 || rad >= 40) vr = -vr;
        rad += vr;
        if (x + vx > glutGet(GLUT_WINDOW_WIDTH) || x + vx < 0) vx = -vx;
        if (y + vy > glutGet(GLUT_WINDOW_HEIGHT) || y + vy < 0) vy = -vy;
        x = std::min(std::max(x + vx, 0.0), (double) glutGet(GLUT_WINDOW_WIDTH));
        y = std::min(std::max(y + vy, 0.0), (double) glutGet(GLUT_WINDOW_HEIGHT));
    }
};


void init();
void display();
void reshape(int, int);
void timer(int);

std::vector<Particle> particles;

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowPosition(200, 100);
    glutInitWindowSize(500, 500);

    glutCreateWindow("Window 1");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    init();

    glutMainLoop();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_BLEND); //Enable blending.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; i < 18; i++) {
        Particle p = Particle(rand() % (glutGet(GLUT_WINDOW_WIDTH) - 80) + 40,
                        rand() % (glutGet(GLUT_WINDOW_HEIGHT) - 80) + 40,
                        (rand() % 2 - 0.5) * 2 * (rand() % 8 + 1) / 5.0,
                        (rand() % 2 - 0.5) * 2 * (rand() % 8 + 1) / 5.0);
        std::cout << p.x << "," << p.y << "," << p.rad << "," << p.vx << "," << p.vy << "," << p.r << "," << p.g << "," << p.b << std::endl;
        particles.push_back(p);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //draw
    for (Particle &p : particles) {
        p.update();
        p.draw();
    }
    std::cout << std::endl;

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}
