#include "interpolation.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>

void Interpolation::loadMatrix()//converts data points into matrix form
{
    int n = static_cast<int>(dataPoints.size());

    for (int i = 0; i < n; ++i) {
        (*this)(i, 0) = dataPoints[i].x;
        (*this)(i, 1) = dataPoints[i].fx;
    }
}

Interpolation::Interpolation(const std::vector<DataPoint>& points)
    : Matrix(static_cast<int>(points.size()), 2),
      dataPoints(points)
{
    if (points.empty())
        throw std::runtime_error("Interpolation: data point list is empty.");
    loadMatrix();
}

Interpolation::Interpolation(const std::string& filename)
    : Matrix(1, 2) // temporary size; will be resized after reading
{
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Interpolation: cannot open file: " + filename);

    int n;
    file >> n;
    if (n <= 0)
        throw std::runtime_error("Interpolation: number of data points must be positive.");

    dataPoints.resize(n);//allocates memory
    for (int i = 0; i < n; ++i) {
        if (!(file >> dataPoints[i].x >> dataPoints[i].fx))
            throw std::runtime_error("Interpolation: file has insufficient data. Expected "
                                     + std::to_string(n) + " points.");
    }
    file.close();

    // Resize the inherited Matrix data block to n x 2
    for (int i = 0; i < rows; ++i) delete[] data[i];
    delete[] data;
    rows = n;
    cols = 2;
    data = new double*[rows];
    for (int i = 0; i < rows; ++i)
        data[i] = new double[cols]();
    loadMatrix();
}

const std::vector<DataPoint>& Interpolation::getDataPoints() const
{
    return dataPoints;
}

int Interpolation::getNumPoints() const
{
    return static_cast<int>(dataPoints.size());
}

void Interpolation::displayData() const
{
    const int W = 16;
    std::cout << "\n============================================\n";
    std::cout << "  Data Points  (" << dataPoints.size() << " points)\n";
    std::cout << "============================================\n";
    std::cout << std::setw(W) << "x"
              << std::setw(W) << "f(x)" << "\n";
    std::cout << std::string(W * 2, '-') << "\n";
    for (const auto& p : dataPoints)
        std::cout << std::setw(W) << std::fixed << std::setprecision(6) << p.x
                  << std::setw(W) << p.fx << "\n";
    std::cout << "============================================\n";
}