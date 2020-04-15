#include "ControlPanel.h"
#include "glfw/glfw3.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>

ControlPanel::ControlPanel(GasSimulator &g) : g(g) {
    this->n = g.n;
    this->r = g.r;
    this->maxv = g.maxv;
    nanogui::init();
    screen = new nanogui::Screen(nanogui::Vector2i(300, 500), "Simulator Control Panel", false);
    form = new nanogui::FormHelper(screen);
    auto w = form->add_window(nanogui::Vector2i(0, 0), "");
    w->set_fixed_size(screen->size());
    screen->set_layout(new nanogui::BoxLayout(nanogui::Orientation::Vertical));
    form->add_group("Gas Properties");
    form->add_variable<int>("Particle amount", [&](int n) {
        if (0 < n)
            this->n = n;
    }, [&]() {
        return this->n;
    })->set_fixed_width(80);
    form->add_variable<int>("Particle radius", [&](int r) {
        if (0 < r)
            this->r = r;
    }, [&]() {
        return this->r;
    })->set_fixed_width(80);
    form->add_variable<double>("Initial max speed", [&](double maxv) {
        if (0 < maxv)
            this->maxv = maxv;
    }, [&]() {
        return this->maxv;
    })->set_fixed_width(80);
    form->add_button("Restart simulation", [&]() {
        std::cout << "button pressed!" << std::endl;
        g.mu.lock();
        std::cout << "locked by cp" << std::endl;
        g.gasInit(n, r, maxv);
        std::cout << g.particles.size() << std::endl;
        for (Particle p : g.particles) {
            std::cout << p.x << " ";
        }
        std::cout << std::endl;
        g.mu.unlock();
        std::cout << "unlocked by cp" << std::endl;
    })->set_width(10);

    form->add_group("Container heating");

    std::string sides[] = {"Right", "Top", "Left", "Bottom"};
    int fontSize = form->widget_font_size();
    for (std::string s : sides) {
        nanogui::Widget *radioWrapper = new nanogui::Widget(w);
        radioWrapper->set_layout(new nanogui::BoxLayout(nanogui::Orientation::Horizontal));
        nanogui::Button *cool = new nanogui::Button(radioWrapper, "Cool");
        cool->set_flags(nanogui::Button::RadioButton);
        cool->set_font_size(fontSize);
        nanogui::Button *ins = new nanogui::Button(radioWrapper, "Insulate");
        ins->set_flags(nanogui::Button::RadioButton);
        ins->set_font_size(fontSize);
        ins->set_pushed(true);
        nanogui::Button *heat = new nanogui::Button(radioWrapper, "Heat");
        heat->set_flags(nanogui::Button::RadioButton);
        heat->set_font_size(fontSize);
        form->add_widget(s + " side", radioWrapper);
    }

    form->add_group("Program settings");
    form->add_variable<int>("Framerate", [&](int fps) {
        if (0 < fps && fps <= 60)
            g.fps = fps;
    }, [&]() {
        return g.fps;
    })->set_fixed_width(80);
    form->add_variable<int>("Partition size", [&](int cellw) {
        if (0 < cellw)
            g.cellw = cellw;
    }, [&]() {
        return g.cellw;
    })->set_fixed_width(80);
    
    //radioWrapper->set_visible(true);
}

void ControlPanel::mainloop() {
    screen->perform_layout();
    screen->set_visible(true);
    nanogui::mainloop();
}