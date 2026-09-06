from root_finding import BisectionMethod, NewtonRaphson, FixedPointIteration
import math

if __name__ == "__main__":
    methods = [BisectionMethod(), NewtonRaphson(), FixedPointIteration()]
    print("Function : f(x) = x^3 - x - 2")
    for idx, method in enumerate(methods, start=1):
        root = method.solve(1e-4)
        if math.isnan(root):
            print(f"Method {idx}: Interval not found")
        else:
            print(f"Method {idx} Root = {root}")
