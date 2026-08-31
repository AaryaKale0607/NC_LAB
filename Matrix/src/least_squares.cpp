#include "../include/least_squares.hpp"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;

LeastSquares::LeastSquares(const vector<FitPoint>& points)
    : CurveFitting(points)
{}

LeastSquares::LeastSquares(const string& filename)
    : CurveFitting(filename)
{}

void LeastSquares::buildDesignMatrix()
{
    int n = static_cast<int>(dataPoints.size());
    reallocBlock(n, 2);
    for (int i = 0; i < n; ++i) {
        (*this)(i, 0) = 1.0;              // constant term
        (*this)(i, 1) = dataPoints[i].xi; // linear term
    }
}

vector<double> LeastSquares::buildRHS() const
{
    int n = static_cast<int>(dataPoints.size());
    vector<double> b(n);
    for (int i = 0; i < n; ++i)
        b[i] = dataPoints[i].fi;
    return b;
}

vector<double> LeastSquares::solveNormalEquations(const Matrix& AtA,
                                                   const vector<double>& Atb) const
{
    int m = static_cast<int>(Atb.size());   // always 2 for the linear model

    // Build augmented matrix [AtA | Atb]
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
            throw runtime_error(
                "LeastSquares: singular normal-equation matrix — "
                "add more data points or check for duplicate xi values.");

        for (int row = col + 1; row < m; ++row) {
            double factor = aug(row, col) / diag;
            for (int j = col; j <= m; ++j)
                aug(row, j) -= factor * aug(col, j);
        }
    }

    // Back substitution
    vector<double> sol(m, 0.0);
    for (int i = m - 1; i >= 0; --i) {
        sol[i] = aug(i, m);
        for (int j = i + 1; j < m; ++j)
            sol[i] -= aug(i, j) * sol[j];
        sol[i] /= aug(i, i);
    }
    return sol;
}

void LeastSquares::fit()
{
    buildDesignMatrix();    // fills inherited Matrix block with A (N×2)

    int n = rows;           // number of data points
    int m = cols;           // always 2 for the linear model

    Matrix AtA(m, m);
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j) {
            double sum = 0.0;
            for (int k = 0; k < n; ++k)
                sum += (*this)(k, i) * (*this)(k, j);
            AtA(i, j) = sum;
        }

    vector<double> b   = buildRHS();
    vector<double> Atb(m, 0.0);
    for (int i = 0; i < m; ++i)
        for (int k = 0; k < n; ++k)
            Atb[i] += (*this)(k, i) * b[k];

    coefficients = solveNormalEquations(AtA, Atb);

    // Restore the Matrix block to the (xi, fi) table
    syncToMatrix();
}

// evaluate — compute ŷ at a given x
//   y = coefficients[1] * x + coefficients[0]
double LeastSquares::evaluate(double x) const
{
    if (coefficients.empty())
        throw runtime_error("LeastSquares: call fit() before evaluate().");

    return coefficients[1] * x + coefficients[0];
}

// printEquation — print the fitted line
//   y = a*x + b   (coefficients[0]=b, coefficients[1]=a)

void LeastSquares::printEquation() const
{
    if (coefficients.empty())
        throw runtime_error("LeastSquares: call fit() before printEquation().");

    double a = coefficients[1];   // slope
    double b = coefficients[0];   // intercept

    cout << fixed << setprecision(6);
    cout << "\nFitted Linear Equation:\n";
    cout << "  y = " << a << " * x"
         << (b >= 0.0 ? " + " : " - ")
         << fabs(b) << "\n";
}

//   RMSE = sqrt( (1/N) * Σ_{i=0}^{N-1} (yi - fi)² )

double LeastSquares::rootMeanSquareError() const
{
    if (coefficients.empty())
        throw runtime_error("LeastSquares: call fit() before rootMeanSquareError().");

    int N = static_cast<int>(dataPoints.size());
    double sumSq = 0.0;
    for (const auto& p : dataPoints) {
        double residual = evaluate(p.xi) - p.fi;
        sumSq          += residual * residual;
    }
    return sqrt(sumSq / static_cast<double>(N));
}

void LeastSquares::printSummary() const
{
    if (coefficients.empty())
        throw runtime_error("LeastSquares: call fit() before printSummary().");

    const int W = 14;

    cout << "\n" << string(W * 6, '=') << "\n";
    cout << "  Linear Least Squares Summary\n";
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
        sumXi    += p.xi;
        sumFi    += p.fi;
        sumXi2   += p.xi * p.xi;
        sumFiXi  += p.fi * p.xi;
        sumSqErr += err  * err;
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

    printEquation();

    double rmse = rootMeanSquareError();

    cout << fixed << setprecision(8);
    
    cout << "  RMSE = " << rmse << "\n";
    cout << "\n  (RMSE formula: sqrt( (1/N) * Σ(yi - fi)² ) )\n\n";
}