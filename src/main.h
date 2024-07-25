#ifndef MANDELBROT_H
#define MANDELBROT_H

int main(int argc, char** argv);
void iterate_mandelbrot(double cReal, double cImag, double zReal, double zImag);
void drawJuliaSet(double xCenter, double yCenter);

void hsl2rgb(double h, double s, double l);
char* getCmdOption(char** begin, char** end, const std::string& option);
bool cmdOptionExists(char** begin, char** end, const std::string& option);

#endif
