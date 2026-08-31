#include <iostream>
#include <cmath>
#include <iomanip>
#include "numerical_differentiation.hpp"

using namespace std;

int main()
{
    double x = 2.0;

    Differentiation *methods[3];

    methods[0] = new ForwardDifference();
    methods[1] = new BackwardDifference();
    methods[2] = new CentralDifference();

    string names[3] = {"Forward Difference ", "Backward Difference", "Central Difference "};

    double exact = 3 * x * x - 1; // analytical derivative of x^3 - x - 2

    cout << fixed << setprecision(6);
    cout << "\nFunction : f(x) = x^3 - x - 2\n";
    cout << "At x = " << x << "\n";

    for(double h = 0.1 ; h >= 1e-10 ; h /= 10)
       {
           cout << "h = " << h << "\n" ;

            for (int i = 0; i < 3; i++)
               {
                   double result = methods[i]->solve(x, h);
                   double error = fabs(result - exact);

                   cout << names[i] << " : f'(x) = " << result
                        << "   Error = " << error << endl;
               }
       }

    cout << "\nExact derivative = " << exact << endl;

    for (int i = 0; i < 3; i++)
        delete methods[i];

    return 0;
}