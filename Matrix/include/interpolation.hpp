#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include "matrix.hpp"
#include <vector>
#include <string>

// Structure to hold a data point (xi, f(xi))
struct DataPoint {
    double x;
    double fx;
};
class Interpolation : public Matrix
{
protected:
    std::vector<DataPoint> dataPoints; // raw data kept for convenience
    // Helper: load dataPoints into the inherited Matrix (n x 2)
    void loadMatrix();
public:
    // Construct from a vector of data points
    explicit Interpolation(const std::vector<DataPoint>& points);
    // Construct by reading from a file
    // Format: first line = n, then n lines of "x  f(x)"
    explicit Interpolation(const std::string& filename);
    virtual ~Interpolation() = default;
    // Evaluate the interpolant at x
    virtual double evaluate(double x) const = 0;
    // Print a human-readable form of the interpolating polynomial
    virtual void printPolynomial() const = 0;
    // Interactive query loop
    virtual void queryLoop() const = 0;
     const std::vector<DataPoint>& getDataPoints() const;
    int getNumPoints() const;
    // Display the data-point table (uses Matrix::display internally)
    void displayData() const;
};

#endif