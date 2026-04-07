#include "Lagrange.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>
#include <algorithm>

// -----------------------------------------------------------
// readDataFromFile
// -----------------------------------------------------------
std::vector<DataPoint> readDataFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + filename);

    int n;
    file >> n;
    if (n <= 0)
        throw std::runtime_error("Number of data points must be positive.");

    std::vector<DataPoint> data(n);
    for (int i = 0; i < n; ++i) {
        if (!(file >> data[i].x >> data[i].fx))
            throw std::runtime_error("File has insufficient data. Expected " +
                                     std::to_string(n) + " points.");
    }
    file.close();
    return data;
}

// -----------------------------------------------------------
// lagrangeBasis  —  L_i(x)
// -----------------------------------------------------------
double lagrangeBasis(const std::vector<DataPoint>& data, int i, double x) {
    int    n           = static_cast<int>(data.size());
    double numerator   = 1.0;
    double denominator = 1.0;
    for (int j = 0; j < n; ++j) {
        if (j == i) continue;
        numerator   *= (x         - data[j].x);
        denominator *= (data[i].x - data[j].x);
    }
    if (std::fabs(denominator) < 1e-15)
        throw std::runtime_error("Denominator zero — duplicate x-values?");
    return numerator / denominator;
}

// -----------------------------------------------------------
// lagrangeInterpolate  —  P(x) = Σ f_i * L_i(x)
// -----------------------------------------------------------
double lagrangeInterpolate(const std::vector<DataPoint>& data, double x) {
    double result = 0.0;
    int    n      = static_cast<int>(data.size());
    for (int i = 0; i < n; ++i)
        result += data[i].fx * lagrangeBasis(data, i, x);
    return result;
}

// -----------------------------------------------------------
// printPolynomial
//   Samples P(x) at many points to extract coefficients by
//   using a Vandermonde-style approach: evaluates P at
//   degree+1 points then back-substitutes (Newton forward
//   difference is cleaner for equidistant; here we use a
//   direct numeric coefficient extraction via the fact that
//   we can expand the polynomial term by term).
//
//   Approach: represent each L_i(x) as a coefficient vector,
//   multiply them out, then sum up f_i * coeff_i.
// -----------------------------------------------------------

// Multiply polynomial 'a' by (x - root), returns new coeff vector
// coeff[k] = coefficient of x^k
static std::vector<double> polyMulRoot(const std::vector<double>& a, double root) {
    int n = static_cast<int>(a.size());
    std::vector<double> result(n + 1, 0.0);
    for (int k = 0; k < n; ++k) {
        result[k + 1] += a[k];       // a[k] * x
        result[k]     -= a[k] * root; // a[k] * (-root)
    }
    return result;
}

// Build coefficient vector for L_i(x)
static std::vector<double> basisCoeffs(const std::vector<DataPoint>& data, int i) {
    int n = static_cast<int>(data.size());
    std::vector<double> coeffs = {1.0}; // start with 1
    double denom = 1.0;
    for (int j = 0; j < n; ++j) {
        if (j == i) continue;
        coeffs = polyMulRoot(coeffs, data[j].x);
        denom *= (data[i].x - data[j].x);
    }
    for (auto& c : coeffs) c /= denom;
    return coeffs;
}

void printPolynomial(const std::vector<DataPoint>& data) {
    int n     = static_cast<int>(data.size());
    int degree = n - 1;

    // Accumulate P(x) coefficient vector
    std::vector<double> poly(degree + 1, 0.0);
    for (int i = 0; i < n; ++i) {
        std::vector<double> Li = basisCoeffs(data, i);
        for (int k = 0; k <= degree; ++k)
            poly[k] += data[i].fx * Li[k];
    }

    // Round near-zero coefficients
    for (auto& c : poly)
        if (std::fabs(c) < 1e-9) c = 0.0;

    std::cout << "\n============================================\n";
    std::cout << "  Lagrange Interpolating Polynomial\n";
    std::cout << "============================================\n";
    std::cout << "  P(x) = ";

    bool firstTerm = true;
    // Print from highest degree down
    for (int k = degree; k >= 0; --k) {
        double c = poly[k];
        if (c == 0.0) continue;

        // Sign
        if (firstTerm) {
            if (c < 0) std::cout << "-";
        } else {
            std::cout << (c < 0 ? " - " : " + ");
        }

        double ac = std::fabs(c);

        if (k == 0) {
            // constant term: always print value
            std::cout << ac;
        } else if (k == 1) {
            if (std::fabs(ac - 1.0) < 1e-9)
                std::cout << "x";
            else
                std::cout << ac << "x";
        } else {
            if (std::fabs(ac - 1.0) < 1e-9)
                std::cout << "x^" << k;
            else
                std::cout << ac << "x^" << k;
        }
        firstTerm = false;
    }

    if (firstTerm) std::cout << "0"; // zero polynomial
    std::cout << "\n============================================\n";
}

// -----------------------------------------------------------
// queryLoop
//   Asks user for x values, evaluates P(x), shows updated table
// -----------------------------------------------------------
void queryLoop(const std::vector<DataPoint>& data) {
    const int W = 16;

    // Collect user query results
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

        double px = lagrangeInterpolate(data, qx);
        userPoints.push_back({qx, px});
        std::cout << "  P(" << qx << ") = " << std::fixed
                  << std::setprecision(6) << px << "\n";
    }

    if (userPoints.empty()) {
        std::cout << "\n  No query points entered.\n";
        return;
    }

    // ---- Print combined table: original + user points ----
    // Merge and sort by x
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

    for (auto& p : allPoints) {
        // Check if this x was in original data
        bool isOriginal = false;
        for (auto& d : data)
            if (std::fabs(d.x - p.x) < 1e-12) { isOriginal = true; break; }

        std::cout << std::setw(W) << std::fixed << std::setprecision(6) << p.x
                  << std::setw(W) << p.fx
                  << std::setw(10) << (isOriginal ? "[given]" : "[interp]")
                  << "\n";
    }
    std::cout << "============================================\n";
}