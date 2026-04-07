#ifndef LAGRANGE_H
#define LAGRANGE_H

#include <vector>
#include <string>

// Structure to hold a data point (xi, f(xi))
struct DataPoint {
    double x;
    double fx;
};

// -------------------------------------------------------
// Reads scattered data points from a text file.
// Format: first line = n, then n lines of "x  f(x)"
// -------------------------------------------------------
std::vector<DataPoint> readDataFromFile(const std::string& filename);

// -------------------------------------------------------
// Computes the i-th Lagrange basis polynomial L_i(x).
//   L_i(x) = product_{j!=i} (x - x_j) / (x_i - x_j)
// -------------------------------------------------------
double lagrangeBasis(const std::vector<DataPoint>& data, int i, double x);

// -------------------------------------------------------
// Evaluates P(x) = sum_{i=0}^{n} f_i * L_i(x)
// -------------------------------------------------------
double lagrangeInterpolate(const std::vector<DataPoint>& data, double x);

// -------------------------------------------------------
// Computes polynomial coefficients numerically and prints
// the equation e.g.  P(x) = x^2 + 3.000000x + 2.000000
// -------------------------------------------------------
void printPolynomial(const std::vector<DataPoint>& data);

// -------------------------------------------------------
// Interactive loop: user enters x values one by one,
// program evaluates P(x) and shows a running table of
// original data + all user-queried points.
// -------------------------------------------------------
void queryLoop(const std::vector<DataPoint>& data);

#endif // LAGRANGE_H