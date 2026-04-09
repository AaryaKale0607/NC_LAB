#ifndef LAGRANGE_H
#define LAGRANGE_H

#include "interpolation.hpp"
#include <vector>
#include <string>

class Lagrange : public Interpolation
{
public:
    explicit Lagrange(const std::vector<DataPoint>& points); //constructor initializes the Lagrange object using a vector of data points

    explicit Lagrange(const std::string& filename); //constructor reads interpolation data from a file

    virtual ~Lagrange() = default;

    double evaluate(double x) const override; //compute value of polynomial at x , override base class function

    void printPolynomial() const override; // Print the polynomial in expanded form, e.g. P(x) = x^2 + 3x + 2

    void queryLoop() const override;

private:
    double basisValue(int i, double x) const; //calculates the ith Lagrange basis value at a given x

    std::vector<double> basisCoeffs(int i) const; //generates polynomial coefficients
};

#endif