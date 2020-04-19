#include "ControlPanel.h"
#include "glfw/glfw3.h"
#include <iostream>
#include <string>
#include <thread>
#include <vector>

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
    form->add_group("Gas properties");
    form->add_variable<int>(
            "Particle amount", [&](int n) {
        if (0 < n)
            this->n = n; }, [&]() { return this->n; })
        ->set_fixed_width(80);
    form->add_variable<int>(
            "Particle radius", [&](int r) {
        if (0 < r)
            this->r = r; }, [&]() { return this->r; })
        ->set_fixed_width(80);
    form->add_variable<double>(
            "Initial max speed", [&](double maxv) {
        if (0 < maxv)
            this->maxv = maxv; }, [&]() { return this->maxv; })
        ->set_fixed_width(80);
    form->add_button("Restart simulation", [&]() {
            //std::cout << "button pressed!" << std::endl;
            g.mu.lock();
            //std::cout << "locked by cp" << std::endl;
            g.gasInit(n, r, maxv);
            /*
        for (Particle &p : g.particles) {
            std::cout << p.i << ":" << p.x << "," << p.y << ":";
            for (IntPair i : p.keys) {
                std::cout << i.first << "," << i.second << ":";
            }
            std::cout << std::endl;
        }
        g.printMap();
        std::cout << "unlocked by cp" << std::endl;
        */
            g.mu.unlock();
        })
        ->set_width(10);

    form->add_group("Container heating");

    std::string sides[] = {"Top", "Left", "Right", "Bottom"};
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

        if (s == "Top") {
            cool->set_change_callback([&](bool b) {
                if (b) {
                    g.top = COOL;
                    std::cout << g.top << " " << g.left << " " << g.right << " " << g.bottom << std::endl;
                }
            });
            ins->set_change_callback([&](bool b) {
                if (b) {
                    g.top = INSULATE;
                    std::cout << g.top << " " << g.left << " " << g.right << " " << g.bottom << std::endl;
                }
            });
            heat->set_change_callback([&](bool b) {
                if (b) {
                    g.top = HEAT;
                    std::cout << g.top << " " << g.left << " " << g.right << " " << g.bottom << std::endl;
                }
            });
        } else if (s == "Left") {
            cool->set_change_callback([&](bool b) {
                if (b) {
                    g.left = COOL;
                    std::cout << g.top << " " << g.left << " " << g.right << " " << g.bottom << std::endl;
                }
            });
            ins->set_change_callback([&](bool b) {
                if (b) {
                    g.left = INSULATE;
                    std::cout << g.top << " " << g.left << " " << g.right << " " << g.bottom << std::endl;
                }
            });
            heat->set_change_callback([&](bool b) {
                if (b) {
                    g.left = HEAT;
                    std::cout << g.top << " " << g.left << " " << g.right << " " << g.bottom << std::endl;
                }
            });
        } else if (s == "Right") {
            cool->set_change_callback([&](bool b) {
                if (b) {
                    g.right = COOL;
                    std::cout << g.top << " " << g.left << " " << g.right << " " << g.bottom << std::endl;
                }
            });
            ins->set_change_callback([&](bool b) {
                if (b) {
                    g.right = INSULATE;
                    std::cout << g.top << " " << g.left << " " << g.right << " " << g.bottom << std::endl;
                }
            });
            heat->set_change_callback([&](bool b) {
                if (b) {
                    g.right = HEAT;
                    std::cout << g.top << " " << g.left << " " << g.right << " " << g.bottom << std::endl;
                }
            });
        } else if (s == "Bottom") {
            cool->set_change_callback([&](bool b) {
                if (b) {
                    g.bottom = COOL;
                    std::cout << g.top << " " << g.left << " " << g.right << " " << g.bottom << std::endl;
                }
            });
            ins->set_change_callback([&](bool b) {
                if (b) {
                    g.bottom = INSULATE;
                    std::cout << g.top << " " << g.left << " " << g.right << " " << g.bottom << std::endl;
                }
            });
            heat->set_change_callback([&](bool b) {
                if (b) {
                    g.bottom = HEAT;
                    std::cout << g.top << " " << g.left << " " << g.right << " " << g.bottom << std::endl;
                }
            });
        }

        form->add_widget(s + " side", radioWrapper);
    }

    form->add_group("Program settings");
    form->add_variable<int>(
            "Framerate", [&](int fps) {
        if (0 < fps && fps <= 60)
            g.fps = fps; }, [&]() { return g.fps; })
        ->set_fixed_width(80);
    form->add_variable<int>(
            "Partition size", [&](int cellw) {
        if (2 * g.r < cellw)
            g.cellw = cellw; }, [&]() { return g.cellw; })
        ->set_fixed_width(80);

    //radioWrapper->set_visible(true);
}

void ControlPanel::mainloop() {
    screen->perform_layout();
    screen->set_visible(true);
    nanogui::mainloop();
    nanogui::shutdown();
    exit(0);
}