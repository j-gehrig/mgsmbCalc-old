#ifndef MATH_H
#define MATH_H

#include <string>
#include <cmath>

struct Math {
    static double factorial(double x);

    static double cos(double x);
    static double sin(double x);
    static double tan(double x);

    static double root(double n, double a);
    static double sqroot(double x);

    static double arcsin(double x);
    static double arccos(double x);
    static double arctan(double x);

    static double exp(double x);

    static double ln(double x);
    static double log(double b, double x);

    static double pow(double a, double n);
};

#endif
