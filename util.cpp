#include "util.h"
#include "main.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>

std::vector<unsigned long long> timers;
std::ofstream fout("output.txt");

void print(char s[]) {
    fout << s << std::endl;
}

void print(double s) {
    fout << s << std::endl;
}

void print(int s) {
    fout << s << std::endl;
}

int tStart() {
    timers.push_back(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    return timers.size() - 1;
}

unsigned long long tEnd(int i, char m[]) {
    unsigned long long t = tEnd(i);
    fout << "[" << i << "]" << m << " time: " << t << std::endl;
    return t;
}

unsigned long long tEnd(int i) {
    unsigned long long t = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    timers.erase(timers.begin() + i);
    return t - timers[i];
}

void printMap() {
    for (auto &e : map) {
        fout << "(" << e.first.first << ", " << e.first.second << ") : ";
        for (int f : e.second) {
            fout << f << ", ";
        }
        fout << std::endl;
    }
    fout << std::endl;
}

void printSpeeds() {
    int a[40] = {0};
    for (Particle &p : all) {
        double s = sqrt(p.vx * p.vx + p.vy * p.vy);
        a[(int) s]++;
    }
    fout << "speed | count |" << std::endl;
    for (int i = 0; i < 20; i++) {
        fout << std::setw(i < 9 ? 3 : 2) << i << "-" << i + 1 << " | " << std::setw(5) << a[i] << " |";
        for (int j = 0; j < a[i]; j++) {
            fout << "*";
        }
        fout << std::endl;
    }
}
