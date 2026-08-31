#include <cmath>
#include "numerical_differentiation.hpp"

// Function : f(x) = x^3 - x - 2
double Differentiation::func(double x)
{
    return x * x * x - x - 2;
}

double ForwardDifference::solve(double x, double h)
{
    return (func(x + h) - func(x)) / h;
}

double BackwardDifference::solve(double x, double h)
{
    return (func(x) - func(x - h)) / h;
}

double CentralDifference::solve(double x, double h)
{
    return (func(x + h) - func(x - h)) / (2 * h);
}