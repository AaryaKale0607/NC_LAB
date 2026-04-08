#ifndef LAGRANGE_H
#define LAGRANGE_H

#include "interpolation.hpp"
#include <vector>
#include <string>

// -------------------------------------------------------
// Lagrange: concrete interpolation class.
// Inherits Interpolation (which inherits Matrix).
//
// Inheritance chain:
//   Matrix  <--  Interpolation  <--  Lagrange
// -------------------------------------------------------
class Lagrange : public Interpolation
{
public:
    // Construct from a vector of data points
    explicit Lagrange(const std::vector<DataPoint>& points);

    // Construct by reading from a file
    explicit Lagrange(const std::string& filename);

    virtual ~Lagrange() = default;

    // ---------------------------------------------------------
    // Overrides of the pure-virtual Interpolation interface
    // ---------------------------------------------------------

    // Evaluate the Lagrange interpolating polynomial P(x)
    double evaluate(double x) const override;

    // Print the polynomial in expanded form, e.g. P(x) = x^2 + 3x + 2
    void printPolynomial() const override;

    // Interactive loop: user enters x values, sees P(x) and a running table
    void queryLoop() const override;

private:
    // ---------------------------------------------------------
    // Internal helpers
    // ---------------------------------------------------------

    // Compute the i-th Lagrange basis polynomial L_i(x)
    //   L_i(x) = product_{j!=i} (x - x_j) / (x_i - x_j)
    double basisValue(int i, double x) const;

    // Build the coefficient vector of L_i(x) as powers of x
    //   returned[k] = coefficient of x^k
    std::vector<double> basisCoeffs(int i) const;
};

#endif // LAGRANGE_H