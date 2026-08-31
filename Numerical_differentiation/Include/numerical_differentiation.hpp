class Differentiation
{
public:
    double func(double x);

    virtual double solve(double x, double h) = 0;
};

class ForwardDifference : public Differentiation
{
public:
    double solve(double x, double h) override;
};

class BackwardDifference : public Differentiation
{
public:
    double solve(double x, double h) override;
};

class CentralDifference : public Differentiation
{
public:
    double solve(double x, double h) override;
};