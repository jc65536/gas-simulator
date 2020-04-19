/*
TODO:
 - make collision detection based on intersecting path lines
 - optimize!!
 - add buttons to heat/cool
 - add data visualizations
 - add customizable runs
 - make 3d???
*/

#define GLFW_DLL

#include "ControlPanel.h"
#include "GasSimulator.h"
#include <iostream>
#include <thread>
#include <fstream>

int main(int argc, char *argv[]) {
    
    //std::ofstream out("output.txt");
    //std::cout.rdbuf(out.rdbuf());

    GasSimulator g;
    
    std::thread t([&]() {
        ControlPanel cp(g);
        cp.mainloop();
        std::cout << "cp thread somehow failed?" << std::endl;
    });
    
    g.mainloop();
    t.join();
    
    //std::cout.rdbuf(nullptr);
}
