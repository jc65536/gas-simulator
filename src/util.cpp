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
