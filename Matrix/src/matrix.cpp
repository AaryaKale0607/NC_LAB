#include "../include/matrix.hpp"
#include <stdexcept>
#include <cmath>

Matrix::Matrix(int r, int c)
{
    if (r <= 0 || c <= 0)
        throw invalid_argument("Invalid matrix size.");

    rows = r;
    cols = c;

    data = new double *[rows];
    for (int i = 0; i < rows; i++)
        data[i] = new double[cols];
}

Matrix::Matrix(const Matrix &other)
{
    rows = other.rows;
    cols = other.cols;

    data = new double *[rows];
    for (int i = 0; i < rows; i++)
    {
        data[i] = new double[cols];
        for (int j = 0; j < cols; j++)
            data[i][j] = other.data[i][j];
    }
}

Matrix::~Matrix()
{
    for (int i = 0; i < rows; i++)
        delete[] data[i];
    delete[] data;
}

void Matrix::readFromFile(const string &filename)
{
    ifstream file(filename);
    if (!file)
        throw runtime_error("Cannot open input file.");

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            file >> data[i][j];

    file.close();
}

void Matrix::writeToFile(const string &filename) const
{
    ofstream file(filename);
    if (!file)
        throw runtime_error("Cannot open output file.");

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            file << data[i][j] << " ";
        file << endl;
    }
    file.close();
}

Matrix Matrix::add(const Matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
        throw runtime_error("Addition size mismatch.");

    Matrix result(rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.data[i][j] = data[i][j] + other.data[i][j];

    return result;
}

Matrix Matrix::subtract(const Matrix &other) const
{
    if (rows != other.rows || cols != other.cols)
        throw runtime_error("Subtraction size mismatch.");

    Matrix result(rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.data[i][j] = data[i][j] - other.data[i][j];

    return result;
}



void Matrix::solve(bool pivoting)
{
    gaussianElimination(pivoting);
    solution = backwardSubstitution();
}

std::vector<double> Matrix::getSolution() const
{
    return solution;
}