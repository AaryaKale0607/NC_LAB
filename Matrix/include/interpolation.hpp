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

// -------------------------------------------------------
// Interpolation: abstract base class inheriting from Matrix.
// Stores data points as a 2-column Matrix (column 0 = x,
// column 1 = f(x)) and declares the interface every
// concrete interpolation method must implement.
// -------------------------------------------------------
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

    // ---------------------------------------------------------
    // Pure virtual interface — every subclass must implement
    // ---------------------------------------------------------

    // Evaluate the interpolant at x
    virtual double evaluate(double x) const = 0;

    // Print a human-readable form of the interpolating polynomial
    virtual void printPolynomial() const = 0;

    // Interactive query loop
    virtual void queryLoop() const = 0;

    // ---------------------------------------------------------
    // Common helpers available to all subclasses
    // ---------------------------------------------------------
    const std::vector<DataPoint>& getDataPoints() const;
    int getNumPoints() const;

    // Display the data-point table (uses Matrix::display internally)
    void displayData() const;
};

#endif // INTERPOLATION_HPP