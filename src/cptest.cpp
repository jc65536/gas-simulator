#include "ControlPanel.h"
#include "GasSimulator.h"
#include <iostream>
#include <windows.h>

using namespace std;

int main() {
    GasSimulator g;
    ControlPanel cp = ControlPanel(g);
    cp.mainloop();

}