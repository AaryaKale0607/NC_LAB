#include "../include/least_squares.hpp"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// Constructors — delegate entirely to CurveFitting, then build design matrix
// ─────────────────────────────────────────────────────────────────────────────
LeastSquares::LeastSquares(const vector<FitPoint>& points,
                           FitModel m,
                           int deg)
    : CurveFitting(points, m, deg)
{}

LeastSquares::LeastSquares(const string& filename,
                           FitModel m,
                           int deg)
    : CurveFitting(filename, m, deg)
{}

// ─────────────────────────────────────────────────────────────────────────────
// buildDesignMatrix
//
// Builds the Vandermonde-style matrix A and stores it in the inherited
// Matrix block (overwrites the (xi, fi) table temporarily during fit).
//
//   LINEAR / POLYNOMIAL  → n × (degree+1): row i = [1, xi, xi², …, xi^d]
//   EXPONENTIAL / POWER  → n × 2:          row i = [1, xi]  or [1, ln xi]
//   LOGARITHMIC          → n × 2:          row i = [1, ln xi]
// ─────────────────────────────────────────────────────────────────────────────
void LeastSquares::buildDesignMatrix()
{
    int n = static_cast<int>(dataPoints.size());
    int d = (model == FitModel::LINEAR)     ? 1
          : (model == FitModel::POLYNOMIAL) ? degree
                                            : 1;  // linearised models: 2 cols

    if (model == FitModel::LINEAR || model == FitModel::POLYNOMIAL) {
        reallocBlock(n, d + 1);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j <= d; ++j)
                (*this)(i, j) = pow(dataPoints[i].xi, j);
    } else {
        reallocBlock(n, 2);
        for (int i = 0; i < n; ++i) {
            (*this)(i, 0) = 1.0;
            if (model == FitModel::EXPONENTIAL)
                (*this)(i, 1) = dataPoints[i].xi;
            else if (model == FitModel::POWER)
                (*this)(i, 1) = log(dataPoints[i].xi);
            else if (model == FitModel::LOGARITHMIC)
                (*this)(i, 1) = log(dataPoints[i].xi);
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// buildRHS — the observed b vector (with log transform where needed)
// ─────────────────────────────────────────────────────────────────────────────
vector<double> LeastSquares::buildRHS() const
{
    int n = static_cast<int>(dataPoints.size());
    vector<double> b(n);

    for (int i = 0; i < n; ++i) {
        double f = dataPoints[i].fi;
        if (model == FitModel::EXPONENTIAL || model == FitModel::POWER) {
            if (f <= 0.0)
                throw runtime_error(
                    "LeastSquares: EXPONENTIAL/POWER require fi > 0 for all points.");
            b[i] = log(f);
        } else {
            b[i] = f;
        }
    }
    return b;
}

// ─────────────────────────────────────────────────────────────────────────────
// solveNormalEquations
//
// Builds augmented matrix [AtA | Atb] then applies Gaussian elimination
// with partial pivoting — identical strategy to GaussianSolver in this
// project, but self-contained so LeastSquares has no extra dependency.
// Uses inherited Matrix operator()(i,j) for element access.
// ─────────────────────────────────────────────────────────────────────────────
vector<double> LeastSquares::solveNormalEquations(const Matrix& AtA,
                                                   const vector<double>& Atb) const
{
    int m = static_cast<int>(Atb.size());

    // Augmented matrix [AtA | Atb]  — uses Matrix(r,c) constructor
    Matrix aug(m, m + 1);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j)
            aug(i, j) = AtA(i, j);
        aug(i, m) = Atb[i];
    }

    // Forward elimination with partial pivoting
    for (int col = 0; col < m; ++col) {
        int pivot = col;
        for (int row = col + 1; row < m; ++row)
            if (fabs(aug(row, col)) > fabs(aug(pivot, col)))
                pivot = row;

        if (pivot != col)
            for (int j = 0; j <= m; ++j)
                swap(aug(col, j), aug(pivot, j));

        double diag = aug(col, col);
        if (fabs(diag) < 1e-14)
            throw runtime_error("LeastSquares: singular normal-equation matrix — "
                                "add more data points or reduce polynomial degree.");

        for (int row = col + 1; row < m; ++row) {
            double factor = aug(row, col) / diag;
            for (int j = col; j <= m; ++j)
                aug(row, j) -= factor * aug(col, j);
        }
    }

    // Back substitution — same logic as Matrix::backwardSubstitution()
    vector<double> sol(m, 0.0);
    for (int i = m - 1; i >= 0; --i) {
        sol[i] = aug(i, m);
        for (int j = i + 1; j < m; ++j)
            sol[i] -= aug(i, j) * sol[j];
        sol[i] /= aug(i, i);
    }
    return sol;
}

// ─────────────────────────────────────────────────────────────────────────────
// fit — solve the normal equations  (AᵀA)c = Aᵀb
//
// Steps:
//  1. Build design matrix A into the inherited Matrix block
//  2. Compute AᵀA  (m × m)
//  3. Compute Aᵀb  (m-vector)
//  4. Solve via Gaussian elimination → raw coefficients
//  5. Back-transform for EXPONENTIAL / POWER (intercept = ln a → a = e^(ln a))
// ─────────────────────────────────────────────────────────────────────────────
void LeastSquares::fit()
{
    buildDesignMatrix();   // fills inherited Matrix block with A

    int n = rows;          // number of data points
    int m = cols;          // number of basis functions (degree+1 or 2)

    // ── AᵀA  (m × m) using Matrix operator()(i,j) ────────────────────────────
    Matrix AtA(m, m);
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j) {
            double sum = 0.0;
            for (int k = 0; k < n; ++k)
                sum += (*this)(k, i) * (*this)(k, j);
            AtA(i, j) = sum;
        }

    // ── Aᵀb  (m-vector) ───────────────────────────────────────────────────────
    vector<double> b    = buildRHS();
    vector<double> Atb(m, 0.0);
    for (int i = 0; i < m; ++i)
        for (int k = 0; k < n; ++k)
            Atb[i] += (*this)(k, i) * b[k];

    // ── Solve and store coefficients ──────────────────────────────────────────
    vector<double> raw = solveNormalEquations(AtA, Atb);

    if (model == FitModel::EXPONENTIAL || model == FitModel::POWER) {
        // linearised: raw[0] = ln(a), raw[1] = b  →  recover a = e^(ln a)
        coefficients = { exp(raw[0]), raw[1] };
    } else {
        coefficients = raw;
    }

    // Restore Matrix block to (xi, fi) table so display() stays meaningful
    syncToMatrix();
}

// ─────────────────────────────────────────────────────────────────────────────
// evaluate — compute ŷ at a given x using the fitted coefficients
// ─────────────────────────────────────────────────────────────────────────────
double LeastSquares::evaluate(double x) const
{
    if (coefficients.empty())
        throw runtime_error("LeastSquares: call fit() before evaluate().");

    switch (model) {
        case FitModel::LINEAR:
        case FitModel::POLYNOMIAL: {
            // Horner-like: c0 + c1*x + c2*x² + …
            double result = 0.0, xPow = 1.0;
            for (double c : coefficients) {
                result += c * xPow;
                xPow   *= x;
            }
            return result;
        }
        case FitModel::EXPONENTIAL:
            // y = a * e^(b*x)
            return coefficients[0] * exp(coefficients[1] * x);

        case FitModel::POWER:
            // y = a * x^b
            if (x <= 0.0)
                throw domain_error("LeastSquares: POWER model requires x > 0.");
            return coefficients[0] * pow(x, coefficients[1]);

        case FitModel::LOGARITHMIC:
            // y = a + b * ln(x)
            if (x <= 0.0)
                throw domain_error("LeastSquares: LOGARITHMIC model requires x > 0.");
            return coefficients[0] + coefficients[1] * log(x);
    }
    throw logic_error("LeastSquares::evaluate — unknown model.");
}

// ─────────────────────────────────────────────────────────────────────────────
// rootMeanSquareError
//
// Formula from the notebook (Image 3):
//
//   RMSE = ( (1 / N+1) * Σ_{i=0}^{N} (yi - fi)² )^(1/2)
//
// where yi = evaluate(xi) is the fitted value and fi is the observed value.
// N+1 = total number of points (the notebook uses N+1 as the count).
// ─────────────────────────────────────────────────────────────────────────────
double LeastSquares::rootMeanSquareError() const
{
    if (coefficients.empty())
        throw runtime_error("LeastSquares: call fit() before rootMeanSquareError().");

    int N1 = static_cast<int>(dataPoints.size());   // N+1 in notebook notation
    double sumSq = 0.0;
    for (const auto& p : dataPoints) {
        double residual = evaluate(p.xi) - p.fi;    // (yi - fi)
        sumSq          += residual * residual;
    }
    return sqrt(sumSq / static_cast<double>(N1));
}

// ─────────────────────────────────────────────────────────────────────────────
// printEquation — print the fitted equation
// ─────────────────────────────────────────────────────────────────────────────
void LeastSquares::printEquation() const
{
    if (coefficients.empty())
        throw runtime_error("LeastSquares: call fit() before printEquation().");

    cout << fixed << setprecision(6);

    switch (model) {
        case FitModel::LINEAR:
            // y = a*x + b   (coefficients[0]=b, coefficients[1]=a)
            cout << "\nFitted Linear Equation:\n";
            cout << "  y = " << coefficients[1] << " * x"
                 << (coefficients[0] >= 0 ? " + " : " - ")
                 << fabs(coefficients[0]) << "\n";
            break;

        case FitModel::POLYNOMIAL: {
            cout << "\nFitted Polynomial (degree " << degree << "):\n  y = ";
            bool first = true;
            for (int i = 0; i < static_cast<int>(coefficients.size()); ++i) {
                double c = coefficients[i];
                if (!first) cout << (c >= 0.0 ? " + " : " - ");
                else if (c < 0.0) cout << "-";
                cout << fabs(c);
                if      (i == 1) cout << "*x";
                else if (i >  1) cout << "*x^" << i;
                first = false;
            }
            cout << "\n";
            break;
        }
        case FitModel::EXPONENTIAL:
            cout << "\nFitted Exponential:\n  y = "
                 << coefficients[0] << " * e^(" << coefficients[1] << " * x)\n";
            break;

        case FitModel::POWER:
            cout << "\nFitted Power Law:\n  y = "
                 << coefficients[0] << " * x^" << coefficients[1] << "\n";
            break;

        case FitModel::LOGARITHMIC:
            cout << "\nFitted Logarithmic:\n  y = "
                 << coefficients[0]
                 << (coefficients[1] >= 0 ? " + " : " - ")
                 << fabs(coefficients[1]) << " * ln(x)\n";
            break;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// printSummary — full diagnostic output
//
// Mirrors the notebook layout (Images 2 & 3):
//   1. Data table  (xi, fi, xi², fi*xi, ŷi, (yi-fi)²)
//   2. Fitted equation
//   3. R²
//   4. RMSE
// ─────────────────────────────────────────────────────────────────────────────
void LeastSquares::printSummary() const
{
    if (coefficients.empty())
        throw runtime_error("LeastSquares: call fit() before printSummary().");

    const int W = 14;

    // ── Extended table (notebook style) ──────────────────────────────────────
    cout << "\n" << string(W * 6, '=') << "\n";
    cout << "  Least Squares Summary\n";
    cout << string(W * 6, '=') << "\n";
    cout << setw(W) << "xi"
         << setw(W) << "fi"
         << setw(W) << "xi^2"
         << setw(W) << "fi*xi"
         << setw(W) << "yi (fitted)"
         << setw(W) << "(yi-fi)^2"
         << "\n";
    cout << string(W * 6, '-') << "\n";

    double sumXi = 0, sumFi = 0, sumXi2 = 0, sumFiXi = 0, sumSqErr = 0;

    for (const auto& p : dataPoints) {
        double yi  = evaluate(p.xi);
        double err = yi - p.fi;
        cout << setw(W) << fixed << setprecision(4) << p.xi
             << setw(W) << p.fi
             << setw(W) << p.xi * p.xi
             << setw(W) << p.fi * p.xi
             << setw(W) << yi
             << setw(W) << err * err
             << "\n";
        sumXi   += p.xi;
        sumFi   += p.fi;
        sumXi2  += p.xi  * p.xi;
        sumFiXi += p.fi  * p.xi;
        sumSqErr+= err   * err;
    }

    cout << string(W * 6, '-') << "\n";
    cout << setw(W) << sumXi
         << setw(W) << sumFi
         << setw(W) << sumXi2
         << setw(W) << sumFiXi
         << setw(W) << ""
         << setw(W) << sumSqErr
         << "\n";
    cout << string(W * 6, '=') << "\n";

    // ── Fitted equation ───────────────────────────────────────────────────────
    printEquation();

    // ── R² and RMSE ───────────────────────────────────────────────────────────
    double rmse = rootMeanSquareError();
    double r2   = rSquared();

    cout << fixed << setprecision(8);
    cout << "\n  R²   = " << r2   << "\n";
    cout << "  RMSE = " << rmse << "\n";
    cout << "\n  (RMSE formula: sqrt( (1/(N+1)) * Σ(yi - fi)² ) )\n\n";
}