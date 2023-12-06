#ifndef MATH_H
#define MATH_H

#include <string>
#include <cmath>

namespace math {
int factorial(int x);
double sin_x_reduce(double x);
double sin_taylor(double x);
double cos(double x);
double cos_taylor(double x);
double sin(double x);
double tan(double x);
template<typename T>
double newton(double x_n, double (*f)(T, T, T), double (*f1)(T, T, T), double param = 0, double param1 = 0);
double root_f(double x, double n, double a);
double root_f1(double x, double n, double param1);
double root(double n, double a);
double sqroot_Heron(double a, double e);
double arcsin_f(double x, double a, double param1);
double arcsin_f1(double x, double param, double param1);
double arcsin(double x);
double arccos(double x);
double arctan_f(double x, double a, double param1);
double arctan_f1(double x, double param, double param1);
double arctan(double x);
double exp_taylor(double x);
double exp(double x);
double ln_f(double x, double a, double param1);
double ln_f1(double x, double param, double param1);
double ln(double x);
double log(double b, double x);
double pow(double a, double n);
}

#endif
