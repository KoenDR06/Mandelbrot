#include <iostream>
#include "CImg.h"
#include <fstream>
#include <cmath>

#include "main.h"

using namespace cimg_library;

unsigned char color[3];
double offset = 1;
int sizeX = 512;
int sizeY = 512;
double zoom = 0;
double xCenter = 0;
double yCenter = 0;
bool slow = true;
bool drawColor = false;

int main(int argc, char** argv) {
    if(cmdOptionExists(argv, argv+argc, "--help")) {
        std::string line;
        std::ifstream helpFile("/home/horseman/Programming/MandelbrotCPP/assets/help.txt");
        while (getline(helpFile, line)) {
		std::cout << line << "\n";
        }
        std::ifstream renderHelpFile("/home/horseman/Programming/MandelbrotCPP/assets/render-help.txt");
        while (getline(renderHelpFile, line)) {
            std::cout << line << "\n";
        }
        return 0;
    }

    if(cmdOptionExists(argv, argv+argc, "--offset")) {
	offset = std::stod(getCmdOption(argv, argv + argc, "--offset"));
    }

    if(cmdOptionExists(argv, argv+argc, "--resolution")) {
	sizeX = atoi(getCmdOption(argv, argv + argc, "--resolution"));
	sizeY = atoi(getCmdOption(argv, argv + argc, "--resolution"));
    }

    if(cmdOptionExists(argv, argv+argc, "--zoom")) {
        zoom = std::stod(getCmdOption(argv, argv + argc, "--zoom"));
    }

    if(cmdOptionExists(argv, argv+argc, "--real")) {
        xCenter = std::stod(getCmdOption(argv, argv + argc, "--real"));
    }

    if(cmdOptionExists(argv, argv+argc, "--imag")) {
        yCenter = std::stod(getCmdOption(argv, argv + argc, "--imag"));
    }

    if(cmdOptionExists(argv, argv+argc, "--fast")) {
        slow = false;
    }

    if(cmdOptionExists(argv, argv+argc, "--color")) {
        drawColor = true;
    }


    int numberOfColorChannels = 3;
    unsigned char initialValue = 0;
    CImg<unsigned char> image(sizeX, sizeY, 1, numberOfColorChannels, initialValue);
    CImgDisplay display(image, "Mandelbrot");

    // Pixel coordinates
    int x = 0;
    int y = 0;

    bool done = false;
    bool refresh = true;
    while (!display.is_closed()) {
        if (display.is_keyESC()) {
            display.close();
            break;
        }
        if (display.is_keyC()) {
          drawColor = !drawColor;
          refresh = true;
        }
        if (display.is_keyH()) {
            std::string line;
            std::ifstream helpFile("/home/horseman/Programming/MandelbrotCPP/assets/render-help.txt");
            while (getline(helpFile, line)) {
                std::cout << line << "\n";
            }
        }
        if (display.is_keyARROWUP()) {
            if (display.is_keyCTRLLEFT() || display.is_keyCTRLRIGHT()) {
                offset /= 0.5;
            } else {
                offset /= 0.9;
            }
            refresh = true;
        }
        if (display.is_keyARROWDOWN()) {
            if (display.is_keyCTRLLEFT() || display.is_keyCTRLRIGHT()) {
                offset *= 0.5;
            } else {
                offset *= 0.9;
            }
            refresh = true;
        }
        if (display.button() && display.mouse_y() >= 0 && display.mouse_x() >= 0 || refresh) {
//            std::cout << "Rendering\n";
            done = false;
            if (!refresh) {
                xCenter = std::exp(-1.0 * zoom) * (4.0 * display.mouse_x() / sizeX - 2.0) + xCenter;
                yCenter = std::exp(-1.0 * zoom) * (4.0 * display.mouse_y() / sizeY - 2.0) + yCenter;
                zoom += 0.75;
                offset *= 0.8;
            }
            while (!done) {
                iterate_mandelbrot(std::exp(-1.0 * zoom) * (4.0 * x / sizeX - 2.0) + xCenter, std::exp(-1.0 * zoom) * (4.0 * y / sizeY - 2.0) + yCenter, 0.0, 0.0);
                image.draw_point(x, y, color);
                if (++x > sizeX) {
                    x = 0;
                    y++;
                    if (slow) {
                        image.display(display);
                    }
	        }
                if (y > sizeY) {
                    y = 0;
                    if (!slow) {
	                image.display(display);
                    }
                    done = true;
                }
            }
            refresh = false;
//            std::cout << "Waiting\n";
        }
	display.wait();
    }

    std::cout << "--zoom " << zoom << " --real " << xCenter << " --imag " << yCenter << std::endl;
    return 0;
}

// z = zReal + zImag
// c = cReal + cImag
// z^2 + c = (zReal^2 - zImag^2 + cReal) + (2*zReal*zImag + cImag)*i
void iterate_mandelbrot(double cReal, double cImag, double zReal, double zImag) {
	double index = 0;
	while (zImag * zImag + zReal * zReal <= 4) {
		double tempRe = zReal * zReal - zImag * zImag + cReal;
		zImag = 2 * zReal * zImag + cImag;
		zReal = tempRe;
		index += offset;
		if (index > 255.0) {
			color[0] = 0;
			color[1] = 0;
			color[2] = 0;
			break;
		}
	}
	if (index <= 255 && !drawColor) {
		color[0] = (int)index;
		color[1] = (int)index;
		color[2] = (int)index;
	} else if (index <= 255 && drawColor) {
            hsl2rgb(index * 360.0 / 255.0, 1.0, 0.5);
        }
}

char* getCmdOption(char** begin, char** end, const std::string& option) {
	char** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end) {
		return *itr;
	}
	return nullptr;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option) {
	return std::find(begin, end, option) != end;
}

void hsl2rgb(double h, double s, double l) {
    double a = s * std::min(l, 1.0 - l);

    auto f = [=](double n) -> double {
        double k = std::fmod(n + h / 30.0, 12.0);
        return l - a * std::max(std::min({k - 3.0, 9.0 - k, 1.0}), -1.0);
    };

    color[0] = static_cast<int>(std::round(f(0) * 255));
    color[1] = static_cast<int>(std::round(f(8) * 255));
    color[2] = static_cast<int>(std::round(f(4) * 255));
}
