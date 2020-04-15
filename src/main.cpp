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
#include "ControlPanel.h"
#include "GasSimulator.h"
#include "nanogui/nanogui.h"
#include "util.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <thread>

bool pause = false;
int fps = 60;

int main(int argc, char *argv[]) {
    if (!glfwInit()) exit(1);
    GasSimulator g;
    std::thread t([&]() {
        ControlPanel cp(g);
        cp.mainloop();
        std::cout << "cp thread is done?!" << std::endl;
    });
    g.mainloop();
    t.join();
    glfwTerminate();
}
