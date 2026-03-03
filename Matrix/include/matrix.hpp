#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "include/linear_solver.hpp"

using namespace std;

class Matrix : public LinearSolver
{
private:
    int rows;
    int cols;
    double **data;
    vector<double> solution;

public:
    Matrix(int r, int c);
    Matrix(const Matrix &other);
    ~Matrix();

    void readFromFile(const string &filename) override;
    void writeToFile(const string &filename) const;

    Matrix add(const Matrix &other) const;
    Matrix subtract(const Matrix &other) const;

    void gaussianElimination(bool pivoting);  
    vector<double> backwardSubstitution() const;

    void solve(bool pivoting) override;
    vector<double> getSolution() const override;

    void display() const;
};

#endif