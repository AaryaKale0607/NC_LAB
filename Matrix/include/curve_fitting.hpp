#ifndef CURVE_FITTING_HPP
#define CURVE_FITTING_HPP

#include "matrix.hpp"
#include <vector>
#include <string>

struct FitPoint {
    double xi;
    double fi;
};

class CurveFitting : public Matrix
{
protected:
    std::vector<FitPoint> dataPoints;    // raw (xi, fi) pairs
    std::vector<double>   coefficients;  // fitted coefficients after fit()

    void syncToMatrix();
    void reallocBlock(int r, int c);

public:
    // to prevent accidental implicit conversions.
    // Construct from a vector of data points
    explicit CurveFitting(const std::vector<FitPoint>& points);

    // Construct by reading a data file
    explicit CurveFitting(const std::string& filename);

    virtual ~CurveFitting() = default;

    // Read N (xi, fi) pairs from a text file
    void readTable(const std::string& filename);

    // Write the (xi, fi) data table to a file
    void writeTable(const std::string& filename) const;

    // Append a single data point at runtime (invalidates any previous fit)
    void feedPoint(double xi, double fi);

    // Pretty-print the data table
    void displayTable() const;

    // Pure virtual interface — implemented by LeastSquares
    virtual void   fit()                    = 0;
    virtual double evaluate(double x) const = 0;
    virtual void   printEquation()    const = 0;

    // Coefficient of determination R² = 1 − SS_res / SS_tot
    // virtual double rSquared() const;

    // Interactive x → y evaluation loop (enter 'q' to quit)
    // virtual void queryLoop() const;

    // Accessors
    const std::vector<FitPoint>& getDataPoints()   const;
    const std::vector<double>&   getCoefficients() const;
    int                          getNumPoints()    const;
};

#endif 