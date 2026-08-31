#include "../include/curve_fitting.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>
using namespace std;

void CurveFitting::reallocBlock(int r, int c)
{
    if (data != nullptr) {
        for (int i = 0; i < rows; ++i)
            delete[] data[i];
        delete[] data;
    }
    rows = r;
    cols = c;
    data = new double*[rows];
    for (int i = 0; i < rows; ++i)
        data[i] = new double[cols]();   // zero-initialise
}

void CurveFitting::syncToMatrix()
{
    int n = static_cast<int>(dataPoints.size());
    reallocBlock(n, 2);
    for (int i = 0; i < n; ++i) {
        (*this)(i, 0) = dataPoints[i].xi;
        (*this)(i, 1) = dataPoints[i].fi;
    }
}

CurveFitting::CurveFitting(const vector<FitPoint>& points)
    : Matrix(static_cast<int>(points.size()), 2),
      dataPoints(points)
{
    if (points.empty())
        throw runtime_error("CurveFitting: data point list is empty.");

    syncToMatrix();
}

CurveFitting::CurveFitting(const string& filename)
    : Matrix(1, 2)   // temporary; readTable() will reallocate
{
    readTable(filename);
}

void CurveFitting::readTable(const string& filename)
{
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
    syncToMatrix();
}

// writeTable — write (xi, fi) table to file in readTable() format
void CurveFitting::writeTable(const string& filename) const
{
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

void CurveFitting::feedPoint(double xi, double fi)
{
    dataPoints.push_back({xi, fi});
    coefficients.clear();   // fit is now stale — caller must re-call fit()
    syncToMatrix();
}

// displayTable — pretty-print the (xi, fi) table
void CurveFitting::displayTable() const
{
    const int W = 16;
    cout << "\n============================================\n";
    cout << "  Linear Least Squares Data  ("
         << dataPoints.size() << " points)\n";
    cout << "============================================\n";
    cout << setw(W) << "xi" << setw(W) << "fi" << "\n";
    cout << string(W * 2, '-') << "\n";
    for (const auto& p : dataPoints)
        cout << setw(W) << fixed << setprecision(6) << p.xi
             << setw(W) << p.fi << "\n";
    cout << "============================================\n";
}

const vector<FitPoint>& CurveFitting::getDataPoints()   const { return dataPoints;   }
const vector<double>&   CurveFitting::getCoefficients() const { return coefficients; }
int CurveFitting::getNumPoints() const
{
    return static_cast<int>(dataPoints.size());
}