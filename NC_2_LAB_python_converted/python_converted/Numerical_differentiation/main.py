# from numerical_differentiation import ForwardDifference, BackwardDifference, CentralDifference

# if __name__ == "__main__":
#     x = 2.0
#     exact = 3 * x * x - 1
#     print(f"Function : f(x) = x^3 - x - 2")
#     print(f"At x = {x}")
#     methods = [
#         ("Forward Difference", ForwardDifference()),
#         ("Backward Difference", BackwardDifference()),
#         ("Central Difference", CentralDifference()),
#     ]
#     for h in [0.1 / (10 ** k) for k in range(0, 11)]:
#         print(f"h = {h}")
#         for name, method in methods:
#             value = method.solve(x, h)
#             error = abs(value - exact)
#             print(f"{name} : f'(x) = {value:.10f}   Error = {error:.10e}")
#     print(f"Exact derivative = {exact}")


from numerical_differentiation import ForwardDifference, BackwardDifference, CentralDifference
from Richardson import RichardsonExtrapolation
from Lagrange import LagrangeInterpolation
from Newton import NewtonInterpolation


def demo_differentiation():
    print("=" * 70)
    print("PART 1: Forward / Backward / Central Difference / Richardson")
    print("=" * 70)

    x = 2.0
    exact = 3 * x * x - 1
    print("Function : f(x) = x^3 - x - 2")
    print(f"At x = {x}")

    methods = [
        ("Forward Difference", ForwardDifference()),
        ("Backward Difference", BackwardDifference()),
        ("Central Difference", CentralDifference()),
        ("Richardson Extrapolation", RichardsonExtrapolation()),
    ]
    
    for h in [0.1 / (10 ** k) for k in range(0, 11)]:
        print(f"\nh = {h}")
        for name, method in methods:
            value = method.solve(x, h)
            error = abs(value - exact)
            print(f"{name} : f'(x) = {value:.10f}   Error = {error:.10e}")

    print(f"\nExact derivative = {exact}")


def demo_lagrange():
    print("\n" + "=" * 70)
    print("PART 2: Lagrange Interpolation")
    print("=" * 70)

    x_data = [0, 1, 2]
    y_data = [1, 3, 7]

    lag = LagrangeInterpolation(x_data, y_data)
    print(f"Data: x = {x_data}   f(x) = {y_data}")
    print(f"P(1.5)            = {lag.solve(1.5):.6f}")
    print(f"P'(1.5) (numeric) = {lag.differentiate(1.5):.6f}")


def demo_newton():
    print("\n" + "=" * 70)
    print("PART 3: Newton Divided-Difference Interpolation")
    print("=" * 70)

    x_data = [-2, -1, 0, 1, 2]
    y_data = [1, 0, 1, 4, 9]

    newt = NewtonInterpolation(x_data, y_data)
    newt.print_table()
    print(f"Coefficients: {newt.coeffs}")

    print(f"{'x':>6}{'P(x)':>12}{'exact':>12}{'P prime(x)':>14}{'exact deriv':>14}")
    for xv in [-1.5, -0.5, 0.5, 1.5]:
        p = newt.solve(xv)
        dp = newt.differentiate(xv)
        exact = xv ** 2 + 2 * xv + 1
        exact_deriv = 2 * xv + 2
        print(f"{xv:6.2f}{p:12.6f}{exact:12.6f}{dp:14.6f}{exact_deriv:14.6f}")


if __name__ == "__main__":
    demo_differentiation()
    demo_lagrange()
    demo_newton()