#include "lu_solver.hpp"
#include <cmath>
#include <stdexcept>

LUSolver::LUSolver(int n, Method m)
    : SystemOfLinearEquation(n), method(m)
{
}

void LUSolver::solve()
{
    if (method == CROUT)
        croutDecomposition();
    else if (method == DOOLITTLE)
        doolittleDecomposition();
    else if (method == CHOLESKY)
        choleskyDecomposition();
}

void LUSolver::croutDecomposition()
{
    int n = rows;

    std::vector<std::vector<double>> L(n, std::vector<double>(n, 0));
    std::vector<std::vector<double>> U(n, std::vector<double>(n, 0));

    for (int j = 0; j < n; j++)
    {
        U[j][j] = 1;

        for (int i = j; i < n; i++)
        {
            double sum = 0;
            for (int k = 0; k < j; k++)
                sum += L[i][k] * U[k][j];

            L[i][j] = data[i][j] - sum;
        }

        for (int i = j + 1; i < n; i++)
        {
            double sum = 0;
            for (int k = 0; k < j; k++)
                sum += L[j][k] * U[k][i];

            U[j][i] = (data[j][i] - sum) / L[j][j];
        }
    }

    // You can add forward/back substitution here to compute solution
}

void LUSolver::doolittleDecomposition()
{
    int n = rows;

    std::vector<std::vector<double>> L(n, std::vector<double>(n, 0));
    std::vector<std::vector<double>> U(n, std::vector<double>(n, 0));

    for (int i = 0; i < n; i++)
    {
        L[i][i] = 1;

        for (int j = i; j < n; j++)
        {
            double sum = 0;
            for (int k = 0; k < i; k++)
                sum += L[i][k] * U[k][j];

            U[i][j] = data[i][j] - sum;
        }

        for (int j = i + 1; j < n; j++)
        {
            double sum = 0;
            for (int k = 0; k < i; k++)
                sum += L[j][k] * U[k][i];

            L[j][i] = (data[j][i] - sum) / U[i][i];
        }
    }
}

void LUSolver::choleskyDecomposition()
{
    int n = rows;

    std::vector<std::vector<double>> L(n, std::vector<double>(n, 0));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j <= i; j++)
        {
            double sum = 0;

            for (int k = 0; k < j; k++)
                sum += L[i][k] * L[j][k];

            if (i == j)
                L[i][j] = sqrt(data[i][i] - sum);
            else
                L[i][j] = (data[i][j] - sum) / L[j][j];
        }
    }
}