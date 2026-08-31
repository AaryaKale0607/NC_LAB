#ifndef LEAST_SQUARES_HPP
#define LEAST_SQUARES_HPP

#include "curve_fitting.hpp"

class LeastSquares : public CurveFitting
{
private:
    void buildDesignMatrix();

    // Build the right-hand side vector b = [f0, f1, …, fN-1]
    std::vector<double> buildRHS() const;

    std::vector<double> solveNormalEquations(const Matrix& AtA,
                                             const std::vector<double>& Atb) const;

public:
    explicit LeastSquares(const std::vector<FitPoint>& points);

    // Construct from a file and call fit() automatically
    explicit LeastSquares(const std::string& filename);

    virtual ~LeastSquares() = default;

    void   fit()                    override;

    // Evaluate the fitted line:  y = coefficients[1]*x + coefficients[0]
    double evaluate(double x) const override;

    // Print:  y = a*x + b
    void   printEquation()    const override;

    // ── Root Mean Square Error ────────────────────────────────────────────────
    // RMSE = sqrt( (1/N) * Σ (yi − fi)² )
    double rootMeanSquareError() const;

    // ── Full diagnostic summary ───────────────────────────────────────────────
    // Prints: data table (xi, fi, xi², fi*xi, ŷi, (yi-fi)²),
    //         fitted equation, R², and RMSE.
    void printSummary() const;
};

#endif 