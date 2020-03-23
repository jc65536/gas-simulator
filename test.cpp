#include <cmath>
#include <iostream>
using namespace std;

double *hsl2rgb(double H, double S, double L) {
    double C = (1 - abs(2 * L - 1)) * S;
    double Hp = H / 60;
    double X = 1 - abs(fmod(H / 60, 2) - 1);
    static double rgb[3];
    switch ((int) H / 60) {
    case 0:
        rgb[0] = 1.0;
        rgb[1] = X;
        rgb[2] = 0;
        break;
    case 1:
        rgb[0] = X;
        rgb[1] = 1.0;
        rgb[2] = 0;
        break;
    case 2:
        rgb[0] = 0;
        rgb[1] = 1.0;
        rgb[2] = X;
        break;
    case 3:
        rgb[0] = 0;
        rgb[1] = X;
        rgb[2] = 1.0;
        break;
    case 4:
        rgb[0] = X;
        rgb[1] = 0;
        rgb[2] = 1.0;
        break;
    case 5:
        rgb[0] = 1.0;
        rgb[1] = 0;
        rgb[2] = X;
        break;
    }
    return rgb;
}

int main() {
    for (int i = 0; i < 360; i++) {
        double *rgb = hsl2rgb(i, 1.0, 0.5);
        cout << rgb[0] * 255 << " " << rgb[1] * 255 << " " << rgb[2] * 255 << endl;
    }
}