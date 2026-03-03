#ifndef LU_SOLVER_HPP
#define LU_SOLVER_HPP

#include "system_linear_eq.hpp"

class LUSolver : public SystemOfLinearEquation
{
public:
    enum Method
    {
        CROUT,
        DOOLITTLE,
        CHOLESKY
    };

private:
    Method method;

public:
    LUSolver(int n, Method m);

    void solve() override;

private:
    void croutDecomposition();
    void doolittleDecomposition();
    void choleskyDecomposition();
};

#endif