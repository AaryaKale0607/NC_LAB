#ifndef CURVE_FITTING_HPP
#define CURVE_FITTING_HPP

#include "matrix.hpp"
#include <vector>
#include <string>

// ── Data point struct ─────────────────────────────────────────────────────────
// xi = independent variable, fi = observed / dependent variable
struct FitPoint {
    double xi;
    double fi;
};

// ── Supported fitting models ──────────────────────────────────────────────────
enum class FitModel {
    LINEAR,       // fi = a*xi + b          (degree-1 polynomial)
    POLYNOMIAL,   // fi = a0 + a1*xi + ... + an*xi^n
    EXPONENTIAL,  // fi = a * e^(b*xi)      (linearised)
    POWER,        // fi = a * xi^b           (linearised)
    LOGARITHMIC   // fi = a + b * ln(xi)
};

// ─────────────────────────────────────────────────────────────────────────────
// CurveFitting — abstract base class
//
// Inherits from Matrix so ALL Matrix I/O and display utilities are
// available without re-writing:
//   readFromFile()   – read raw numbers from file (Matrix method)
//   writeToFile()    – write matrix data to file  (Matrix method)
//   display()        – print the matrix block     (Matrix method)
//   operator<<       – stream the matrix           (Matrix method)
//
// The internal Matrix block stores the data table as an (N x 2) matrix:
//   column 0 → xi values
//   column 1 → fi values
//
// Text file format for readTable() / file constructor:
//   Line 1  :  N          (integer — number of data points)
//   Lines 2…N+1 :  xi  fi  (one pair per line, space separated)
// ─────────────────────────────────────────────────────────────────────────────
class CurveFitting : public Matrix
{
protected:
    std::vector<FitPoint> dataPoints;    // raw (xi, fi) pairs
    std::vector<double>   coefficients;  // fitted coefficients after fit()
    FitModel              model;         // chosen fitting model
    int                   degree;        // polynomial degree (POLYNOMIAL only)

    // Sync dataPoints vector → inherited Matrix block (N×2)
    void syncToMatrix();

    // (Re)allocate the inherited Matrix block safely
    void reallocBlock(int r, int c);

public:
    // ── Constructors ─────────────────────────────────────────────────────────
    // Construct from an explicit vector of data points
    CurveFitting(const std::vector<FitPoint>& points,
                 FitModel model = FitModel::LINEAR,
                 int degree = 1);

    // Construct by reading a table from a text file
    // (uses readTable() internally — same format as Lagrange / Interpolation)
    explicit CurveFitting(const std::string& filename,
                          FitModel model = FitModel::LINEAR,
                          int degree = 1);

    virtual ~CurveFitting() = default;

    // ── File I/O — thin wrappers that reuse Matrix infrastructure ─────────────
    // Read N pairs from text file → populates dataPoints + syncs Matrix block
    void readTable(const std::string& filename);

    // Write the (xi, fi) data table back to a file
    // Uses Matrix::writeToFile() for the internal matrix block
    void writeTable(const std::string& filename) const;

    // ── Feed — add one point at runtime ──────────────────────────────────────
    void feedPoint(double xi, double fi);

    // ── Display — reuses Matrix::display() for the internal block ────────────
    // Pretty header + Matrix::display() for the (xi, fi) table
    void displayTable() const;

    // ── Pure-virtual interface (subclasses must implement) ────────────────────
    virtual void   fit()                    = 0; // compute coefficients
    virtual double evaluate(double x) const = 0; // evaluate fitted curve at x
    virtual void   printEquation()    const = 0; // print fitted equation

    // ── Concrete helpers ──────────────────────────────────────────────────────
    // Coefficient of determination R²
    virtual double rSquared() const;

    // Interactive x → y evaluation loop
    virtual void queryLoop() const;

    // ── Accessors ─────────────────────────────────────────────────────────────
    const std::vector<FitPoint>& getDataPoints()   const;
    const std::vector<double>&   getCoefficients() const;
    int                          getNumPoints()    const;
    FitModel                     getModel()        const;
    int                          getDegree()       const;
};

#endif // CURVE_FITTING_HPP