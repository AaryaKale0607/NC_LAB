#include "system_linear_eq.hpp"
#include <sstream>
#include <stdexcept>

SystemOfLinearEquation::SystemOfLinearEquation(int n)
    : Matrix(n, n + 1)//Initialize as augmented matrix (n x n+1)
{
}

void SystemOfLinearEquation::createAugmentedMatrix(const string &leftFilename,
                                                   const string &rightFilename,
                                                   const string &outputFilename) const
{
    ifstream left(leftFilename);
    ifstream right(rightFilename);
    ofstream out(outputFilename);

    if (!left || !right || !out)
        throw runtime_error("Cannot open files for augmented matrix creation.");

    string line;
    vector<double> b;
    double value;
    bool checkedFirstDataLine = false;

    while (right >> value)
        b.push_back(value);

    if (static_cast<int>(b.size()) != rows)
        throw runtime_error("Right-hand side vector size does not match the system size.");

    int row = 0;

    while (getline(left, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        vector<double> values;

        while (ss >> value)
            values.push_back(value);

        if (values.empty())
            continue;

        if (!checkedFirstDataLine)
        {
            checkedFirstDataLine = true;

            if (values.size() == 2 &&
                static_cast<int>(values[0]) == rows &&
                static_cast<int>(values[1]) == cols)
            {
                continue;
            }
        }

        if (row >= rows)
            throw runtime_error("Left-hand side matrix has more rows than expected.");

        if (static_cast<int>(values.size()) != cols - 1)
            throw runtime_error("Coefficient count does not match the system size.");

        for (double coefficient : values)
            out << coefficient << " ";

        out << b[row] << endl;
        row++;
    }

    if (row != rows)
        throw runtime_error("Left-hand side matrix row count does not match the system size.");
}

std::vector<double> SystemOfLinearEquation::getSolution() const//Returns the computed solution vector.
{
    return solution;
}
