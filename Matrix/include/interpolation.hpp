#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include "matrix.hpp"
#include <vector>
#include <string>

// Structure to hold a data point (xi, f(xi))
struct DataPoint {
    double x;
    double fx;
};
class Interpolation : public Matrix
{
protected:
    std::vector<DataPoint> dataPoints; // stores raw interpolation data
    void loadMatrix();
public:
    explicit Interpolation(const std::vector<DataPoint>& points); //constructor initializes interpolation using a vector of data points

    explicit Interpolation(const std::string& filename);  //constructor reads interpolation data from a file
    virtual ~Interpolation() = default;

    virtual double evaluate(double x) const = 0; //compute P(x)

    virtual void printPolynomial() const = 0;

    virtual void queryLoop() const = 0;

    const std::vector<DataPoint>& getDataPoints() const;

    int getNumPoints() const;

    void displayData() const;
};

#endif



//use read file , display , print function from matrix class no need to write them explicitly