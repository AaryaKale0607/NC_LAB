#ifndef LEAST_SQUARES_HPP
#define LEAST_SQUARES_HPP

#include "curve_fitting.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// LeastSquares — concrete least-squares curve fitting
//
// Inherits: LeastSquares → CurveFitting → Matrix
//
// Fits the chosen model by minimising Σ(fi - ŷi)²
// using the normal equations:   (AᵀA) c = Aᵀb
//
// where:
//   A  = design matrix (Vandermonde for polynomial, linearised for others)
//   b  = observed fi values (possibly log-transformed)
//   c  = unknown coefficients
//
// Key metric added here — Root Mean Square Error:
//
//   RMSE = ( (1 / N+1) * Σ (yi − fi)² )^(1/2)
//
//   This is exactly the formula from the notebook images.
// ─────────────────────────────────────────────────────────────────────────────
class LeastSquares : public CurveFitting
{
private:
    // Build design matrix A (stored in inherited Matrix block)
    void buildDesignMatrix();

    // Build right-hand side vector b (log-transformed for EXP / POWER)
    std::vector<double> buildRHS() const;

    // Solve (m × m) normal-equation system via Gaussian elimination
    // with partial pivoting — reuses Matrix operator()() for element access
    std::vector<double> solveNormalEquations(const Matrix& AtA,
                                             const std::vector<double>& Atb) const;

public:
    // ── Constructors ─────────────────────────────────────────────────────────
    LeastSquares(const std::vector<FitPoint>& points,
                 FitModel model = FitModel::LINEAR,
                 int degree = 1);

    // File constructor — reads table then calls fit() automatically
    explicit LeastSquares(const std::string& filename,
                          FitModel model = FitModel::LINEAR,
                          int degree = 1);

    virtual ~LeastSquares() = default;

    // ── CurveFitting overrides ────────────────────────────────────────────────
    void   fit()                    override; // solve normal equations
    double evaluate(double x) const override; // evaluate fitted model at x
    void   printEquation()    const override; // print the fitted equation

    // ── Root Mean Square Error ────────────────────────────────────────────────
    // Formula (from notebook):
    //   RMSE = sqrt( (1/(N+1)) * Σ_{i=0}^{N} (yi - fi)² )
    // where yi = evaluate(xi) is the fitted value and fi is the observed value
    double rootMeanSquareError() const;

    // ── Full diagnostic summary ───────────────────────────────────────────────
    // Prints: data table, fitted equation, R², RMSE, and residuals table
    void printSummary() const;
};

#endif // LEAST_SQUARES_HPP