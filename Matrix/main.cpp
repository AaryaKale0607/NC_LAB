#include "gaussian_solver.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    try
    {
        int n = 225;

        // Base class pointer → Runtime Polymorphism
        SystemOfLinearEquation* solver = new GaussianSolver(n);

        solver->readFromFile("input1.txt");

        solver->solve();   // no bool parameter anymore

        auto solution = solver->getSolution();

        ofstream out("output1.txt");

        for (double val : solution)
            out << val << endl;

        out.close();

        delete solver;

        cout << "Solution written to output1.txt\n";
    }
    catch (exception &e)
    {
        cout << "Error: " << e.what() << endl;

        ofstream out("output.txt");
        out << "Computation failed.\n";
        out.close();
    }

    return 0;
}