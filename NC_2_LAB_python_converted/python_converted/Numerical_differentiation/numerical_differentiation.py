import math


class Differentiation:
    @staticmethod
    def func(x):
        return x * x * x - x - 2

    def solve(self, x, h):
        raise NotImplementedError


class ForwardDifference(Differentiation):
    def solve(self, x, h):
        return (self.func(x + h) - self.func(x)) / h


class BackwardDifference(Differentiation):
    def solve(self, x, h):
        return (self.func(x) - self.func(x - h)) / h


class CentralDifference(Differentiation):
    def solve(self, x, h):
        return (self.func(x + h) - self.func(x - h)) / (2 * h)


if __name__ == "__main__":
    x = 2.0
    exact = 3 * x * x - 1
    print(f"Function : f(x) = x^3 - x - 2")
    print(f"At x = {x}")
    methods = [
        ("Forward Difference", ForwardDifference()),
        ("Backward Difference", BackwardDifference()),
        ("Central Difference", CentralDifference()),
    ]
    for h in [0.1 / (10 ** k) for k in range(0, 11)]:
        print(f"h = {h}")
        for name, method in methods:
            value = method.solve(x, h)
            error = abs(value - exact)
            print(f"{name} : f'(x) = {value:.10f}   Error = {error:.10e}")
    print(f"Exact derivative = {exact}")
