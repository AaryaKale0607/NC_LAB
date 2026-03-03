#ifndef GAUSSIAN_SOLVER_HPP
#define GAUSSIAN_SOLVER_HPP

#include "system_linear_eq.hpp"

class GaussianSolver : public SystemOfLinearEquation
{
public:
    GaussianSolver(int n);

    void solve() override;

    
};

#endif



