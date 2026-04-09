#include "lagrange.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include <algorithm>

Lagrange::Lagrange(const std::vector<DataPoint>& points)
    : Interpolation(points)
{}

Lagrange::Lagrange(const std::string& filename)
    : Interpolation(filename)
{}

double Lagrange::basisValue(int i, double x) const
{
    const auto& data  = dataPoints;         // protected member from Interpolation
    int    n           = static_cast<int>(data.size());//explicitly convert data type from size_t to int
    double numerator   = 1.0;
    double denominator = 1.0;

    for (int j = 0; j < n; ++j) {
        if (j == i) continue;
        numerator   *= (x         - data[j].x);
        denominator *= (data[i].x - data[j].x);
    }

    if (std::fabs(denominator) < 1e-15)
        throw std::runtime_error("Lagrange::basisValue — denominator is zero. "
                                 "Are there duplicate x-values?");
    return numerator / denominator;
}

// evaluate  —  P(x) = Σ f_i * L_i(x)
double Lagrange::evaluate(double x) const
{
    const auto& data = dataPoints;
    int n = static_cast<int>(data.size());
    double result = 0.0;
    for (int i = 0; i < n; ++i)
        result += data[i].fx * basisValue(i, x);
    return result;
}

std::vector<double> Lagrange::basisCoeffs(int i) const  //used for formatting final interpolating polynomial
{
    const auto& data = dataPoints;
    int n = static_cast<int>(data.size());

    auto polyMulRoot = [](const std::vector<double>& a, double root) {
        int sz = static_cast<int>(a.size());
        std::vector<double> result(sz + 1, 0.0);
        for (int k = 0; k < sz; ++k) {
            result[k + 1] += a[k];
            result[k]     -= a[k] * root;
        }
        return result;
    };

    std::vector<double> coeffs = {1.0};//start polynomial
    double denom = 1.0;

    for (int j = 0; j < n; ++j) {
        if (j == i) continue;
        coeffs = polyMulRoot(coeffs, data[j].x);//multiply polynomials
        denom *= (data[i].x - data[j].x);
    }

    if (std::fabs(denom) < 1e-15)
        throw std::runtime_error("Lagrange::basisCoeffs — denominator is zero. "
                                 "Are there duplicate x-values?");

    for (auto& c : coeffs) c /= denom;
    return coeffs;
}

void Lagrange::printPolynomial() const
{
    const auto& data = dataPoints;
    int n      = static_cast<int>(data.size());
    int degree = n - 1;

    std::vector<double> poly(degree + 1, 0.0);
    for (int i = 0; i < n; ++i) {
        std::vector<double> Li = basisCoeffs(i);
        for (int k = 0; k <= degree; ++k)
            poly[k] += data[i].fx * Li[k];
    }

    for (auto& c : poly)
        if (std::fabs(c) < 1e-9) c = 0.0;

    std::cout << "\n============================================\n";
    std::cout << "  Lagrange Interpolating Polynomial\n";
    std::cout << "============================================\n";
    std::cout << "  P(x) = ";

    bool firstTerm = true;
    for (int k = degree; k >= 0; --k) {
        double c = poly[k];
        if (c == 0.0) continue;

        if (firstTerm) {
            if (c < 0) std::cout << "-";
        } else {
            std::cout << (c < 0 ? " - " : " + ");
        }

        double ac = std::fabs(c);

        if (k == 0) {
            std::cout << ac;
        } else if (k == 1) {
            if (std::fabs(ac - 1.0) < 1e-9) std::cout << "x";
            else                              std::cout << ac << "x";
        } else {
            if (std::fabs(ac - 1.0) < 1e-9) std::cout << "x^" << k;
            else                              std::cout << ac << "x^" << k;
        }
        firstTerm = false;
    }

    if (firstTerm) std::cout << "0";
    std::cout << "\n============================================\n";
}

void Lagrange::queryLoop() const
{
    const auto& data = dataPoints;
    const int W = 16;

    std::vector<DataPoint> userPoints;

    std::cout << "\n--------------------------------------------\n";
    std::cout << "  Enter x values to evaluate P(x).\n";
    std::cout << "  Type 'done' or 'q' to finish.\n";
    std::cout << "--------------------------------------------\n";

    while (true) {
        std::cout << "\n  Enter x: ";
        std::string input;
        std::cin >> input;

        if (input == "done" || input == "q" || input == "Q")
            break;

        double qx;
        try {
            qx = std::stod(input);
        } catch (...) {
            std::cout << "  Invalid input. Enter a number or 'done'.\n";
            continue;
        }

        double px = evaluate(qx);
        userPoints.push_back({qx, px});
        std::cout << "  P(" << qx << ") = " << std::fixed
                  << std::setprecision(6) << px << "\n";
    }

    if (userPoints.empty()) {
        std::cout << "\n  No query points entered.\n";
        return;
    }

    // Merge original data + user-queried points and sort by x
    std::vector<DataPoint> allPoints = data;
    for (auto& p : userPoints) allPoints.push_back(p);
    std::sort(allPoints.begin(), allPoints.end(),
              [](const DataPoint& a, const DataPoint& b){ return a.x < b.x; });

    std::cout << "\n============================================\n";
    std::cout << "  Updated Table  (original + queried points)\n";
    std::cout << "============================================\n";
    std::cout << std::setw(W) << "x"
              << std::setw(W) << "f(x) / P(x)"
              << std::setw(10) << "Source\n";
    std::cout << std::string(W * 2 + 10, '-') << "\n";

    for (const auto& p : allPoints) {
        bool isOriginal = false;
        for (const auto& d : data)
            if (std::fabs(d.x - p.x) < 1e-12) { isOriginal = true; break; }

        std::cout << std::setw(W) << std::fixed << std::setprecision(6) << p.x
                  << std::setw(W) << p.fx
                  << std::setw(10) << (isOriginal ? "[given]" : "[interp]")
                  << "\n";
    }
    std::cout << "============================================\n";
}