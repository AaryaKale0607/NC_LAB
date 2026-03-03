#include "include/matrix.hpp"
#include <sstream>
#include <vector>
#include "include/linear_solver.hpp"

int main()
{
    try
    {
        int n = 225; 
        
        LinearSolver* solver = new Matrix(n, n + 1);

        solver->readFromFile("input1.txt");

        solver->solve(true);

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
