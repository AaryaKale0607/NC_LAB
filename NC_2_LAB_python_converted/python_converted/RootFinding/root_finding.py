import math


class RootFindingBase:
    @staticmethod
    def func(x):
        return x * x * x - x - 2

    @staticmethod
    def derivative(x):
        return 3 * x * x - 1

    @staticmethod
    def g(x):
        return math.cbrt(x + 2)


class BisectionMethod(RootFindingBase):
    def solve(self, tol=1e-4):
        step = 0.1
        found = False
        a = 0.0
        b = 0.0

        for i in [step * k for k in range(0, 1001)]:
            if self.func(i) * self.func(i + 1) < 0:
                a = i
                b = i + 1
                found = True
                break

        if not found:
            return float('nan')

        while (b - a) >= tol:
            m = (a + b) / 2.0
            if self.func(a) * self.func(m) < 0:
                b = m
            else:
                a = m
        return (a + b) / 2.0


class NewtonRaphson(RootFindingBase):
    def solve(self, tol=1e-4, x0=1.0):
        x = x0
        while True:
            x_next = x - self.func(x) / self.derivative(x)
            if abs(x_next - x) < tol:
                return x_next
            x = x_next


class FixedPointIteration(RootFindingBase):
    def solve(self, tol=1e-4, x0=1.0):
        x = x0
        while True:
            x_next = self.g(x)
            if abs(x_next - x) < tol:
                return x_next
            x = x_next


if __name__ == "__main__":
    methods = [BisectionMethod(), NewtonRaphson(), FixedPointIteration()]
    print("Function: f(x) = x^3 - x - 2")
    for idx, method in enumerate(methods, start=1):
        root = method.solve(1e-4)
        if math.isnan(root):
            print(f"Method {idx}: interval not found")
        else:
            print(f"Method {idx} root = {root}")
