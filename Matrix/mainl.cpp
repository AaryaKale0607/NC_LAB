//  main.cpp  —  Lagrange's Interpolation
//
//  Usage:
//      ./lagrange <data_file>
//
//  Flow:
//    1. Read data from file
//    2. Print data table
//    3. Print interpolating polynomial P(x)
//    4. Ask user for x values → show updated table with P(x)

#include "Lagrange.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <data_file>\n"
                  << "Example: " << argv[0] << " input.txt\n";
        return EXIT_FAILURE;
    }

    // ---- 1. Read data ----
    std::vector<DataPoint> data;
    try {
        data = readDataFromFile(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    int n = static_cast<int>(data.size());

    // ---- 2. Print input data table ----
    std::cout << "============================================\n";
    std::cout << "  Lagrange's Interpolation\n";
    std::cout << "============================================\n";
    std::cout << "  File   : " << argv[1] << "\n";
    std::cout << "  Points : " << n << "\n";
    std::cout << "--------------------------------------------\n";
    std::cout << std::setw(6)  << "i"
              << std::setw(14) << "x_i"
              << std::setw(14) << "f(x_i)\n";
    std::cout << "--------------------------------------------\n";
    for (int i = 0; i < n; ++i) {
        std::cout << std::setw(6)  << i
                  << std::setw(14) << std::fixed << std::setprecision(6) << data[i].x
                  << std::setw(14) << data[i].fx << "\n";
    }

    // ---- 3. Print polynomial equation ----
    try {
        printPolynomial(data);
    } catch (const std::exception& e) {
        std::cerr << "Polynomial error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    // ---- 4. Interactive query ----
    queryLoop(data);

    std::cout << "\nDone.\n";
    return EXIT_SUCCESS;
}