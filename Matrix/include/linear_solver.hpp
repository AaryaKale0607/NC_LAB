#ifndef LINEAR_SOLVER_HPP
#define LINEAR_SOLVER_HPP

#include <vector>
#include <string>

using namespace std;

class LinearSolver
{
public:
    virtual void readFromFile(const string& filename) = 0;
    virtual void solve(bool pivoting) = 0;   // Pure virtual
    virtual vector<double> getSolution() const = 0;

    virtual ~LinearSolver() {}
};

#endif