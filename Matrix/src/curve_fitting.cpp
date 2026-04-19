#include "../include/curve_fitting.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
// reallocBlock — safely (re)allocate the inherited Matrix data block
// This is needed because Matrix uses raw double** managed by its destructor.
// ─────────────────────────────────────────────────────────────────────────────
void CurveFitting::reallocBlock(int r, int c)
{
    // Free existing block (parent fields: rows, cols, data)
    if (data != nullptr) {
        for (int i = 0; i < rows; ++i)
            delete[] data[i];
        delete[] data;
    }
    rows = r;
    cols = c;
    data = new double*[rows];
    for (int i = 0; i < rows; ++i)
        data[i] = new double[cols]();  // zero-initialise
}

// ─────────────────────────────────────────────────────────────────────────────
// syncToMatrix — mirror dataPoints vector into the inherited (N×2) Matrix block
// Column 0 = xi,  Column 1 = fi
// Uses the inherited operator()(i,j) — no code duplication.
// ─────────────────────────────────────────────────────────────────────────────
void CurveFitting::syncToMatrix()
{
    int n = static_cast<int>(dataPoints.size());
    reallocBlock(n, 2);
    for (int i = 0; i < n; ++i) {
        (*this)(i, 0) = dataPoints[i].xi;
        (*this)(i, 1) = dataPoints[i].fi;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Constructor — from vector of FitPoints
// ─────────────────────────────────────────────────────────────────────────────
CurveFitting::CurveFitting(const vector<FitPoint>& points,
                           FitModel m,
                           int deg)
    : Matrix(static_cast<int>(points.size()), 2),
      dataPoints(points),
      model(m),
      degree(deg)
{
    if (points.empty())
        throw runtime_error("CurveFitting: data point list is empty.");
    if (m == FitModel::POLYNOMIAL && deg < 1)
        throw invalid_argument("CurveFitting: polynomial degree must be >= 1.");

    syncToMatrix();  // populate inherited Matrix block
}

// ─────────────────────────────────────────────────────────────────────────────
// Constructor — from filename (delegates to readTable)
// ─────────────────────────────────────────────────────────────────────────────
CurveFitting::CurveFitting(const string& filename,
                           FitModel m,
                           int deg)
    : Matrix(1, 2),   // temporary; readTable() will reallocate
      model(m),
      degree(deg)
{
    if (m == FitModel::POLYNOMIAL && deg < 1)
        throw invalid_argument("CurveFitting: polynomial degree must be >= 1.");

    readTable(filename);  // read + sync in one call
}

// ─────────────────────────────────────────────────────────────────────────────
// readTable — read N (xi, fi) pairs from a text file
//
// File format (same convention as Interpolation / Lagrange in this project):
//   Line 1       :  N          (integer)
//   Lines 2…N+1  :  xi   fi   (space-separated doubles)
//
// After reading, syncs data into the inherited Matrix block so Matrix::display()
// and Matrix::writeToFile() both work on the live data.
// ─────────────────────────────────────────────────────────────────────────────
void CurveFitting::readTable(const string& filename)
{
    // Use ifstream exactly as Matrix::readFromFile() does internally
    ifstream file(filename);
    if (!file.is_open())
        throw runtime_error("CurveFitting::readTable — cannot open file: " + filename);

    int n;
    file >> n;
    if (n <= 0)
        throw runtime_error("CurveFitting::readTable — N must be positive.");

    dataPoints.resize(n);
    for (int i = 0; i < n; ++i) {
        if (!(file >> dataPoints[i].xi >> dataPoints[i].fi))
            throw runtime_error("CurveFitting::readTable — insufficient data. "
                                "Expected " + to_string(n) + " points.");
    }
    file.close();

    coefficients.clear();   // any previous fit is now stale
    syncToMatrix();         // update inherited Matrix block
}

// ─────────────────────────────────────────────────────────────────────────────
// writeTable — write (xi, fi) data table to file
//
// Writes in the same format readTable() expects so files round-trip.
// The inherited Matrix block is already in sync, so we could also call
// Matrix::writeToFile() to dump raw numbers; here we write the full
// header + pairs so the file is self-describing.
// ─────────────────────────────────────────────────────────────────────────────
void CurveFitting::writeTable(const string& filename) const
{
    // Open file the same way Matrix::writeToFile() does
    ofstream file(filename);
    if (!file.is_open())
        throw runtime_error("CurveFitting::writeTable — cannot open file: " + filename);

    int n = static_cast<int>(dataPoints.size());
    file << n << "\n";
    file << fixed << setprecision(10);
    for (const auto& p : dataPoints)
        file << p.xi << "  " << p.fi << "\n";

    file.close();
}

// ─────────────────────────────────────────────────────────────────────────────
// feedPoint — append a single (xi, fi) pair at runtime
// ─────────────────────────────────────────────────────────────────────────────
void CurveFitting::feedPoint(double xi, double fi)
{
    dataPoints.push_back({xi, fi});
    coefficients.clear();   // fit is now stale — caller must re-call fit()
    syncToMatrix();         // keep Matrix block in sync
}

// ─────────────────────────────────────────────────────────────────────────────
// displayTable — pretty-print the (xi, fi) table
//
// Prints a formatted header then delegates to the inherited Matrix::display()
// which prints the same data from the internal block — no duplication.
// ─────────────────────────────────────────────────────────────────────────────
void CurveFitting::displayTable() const
{
    const int W = 16;
    cout << "\n============================================\n";
    cout << "  Curve Fitting Data  (" << dataPoints.size() << " points)\n";
    cout << "============================================\n";
    cout << setw(W) << "xi" << setw(W) << "fi" << "\n";
    cout << string(W * 2, '-') << "\n";
    for (const auto& p : dataPoints)
        cout << setw(W) << fixed << setprecision(6) << p.xi
             << setw(W) << p.fi << "\n";
    cout << "============================================\n";

    // The Matrix block holds identical data — display() shows it as a matrix:
    // Uncomment the line below to also print the raw matrix view:
    // Matrix::display();
}

// ─────────────────────────────────────────────────────────────────────────────
// rSquared — coefficient of determination R²
//   R² = 1 − SS_res / SS_tot
// ─────────────────────────────────────────────────────────────────────────────
double CurveFitting::rSquared() const
{
    if (coefficients.empty())
        throw runtime_error("CurveFitting: call fit() before rSquared().");

    int n = static_cast<int>(dataPoints.size());

    // Mean of observed fi
    double fMean = 0.0;
    for (const auto& p : dataPoints) fMean += p.fi;
    fMean /= n;

    double ssTot = 0.0, ssRes = 0.0;
    for (const auto& p : dataPoints) {
        double yi   = evaluate(p.xi);         // fitted value
        ssTot      += (p.fi - fMean) * (p.fi - fMean);
        ssRes      += (p.fi - yi)    * (p.fi - yi);
    }

    return (ssTot == 0.0) ? 1.0 : 1.0 - ssRes / ssTot;
}

// ─────────────────────────────────────────────────────────────────────────────
// queryLoop — interactive evaluate loop
// ─────────────────────────────────────────────────────────────────────────────
void CurveFitting::queryLoop() const
{
    if (coefficients.empty())
        throw runtime_error("CurveFitting: call fit() before queryLoop().");

    cout << "\n--- Curve Evaluation (enter 'q' to quit) ---\n";
    string input;
    while (true) {
        cout << "Enter x: ";
        cin >> input;
        if (input == "q" || input == "Q") break;
        try {
            double x = stod(input);
            cout << fixed << setprecision(8)
                 << "  y(" << x << ") = " << evaluate(x) << "\n";
        } catch (...) {
            cout << "  Invalid input — try again.\n";
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Accessors
// ─────────────────────────────────────────────────────────────────────────────
const vector<FitPoint>& CurveFitting::getDataPoints()   const { return dataPoints;   }
const vector<double>&   CurveFitting::getCoefficients() const { return coefficients; }
int   CurveFitting::getNumPoints() const { return static_cast<int>(dataPoints.size()); }
FitModel CurveFitting::getModel()  const { return model;  }
int   CurveFitting::getDegree()    const { return degree; }