#include "decimal.h"

unsigned char color[3];
double offset = 1;
int sizeX = 512;
int sizeY = 512;
double zoom = 0;
double xCenter = 0;
double yCenter = 0;
bool slow = true;
bool drawColor = false;

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
    }
}

// Buggy as shit
bool detect_mul_overflow(decimal a, decimal b) {
    if (a.number == 0 || b.number == 0) {
        std::cout << "a or b is 0, returning false\n";
        return false;
    }
    if ((a * b) / b == b) {
        std::cout << "a*b/b == b is true, returning false\n";
        return false;
    }
    std::cout << a << " " << a * b << " " << (a * b) / b << std::endl;
    return true;
}

bool detect_add_overflow(decimal a, decimal b) {
    if (a.sign xor b.sign) {
        return false;
    }

    decimal result = a + b;
    if (result.number < a.number || result.number < b.number) {
        return true;
    }

    return false;
}

bool detect_overflow(decimal a, decimal b) {
    return detect_mul_overflow(a, a) || detect_mul_overflow(b, b) || detect_add_overflow(a*a, b*b);
}

void iterate_mandelbrot_fixed_point(decimal cReal, decimal cImag, decimal zReal, decimal zImag) {
    double index = 0;
    while (!detect_overflow(zImag, zReal)) {
        decimal tempRe = zReal * zReal - zImag * zImag + cReal;
        zImag = decimal(2LL) * zReal * zImag + cImag;
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
    }
}

int main() {
//    iterate_mandelbrot(-0.53, 0.51, 0, 0);
//    std::cout << static_cast<int>(color[0]) << std::endl;

    decimal zReal(1.1);
    decimal zImag(0.51);

    bool a1 = detect_mul_overflow(zReal, zReal);
    bool a2 = detect_mul_overflow(zImag, zImag);
    bool a3 = detect_add_overflow(zReal * zReal, zImag * zImag);

    std::cout << a1 << a2 << a3 << std::endl;
}
