#include "system_linear_eq.hpp"

SystemOfLinearEquation::SystemOfLinearEquation(int n)
    : Matrix(n, n + 1)
{
}

std::vector<double> SystemOfLinearEquation::getSolution() const
{
    return solution;
}