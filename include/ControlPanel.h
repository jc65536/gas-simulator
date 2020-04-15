#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include "nanogui/nanogui.h"
#include "GasSimulator.h"
#include <thread>

class ControlPanel {
    public:
    nanogui::Screen *screen;
    nanogui::FormHelper *form;
    GasSimulator &g;
    int n, r;
    double maxv;

    ControlPanel(GasSimulator &g);

    void mainloop();
};

#endif // CONTROLPANEL_H